#include <math.h>
#include <iostream>
#include "plane.h"
#include "aerodynamics/aerodynamics.h"

const std::string planeheader{"plane"};
double plane::Smid() const{return 0.5*(params.croot+params.ctip)*params.h*params.n;}

double plane::mass() const{return  params.massn;}

void plane::setmass(double m){params.massn=m;}

double plane::massCenter() const{return 0.6*params.broot+params.XfromNose;}

double plane::getX0() const{return params.XfromNose;}

void plane::setX0(double x0){params.XfromNose=x0;}

double plane::getL() const{return params.broot;}

std::string plane::getname() const{return  name;}

void plane::setname(std::string n){name=n;}

std::string plane::toString() const{return "plane";}

double plane::getCp(double SmidLA,double M) const{
    if(M<1)return Smid()*(0.1*M+0.06)/SmidLA;
 //   std::cout<<SmidLA<<std::endl;
    return 0.5*params.n*Smid()*Aerodynamics::CxWavPlaneOneConsole(SmidLA,
                                    params.broot,params.btip,
                                    (params.broot+params.btip)/(params.broot+params.btip-params.xrf-params.xrr-params.xtf-params.xtr),
                                    params.croot,params.ctip,
                                    params.h,params.angleCmax,M)/SmidLA;
}

double plane::getXp(double M) const{
    double k=M>1?0.45:0.35;
    return (0.5*(params.broot+params.btip)*k)+params.XfromNose;
}

double plane::getCxtr(double SmidLA, double M, double sound_sp, double cin_visc) const{
    return Aerodynamics::CxTrPlane(SmidLA,params.h,params.broot,params.btip,params.croot,params.ctip,M,sound_sp,cin_visc);
}

double plane::getcyaConsole(double Dmid, double SmidLA, double M, double sound_sp, double cin_visc) const{
    return Aerodynamics::CyaOnePlaneConsole(Dmid,SmidLA,params.XfromNose,params.broot,
                                            params.btip,params.croot,params.ctip,
                                            params.h,params.angleCmax,M,sound_sp,cin_visc);
}

planeparams plane::getparams() const{
    return params;
}

plane::plane(material math,
             double Xfromnose,
             double Broot, double Btip,
             double Croot, double Ctip,
             double Xtip, double Xrf,
             double Xrr, double Xtf, double Xtr,
             double H, double N){

    params.mat=math;
    params.XfromNose=Xfromnose;
    params.broot=Broot;
    params.btip=Btip;
    params.croot=Croot;
    params.ctip=Ctip;
    params.xtip=Xtip;
    params.xrf=Xrf;
    params.xrr=Xrr;
    params.xtf=Xtf;
    params.xtr=Xtr;
    params.h=H;
    params.n=N;
    params.angle0=atan(H/(fabs(Xtip)>0.00001?Xtip:0.00001));//поправка для деления));
    params.angleCmax=atan(params.h/(params.xtip-params.xrf+params.xtf));
    params.massn=params.n*0.5*(params.btip+params.broot)*params.h*0.5*(params.ctip+params.croot)*params.mat.Ro;
}

std::ostream &operator<<(std::ostream &os, const plane &pl){
    planeparams par=pl.getparams();
    return os<<planeheader<<'{'
                 <<par.mat<<','
           <<par.XfromNose<<','
               <<par.broot<<','
                <<par.btip<<','
               <<par.croot<<','
                <<par.ctip<<','
                <<par.xtip<<','
                 <<par.xrf<<','
                 <<par.xrr<<','
                 <<par.xtf<<','
                 <<par.xtr<<','
                   <<par.h<<','
                   <<par.n<<'}';
}

std::istream &operator>>(std::istream &in, plane &pl){
    std::string header;
    material mat;
    int tmp{0};
    char delim1{0},delim2{0},delim3{0},delim4{0},delim5{0},
         delim6{0},delim7{0},delim8{0},delim9{0},delim10{0},delim11{0},footer{0};
    double Xfromnose{0},Broot{0},Btip{0},Croot{0},Ctip{0},
            Xtip{0},Xrf{0},Xrr{0},Xtf{0},Xtr{0},H{0},N{0};

    while((tmp=in.get())!=EOF && isspace(tmp));
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=planeheader){
        in.clear(std::ios::failbit);
        return in;
    }

    in>>mat>>delim1>>
            Xfromnose>>delim2>>
            Broot>>delim3>>
            Btip>>delim4>>
            Croot>>delim5>>
            Ctip>>delim6>>
            Xtip>>delim7>>
            Xrf>>delim8>>
            Xrr>>delim9>>
            Xtf>>delim10>>
            Xtr>>delim11>>
            H>>delim5>>
            N>>footer;

    if(!in)return in;
    if(delim1!=delim2 ||
            delim2!=delim3||
            delim3!=delim4||
            delim4!=delim5||
            delim5!=delim6||
            delim6!=delim7||
            delim7!=delim8||
            delim8!=delim9||
           delim9!=delim10||
          delim10!=delim11||
              delim11!=','||
               footer!='}'||
               Xfromnose<0||
                   Broot<0||
                    Btip<0||
                   Croot<0||
                    Ctip<0||
                    Xtip<0||
                     Xrf<0||
                     Xrr<0||
                     Xtf<0||
                     Xtr<0||
             Xrf+Xrr>Broot||
              Xtf+Xtr>Btip||H<0||N<0){
        in.clear(std::ios::failbit);
        return in;
    }
    pl=plane(mat,Xfromnose,Broot,Btip,Croot,Ctip,Xtip,Xrf,Xrr,Xtf,Xtr,H,N);
    return in;
}
