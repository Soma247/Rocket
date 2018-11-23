#include "rocketmodule.h"
#include <iostream>
double nosecone::S() const{
    return 0.25*M_PI*dend*dend;
}

double nosecone::mass() const{
    return mat.Ro*(S()-0.25*M_PI*pow(dend-2*delt,2))*getL()/3.0;
}

double nosecone::massCenter() const{
    return 2*getL()/3.0;
}

double nosecone::getX0() const{
    return 0;
}

double nosecone::getL() const{
    return len;
}

double nosecone::getCp(double Dmid, double M) const{
    return 0;
}

double nosecone::getDend() const{
    return dend;
}

std::string nosecone::getname() const{
    return name;
}

std::string nosecone::toString() const{
    return "nosecone";
}

void nosecone::setdelt(double d){
    delt=d;
}

nosecone::nosecone(matherial math, double Dend, double length, double delta):
    mat{math},dend{Dend},len{length},delt{delta}
{
}

void nosecone::setDend(double d){
    dend=d;
}





double equipment::massCenter() const{return Xm;}

equipment::equipment(double X, double mass, std::string eqname):
    eqmass{mass},Xm{X},name{eqname}
{
}

std::string equipment::toString() const{return "equipment";}

std::string abstractRocketElement::toString()const{return "abstractRocketElement";}

std::string Module::toString() const{
    return "Module";
}


engine::engine(matherial mathShell, matherial mathbr, matherial mathnozzle, matherial mathtz,
               fuel fuel, double Dmid, double mfuel, double Pk, double Pa)://pk/pa~40
    matShell{mathShell},matnozzle{mathnozzle},matbr{mathbr},matTz{mathtz},fl{fuel},d{Dmid}
{
    constexpr double beth=0.26;//угол полураствора
    constexpr double fi=0.15;//коэффициент степени утопленности сопла

    params.PprivUdSt=fuel.PudST*(1-0.01*(4.3+0.17*fl.ALpercent+0.009*fl.ALpercent*fl.ALpercent));
    params.PpUd=params.PprivUdSt+190.3+76*Pk-3.058*Pk*Pk-7000*Pa+25484*Pa*Pa;
    params.Tk=fl.Tst+11.42*(Pk-3.923);
    params.PpUdVoid=params.PpUd+fl.Rst*params.Tk*pow(Pa/Pk,(fl.kst-1)/fl.kst)/params.PpUd;//удельный импульс тяги в пустоте
    params.Dfuel=int(0.99*Dmid);
    params.Dv=int(0.2*params.Dfuel);
    double e0=0.5*(params.Dfuel-params.Dv);//толщина горящего свода при старте

    double t=e0/fl.U(Pk);//время работы двигателя 0.4D/u,
    params.masspsec=0.98*mfuel/t;
    params.Pvacuum=params.masspsec*params.PpUdVoid;//потребная тяга в пустоте

    double a=pow(2/(fl.kst+1),0.5*(fl.kst+1)/(fl.kst-1))*sqrt(fl.kst);//термодинамический комплекс
    double Fcr=params.masspsec*sqrt(0.98*fl.Rst*params.Tk)/(0.99*a*Pk);
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
    params.Leng=params.Lcyl+params.LforwardBottom+params.LbackwardBottom+params.Lc+params.Ly;//полная длина двигателя
    double lambdafuel=(params.LforwardBottom+params.LbackwardBottom+params.Lcyl)/params.Dfuel;//удлинение заряда



    const double f=1.15;//коэффициент безопасности

    params.mbottoms=matShell.Ro*f*Pk*pow(Dmid,3)/matShell.SigmaV;
    params.mcyl=0.5*M_PI*lambdafuel*params.mbottoms;
    const double abr=0.05;//коэффициент бронирующего покрытия
    const double ks=3;//коэффициент формы заряда и канала
    const double ac=0.005;//коэффициент пропорциональности средней толщины стенки сопла к диаметру камеры сгорания
    params.mbr=0.5*M_PI*matbr.Ro*pow(Dmid,3)*(0.5*abr*(1-params.Dv*params.Dv/(Dmid*Dmid))+lambdafuel*(1-params.Dfuel/Dmid));
    params.mnozzle=ks*fl.U(Pk)*fl.roT* 0.8*matnozzle.Ro* sqrt(fl.Rst*params.Tk)*ac*(fa-1)*lambdafuel/(a*Pk*sin(beth));
    double Tcam0{293},TcamMax{473};
    const double atz=0.7E-6;//коэфф. теплопроводности тзп
    double deltaTZ=0.8*sqrt(atz*Dmid*(1-params.Dv/Dmid)/(2*fl.U(Pk)*(0.1-0.8*log10((params.Tk-TcamMax)/(params.Tk-Tcam0)))));
    params.mtz=deltaTZ*Dmid*Dmid*matTz.Ro*(2+M_PI*(0.37*lambdafuel-0.3));
    params.muk=0.2*params.mbottoms*sqrt(1.2*Pk);
    params.mengine=params.mcyl+params.mbottoms+params.mbr+params.mnozzle+params.mtz+params.muk;


    std::cout<<"l="<<params.Leng<<" m="<<params.mengine<<std::endl;

}
