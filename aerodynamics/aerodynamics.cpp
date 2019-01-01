#include <algorithm>
#include "aerodynamics/aerodynamics.h"
#include "math.h"
#include <iostream>


double Aerodynamics::CxTrF(double SmidLA, double Dmid, double Lnose, double Lcyl,
                           double LStern, double Dstern, double M, double sound_sp, double cin_visc, double ht){
    double
    Lf=Lcyl+Lnose+LStern,
    lambNose{Lnose/Dmid},
    lambcyl{Lcyl/Dmid},
    lambStern{LStern/Dmid},
    nyStern{Dmid/Dstern},
    Smid{M_PI*pow(Dmid/2.0,2)/2.0},
    Fcyl{2*Smid*(2*lambcyl-lambNose*(1-nyStern)-lambStern*(1-nyStern))},
    Re{sound_sp*M*Lf/cin_visc},
    n{5+(1.3+0.6*M*(1-(0.25*M*M)))*
                sqrt(
                    fabs(1-
                         pow(log10((ht*Re/Lf)-1)*
                             (1+0.312*M*M)/(2.2+0.7664*M*M),
                             2)
                         )
                    )
     },
    xt{std::min(pow(10,n/Re),Lnose/Lf)},
    cf2{0},nym{0};
    if(Re<485000){
        cf2=2.656/sqrt(Re);
        nym=pow(1+0.1*M*M,-0.125);
    }
    else if(Re<1E07){
        cf2=0.91*pow((1-xt+40*pow(xt,0.625)/pow(Re,0.375)), 0.8)
                /pow(log10(Re),2.58);
        nym=pow(1+0.1*M*M,-0.000000057*Re -0.097);
    }
    else{
        cf2=0.91/pow(log10(Re),2.58);
        nym=pow(1+0.1*M*M,-0.666666666);
    }
    return 0.5*cf2*nym*Fcyl/SmidLA;//отнесен к площади ЛА
}

double Aerodynamics::CxTrPlane(double SmidLA,double h, double Broot,
                               double Btip,double croot, double ctip,
                               double M,
                               double sound_sp, double cin_visc,
                               double ht){//a sound v, cinematic(h)
    double   baverage{(Broot+Btip)/2.0},
    x0{0.5*M_PI-fabs(atan(2*h/(Broot-Btip)))},
    c{0.5*(croot+ctip)/baverage},//отн толщина профиля
    nyc{1+2*c+9*c*c},
    Re{sound_sp*M*baverage/cin_visc},
    n{5+(1.3+0.6*M*(1-0.25*M*M))*
                sqrt(
                    fabs(1-
                         pow(log10((ht*(Re/baverage))-1)*
                             (1+0.312*M*M)/(2.2+0.7664*M*M) , 2)
                         )
                    )
     },
    xtpr{
        std::min(pow(10,n)/Re,
                 fabs(2*h/tan(0.5*M_PI-x0))/baverage)
    },
    xt{xtpr*(1-0.951*sin(x0)+0.047*sin(3.0*x0)+0.013*sin(5.0*x0))},
    cf2{0},
    nym{0};

    if(Re<485000){
        cf2=2.656/sqrt(Re);
        nym=pow(1+0.1*M*M,-0.125);
    }
    else if(Re<1E07){
        cf2=0.91*pow((1-xt+40*pow(xt,0.625)/pow(Re,0.375)), 0.8)
                /pow(log10(Re),2.58);
        nym=pow(1+0.1*M*M,-0.000000057*Re -0.097);
    }
    else{
        cf2=0.91/pow(log10(Re),2.58);
        nym=pow(1+0.1*M*M,-0.66666666);
    }
    return cf2*nym*nyc * 0.5*(Broot+Btip)*h/SmidLA;//для 1 консоли, пересчитать для n к площади ЛА
}

double Aerodynamics::CxpNoseCone(double len, double d,double Dmid, double M){
    if(M<1)M=1;
    return newtonInterp::interp3d(aerodynamics_tables::get_5_1_CxpNoseCone(),M,len/d,2,2)*d*d/(Dmid*Dmid);//к миделю корпуса
}

double Aerodynamics::CxpConoid(double len, double d1, double d2, double Dmid, double M){
    return CxpNoseCone(d2*len/(d2-d1),d2,Dmid,M)*(1-pow(d1/d2,2))*d2*d2/(Dmid*Dmid);//отнесен к миделю корпуса
}

double Aerodynamics::CxpSternBottom(double len, double Dmid, double DStern, double Dnozzle, double M, bool isEnActive){
    double nuk=DStern/Dmid;
    double cx=newtonInterp::interp4d(aerodynamics_tables::get_5_5_CxpStern(),M,len/Dmid,DStern/Dmid,3,3,2)*
            DStern*DStern/(Dmid*Dmid)+
             newtonInterp::interp2d(aerodynamics_tables::get_5_8_CpSternNu1(),M,3)*
              newtonInterp::interp3d(aerodynamics_tables::get_5_9_knu(),(1-nuk)/(2*len/Dmid-nuk*nuk),M,3,2)*
              (DStern*DStern-isEnActive*Dnozzle*Dnozzle)/
               (Dmid*Dmid);//к миделю корпуса
    while(cx<0){
        M>1?M--:M++;
        cx=newtonInterp::interp4d(aerodynamics_tables::get_5_5_CxpStern(),M,len/Dmid,DStern/Dmid,3,3,2)*
                DStern*DStern/(Dmid*Dmid)+
                 newtonInterp::interp2d(aerodynamics_tables::get_5_8_CpSternNu1(),M,3)*
                  newtonInterp::interp3d(aerodynamics_tables::get_5_9_knu(),(1-nuk)/(2*len/Dmid-nuk*nuk),M,3,2)*
                  (DStern*DStern-isEnActive*Dnozzle*Dnozzle)/
                   (Dmid*Dmid);//к миделю корпуса
    }
    return cx;
}

double Aerodynamics::CxWavPlaneOneConsole(double SmidLA, double broot, double btip,double k, double croot, double ctip,double h,  double anglecmax, double M){
    double   baverage{(broot+btip)/2.0},
    c{(croot+ctip)/2},//отн толщина профиля
    lambdakr=h/baverage,//удлинение (отношение размаха 2 консолей к средней хорде)
    nkr{broot/(btip+0.0001)},//сужение
    M0=1-0.7*sqrt(c),
    Mcr=M0+(0.9*pow(tan(anglecmax),1.2)+0.3*pow(lambdakr,-1.5))*(1-M0)*(M0-0.4);//критическое число маха
    if(M<Mcr)return 0;
    double cx= (0.5*(croot+ctip)*h/SmidLA)*
            newtonInterp::interp4d(aerodynamics_tables::get_5_10_CxWavePlane(),lambdakr*sqrt(M*M-1),lambdakr*tan(anglecmax),nkr,3,3,2)*
             (1+newtonInterp::interp2d(aerodynamics_tables::get_5_13_fi_CxWavePlane(),sqrt(M*M-1)*tan(anglecmax),2)*(k-1)
             )*(lambdakr*c*c);//уже отнесен к площади ла, 1 крыло(половина консоли)
    while(cx<0){
        M>1?M--:M++;
        if(M<Mcr)return 0;
        cx=(0.5*(croot+ctip)*h/SmidLA)*
                newtonInterp::interp4d(aerodynamics_tables::get_5_10_CxWavePlane(),lambdakr*sqrt(M*M-1),lambdakr*tan(anglecmax),nkr,3,3,2)*
                 (1+newtonInterp::interp2d(aerodynamics_tables::get_5_13_fi_CxWavePlane(),sqrt(M*M-1)*tan(anglecmax),2)*(k-1)
                 )*(lambdakr*c*c);//уже отнесен к площади ла, 1 крыло(половина консоли)
    }
    return cx;
}

double Aerodynamics::CyaNoseCyll(double Dmid, double SmidLA, double Lnose, double Lcyl, double M){
    double koef=0.25*Dmid*Dmid*M_PI/SmidLA;
    if(M<1.0)return 0.035*koef;
    else if(M>5)M=5;
    return newtonInterp::interp3d(aerodynamics_tables::get_7_2_CyaNoseCyll(),
                                  sqrt(M*M-1)*Dmid/Lnose,
                                  Lcyl/Lnose,3,2)*koef;//приведен к площади ЛА
}

double Aerodynamics::CyaOnePlaneConsole(double Dmid, double SmidLA,double X0, double broot, double btip, double croot, double ctip,double h,  double anglecmax, double M,double sound_sp, double cin_visc){
    double   baverage {(broot+btip)/2.0},
    c {(croot+ctip)/2},//отн толщина профиля
    lambdakr {h/baverage},//удлинение (отношение размаха 1 консолей к средней хорде)
    nkr {broot/(btip+0.0001)},//сужение
    cyaoneplane {newtonInterp::interp4d(aerodynamics_tables::get_7_5_CyaPlane(),lambdakr*sqrt(fabs(M*M-1)),lambdakr*pow(c,
          1.0/3.0),lambdakr*tan(anglecmax),3,3,2)
    },
    D {0.5*Dmid/h},
    L1 {X0+0.5*broot},
    Kaa0 {pow(1+0.4*D,2)*(1+3*D+D*(1-D)/nkr)/pow(1+D,2)},
    sigma {0.093*L1*(1+0.4*M+0.147*M*M-0.006*M*M*M)/pow(M*sound_sp*L1/cin_visc,0.2)},
    psi_border {(1-2*D*D*sigma/(1-D*D))*(1-D*(nkr-1)*sigma/((1-D)*(1+nkr)))},
    psi_m {0.0221*M*M-0.1815*M+1.2225},
    psi_n {0.6+0.4*(1-exp(-0.5*(X0+0.5*(broot-croot))/D))};

    return cyaoneplane*Kaa0*psi_border*psi_m*psi_n * 0.5*(broot+btip)*h/SmidLA;//для одной консоли к площади ЛА
}

double Aerodynamics::xpf(double Dmid,double Lnose,double Lcyl, double M){
    if(M<1)return 0.05;
    return newtonInterp::interp3d(aerodynamics_tables::get_11_2_deltaXf(),sqrt(M*M-1)*Dmid/Lnose,Lcyl/Lnose,3,2);
}


