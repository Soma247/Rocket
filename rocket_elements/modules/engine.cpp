#include "engine.h"
#include <iostream>
#include <math.h>

double engine::S() const{return 0.25*M_PI*d*d;}

double engine::mass() const{return 0;}

double engine::massCenter() const{return 0;}

double engine::getCp(double Dmid, double M) const{return 0;}

double engine::getX0() const{return X0;}

void engine::setX0(double x){X0=x;}

double engine::getL() const{return len;}

void engine::setL(double length){len=length;}

double engine::getDend() const{return d;}

void engine::setDend(double d){}

std::string engine::getname() const{return "";}

void engine::setname(std::string n){name=n;}

std::string engine::toString() const{return "";}

double engine::getdelt() const{return delt;}

void engine::setdelt(double d){}

engine::engine(matherial mathShell, matherial mathbr, matherial mathnozzle, matherial mathtz,
               fuel fuel, double Dmid, double mfuel, double Pk, double Pa)://pk/pa~40, pk,pa в мегапаскалях
    matShell{mathShell},matnozzle{mathnozzle},matbr{mathbr},matTz{mathtz},fl{fuel},d{Dmid}
{
    constexpr double beth=18*M_PI/180;//угол полураствора
    constexpr double fi=0.3;//коэффициент степени утопленности сопла
    const double atz=7E-7;//коэфф. теплопроводности тзп

    params.mfuel=mfuel;
    params.PprivUdSt=fuel.PudST*(1-0.01*(4.3+0.17*fl.ALpercent+0.009*fl.ALpercent*fl.ALpercent));
    params.PpUd=params.PprivUdSt+190.3+76*Pk-3.058*Pk*Pk-7000*Pa+25484*Pa*Pa;
    params.Tk=fl.Tst+11.42*(Pk-3.923);


    params.PpUdVoid=params.PpUd+fl.Rst*params.Tk*pow(Pa/Pk,(fl.kst-1)/fl.kst)/params.PpUd;//удельный импульс тяги в пустоте
    params.Dfuel=0.99*Dmid;
    params.Dv=0.2*params.Dfuel;
    double e0=0.5*(params.Dfuel-params.Dv);//толщина горящего свода при старте

    double t=e0/fl.U(Pk);//время работы двигателя 0.4D/u,

    params.masspsec=0.98*mfuel/t;

    params.Pvacuum=params.masspsec*params.PpUdVoid;//потребная тяга в пустоте(H)


    double a=pow(2/(fl.kst+1),0.5*(fl.kst+1)/(fl.kst-1))*sqrt(fl.kst);//термодинамический комплекс
    double Fcr=params.masspsec*sqrt(0.98*fl.Rst*params.Tk)/(0.99*a*Pk*1E6);
    params.Dcr=2*sqrt(Fcr/M_PI);//диаметр критического сечения сопла

    double fa=pow(2/(fl.kst+1),1/(fl.kst-1))* //степень геометрического расширения сопла
            sqrt((fl.kst-1)/(fl.kst+1))/
            (
                pow(Pa/Pk,1/fl.kst)*
                sqrt(
                    1-pow(Pa/Pk,
                          (fl.kst-1)/fl.kst
                          )
                    )
                );

    double Fa=fa*Fcr;//площадь выходного сечения сопла
    params.Da=2*sqrt(Fa/M_PI);//диаметр выходного сечения сопла



    params.La=0.5*(params.Da-params.Dcr)/tan(beth);//полная длина сопла
    params.Ly=fi*params.La;//длина утопленной части сопла
    params.Lc=params.La-params.Ly;//длина выступающей части сопла
    params.Dc=params.Dcr*(1.5+fi*((params.Da/params.Dcr)-1.5));//диаметр сопла в месте выхода из камеры сгорания
    params.LforwardBottom=0.3*Dmid+0.05*params.Dv;//длины переднего и заднего днищ
    params.LbackwardBottom=0.3*Dmid+0.05*params.Dc;


    double eotn=e0/params.Dfuel;
    double nuc=0.96*4*eotn*(1-eotn);
    double nudn=0.73*(1-1.5*(1-4*eotn*(1-eotn)));
    params.Lcyl=(4*mfuel/(M_PI*Dmid*Dmid*fl.roT) -2*(params.LforwardBottom+params.LbackwardBottom)*nudn/3)/nuc;
    params.Leng=params.Lcyl+params.LforwardBottom+params.LbackwardBottom+params.Lc;//полная длина двигателя

    double lambdafuel=(params.LforwardBottom+params.LbackwardBottom+params.Lcyl)/params.Dfuel;//удлинение заряда




    const double f=1.15;//коэффициент безопасности

    params.mbottoms=matShell.Ro*f*Pk*pow(Dmid,3)/matShell.SigmaV;
    params.mcyl=0.5*M_PI*lambdafuel*params.mbottoms;
    const double abr=0.07E-3;//коэффициент бронирующего покрытия
    const double ks=2.7;//коэффициент формы заряда и канала
    const double ac=0.006;//коэффициент пропорциональности средней толщины стенки сопла к диаметру камеры сгорания

    params.mbr=0.5*M_PI*matbr.Ro*pow(Dmid,3)*(0.5*abr*(1-pow(params.Dv/Dmid,2))/(2*fl.U(Pk))+lambdafuel*(1-params.Dfuel/Dmid));
    params.mnozzle=ks*fl.U(Pk)*fl.roT*0.45*(matnozzle.Ro+mathtz.Ro)*
                                   sqrt(fl.Rst*params.Tk)*ac*(fa-1)*pow(Dmid,3)*lambdafuel/
                                                                                   (a*Pk*1E6*sin(beth));

    double Tcam0{293},TcamMax{473};
    double deltaTZ=0.8*sqrt(atz*Dmid*(1-params.Dv/Dmid)/(2*fl.U(Pk)*(0.1-0.8*log10((params.Tk-TcamMax)/(params.Tk-Tcam0)))));

    params.mtz=deltaTZ*Dmid*Dmid*matTz.Ro*(2+M_PI*(0.37*lambdafuel-0.3));
    params.muk=0.2*params.mbottoms*sqrt(1.2*Pk);
    params.mengine=params.mcyl+params.mbottoms+params.mbr+params.mnozzle+params.mtz+params.muk;

    params.Xnozzle=params.Leng-params.La*2/3;
    params.Xmfuel=params.LforwardBottom+params.Lcyl/2;
    params.Xend=params.LforwardBottom+params.Lcyl+params.LbackwardBottom/3;

    params.Xengine=(params.Xnozzle*params.mnozzle+params.Xmfuel*(params.mcyl+params.mbottoms+params.mbr+params.mtz+params.muk+params.mfuel))/
            (params.mengine+params.mfuel);
    params.Xemptyengine=(params.Xnozzle*params.mnozzle+params.Xmfuel*(params.mcyl+params.mbottoms+params.mbr+params.mtz+params.muk))/
            params.mengine;
 //   std::cout<<params.Xend<<" "<<params.mengine<<" "<<params.Xengine<<" "<<params.Xemptyengine<<std::endl;
}

double fuel::U(double pk){//in millimeter
    return 1E-3*Ukoef*pow(pk,Upow);
}
