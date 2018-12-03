#include "engine.h"
#include <iostream>
#include <math.h>
const std::string engineheader {"HFengine"};
double engine::S() const{return 0.25*M_PI*params.engineDiameter*params.engineDiameter;}
double engine::mass() const{return params.mengine+params.mfuel;}
double engine::massCenter() const{return params.Xengine;}
double engine::getCp(double, double) const{return 0;}//
double engine::getX0() const{return X0;}
void engine::setX0(double x){X0=x;}
double engine::getL() const{return params.Leng;}
void engine::setL(double){}//
double engine::getDend() const{return params.engineDiameter;}
void engine::setDend(double){}
std::string engine::getname() const{return "";}
void engine::setname(std::string n){name=n;}
std::string engine::toString() const{return "";}
double engine::getdelt() const{return params.delt;}
void engine::setdelt(double){}

engineparameters engine::getparams() const{return params;}//

engine::engine(matherial mathShell, matherial mathbr, matherial mathnozzle, matherial mathtz,
               fuel fuel, double Dmid, double mfuel, double Pk, double Pa){//pk/pa~40, pk,pa в мегапаскалях
    constexpr double beth=18*M_PI/180;//угол полураствора
    constexpr double fi=0.3;//коэффициент степени утопленности сопла
    const double atz=7E-7;//коэфф. теплопроводности тзп

    params.matShell=mathShell;
    params.matnozzle=mathnozzle;
    params.matbr=mathbr;
    params.matTz=mathtz;
    params.fl=fuel;
    params.engineDiameter=Dmid;
    params.Pk=Pk;
    params.Pa=Pa;
    params.mfuel=mfuel;
    params.PprivUdSt=fuel.Prelst*(1-0.01*(4.3+0.17*params.fl.AL+0.009*params.fl.AL*params.fl.AL));
    params.PpUd=params.PprivUdSt+190.3+76*Pk-3.058*Pk*Pk-7000*Pa+25484*Pa*Pa;
    params.Tk=params.fl.tst+11.42*(Pk-3.923);
    params.PpUdVoid=params.PpUd+params.fl.rst*params.Tk*pow(Pa/Pk,(params.fl.kst-1)/params.fl.kst)/params.PpUd;
    //удельный импульс тяги в пустоте

    params.Dfuel=0.99*Dmid;
    params.Dv=0.32*params.Dfuel;
    params.Dcyl=Dmid;
    double e0=0.5*(params.Dfuel-params.Dv);//толщина горящего свода при стартe
    params.t=e0/params.fl.U(Pk);//время работы двигателя 0.4D/u,
    params.masspsec=0.98*mfuel/params.t;
    params.Pvacuum=params.masspsec*params.PpUdVoid;//потребная тяга в пустоте(H)
    double a=pow(2/(params.fl.kst+1),0.5*(params.fl.kst+1)/(params.fl.kst-1))*sqrt(params.fl.kst);//термодинамический комплекс
    double Fcr=params.masspsec*sqrt(0.98*params.fl.rst*params.Tk)/(0.99*a*Pk*1E6);
    params.Dcr=2*sqrt(Fcr/M_PI);//диаметр критического сечения сопла
    double fa=pow(2/(params.fl.kst+1),1/(params.fl.kst-1))* //степень геометрического расширения сопла
            sqrt((params.fl.kst-1)/(params.fl.kst+1))/
            (
                pow(Pa/Pk,1/params.fl.kst)*
                sqrt(
                    1-pow(Pa/Pk,
                          (params.fl.kst-1)/params.fl.kst
                          )
                    )
                );
    double Fa=fa*Fcr;//площадь выходного сечения сопла
    params.Da=2*sqrt(Fa/M_PI);//диаметр выходного сечения сопла
    if(params.Da>=Dmid)params.Da=Dmid-0.01;
    params.La=0.5*(params.Da-params.Dcr)/tan(beth);//полная длина сопла
    params.Ly=fi*params.La;//длина утопленной части сопла
    params.Lc=params.La-params.Ly;//длина выступающей части сопла
    params.Dc=params.Dcr*(1.5+fi*((params.Da/params.Dcr)-1.5));//диаметр сопла в месте выхода из камеры сгорания
    params.LforwardBottom=0.3*Dmid+0.05*params.Dv;//длины переднего и заднего днищ
    params.LbackwardBottom=0.3*Dmid+0.05*params.Dc;
    double eotn=e0/params.Dfuel;
    double nuc=0.96*4*eotn*(1-eotn);
    double nudn=0.73*(1-1.5*(1-4*eotn*(1-eotn)));
    params.Lcyl=(4*mfuel/(M_PI*Dmid*Dmid*params.fl.rot) -2*(params.LforwardBottom+params.LbackwardBottom)*nudn/3)/nuc;
    params.Leng=params.Lcyl+params.LforwardBottom+params.LbackwardBottom+params.Lc;//полная длина двигателя
    double lambdafuel=(params.LforwardBottom+params.LbackwardBottom+params.Lcyl)/params.Dfuel;//удлинение заряда
    const double f=1.15;//коэффициент безопасности
    params.mbottoms=params.matShell.Ro*f*Pk*pow(Dmid,3)/params.matShell.SigmaV;
    params.mcyl=0.5*M_PI*lambdafuel*params.mbottoms;
    const double abr=0.07E-3;//коэффициент бронирующего покрытия
    const double ks=2.7;//коэффициент формы заряда и канала
    const double ac=0.006;//коэффициент пропорциональности средней толщины стенки сопла к диаметру камеры сгорания
    params.mbr=0.5*M_PI*params.matbr.Ro*pow(Dmid,3)*(0.5*abr*(1-pow(params.Dv/Dmid,2))/(2*params.fl.U(Pk))+lambdafuel*(1-params.Dfuel/Dmid));
    params.mnozzle=ks*params.fl.U(Pk)*params.fl.rot*0.45*(params.matnozzle.Ro+mathtz.Ro)*
                                   sqrt(params.fl.rst*params.Tk)*ac*(fa-1)*pow(Dmid,3)*lambdafuel/
                                                                                   (a*Pk*1E6*sin(beth));
    double Tcam0{293},TcamMax{473};
    double deltaTZ=0.8*sqrt(atz*Dmid*(1-params.Dv/Dmid)/(2*params.fl.U(Pk)*(0.1-0.8*log10((params.Tk-TcamMax)/(params.Tk-Tcam0)))));
    params.mtz=deltaTZ*Dmid*Dmid*params.matTz.Ro*(2+M_PI*(0.37*lambdafuel-0.3));
    params.muk=0.2*params.mbottoms*sqrt(1.2*Pk);
    params.mengine=params.mcyl+params.mbottoms+params.mbr+params.mnozzle+params.mtz+params.muk;
    params.Xnozzle=params.Leng-params.La*2/3;
    params.Xmfuel=params.LforwardBottom+params.Lcyl/2;
    params.Xend=params.LforwardBottom+params.Lcyl+params.LbackwardBottom/3;
    params.Xengine=(params.Xnozzle*params.mnozzle+
                    params.Xmfuel*(params.mcyl+params.mbottoms+params.mbr+params.mtz+params.muk+params.mfuel))/
                                                                                  (params.mengine+params.mfuel);
    params.Xemptyengine=(params.Xnozzle*params.mnozzle+params.Xmfuel*
                         (params.mcyl+params.mbottoms+params.mbr+params.mtz+params.muk))/
                                                                              params.mengine;
}

double fuel::U(double pk){//in millimeter
    return 1E-3*ukoef*pow(pk,upow);
}

std::ostream &operator<<(std::ostream &os, const engine &en){
    engineparameters par=en.getparams();
    return os<<engineheader<<'{'
            <<par.matShell<<','
               <<par.matbr<<','
           <<par.matnozzle<<','
               <<par.matTz<<','
                  <<par.fl<<','
                <<par.Dcyl<<','
               <<par.mfuel<<','
                  <<par.Pk<<','
                  <<par.Pa<<'}';
}

std::istream &operator>>(std::istream &in, engine &en){
    std::string header;
    matherial mathShell, mathbr, mathnozzle, mathtz;
    fuel fl;
    double Dmid{0},mfuel{0},Pk{0},Pa{0};
    char delim1{0},delim2{0},delim3{0},delim4{0},delim5{0},delim6{0},delim7{0},delim8{0},footer{0};
    int tmp{0};

    while((tmp=in.get())!=EOF && isspace(tmp));
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=engineheader){
        in.clear(std::ios::failbit);
        return in;
    }
    in>>mathShell>>delim1
            >>mathbr>>delim2
        >>mathnozzle>>delim3
            >>mathtz>>delim4
                >>fl>>delim5
              >>Dmid>>delim6
             >>mfuel>>delim7
                >>Pk>>delim8
                >>Pa>>footer;
    if(!in)return in;
    if(delim1!=delim2 ||
            delim2!=delim3||
            delim3!=delim4||
            delim4!=delim5||
            delim5!=delim6||
            delim6!=delim7||
            delim7!=delim8||
            delim8!=',' ||
            footer!='}'||
            Dmid<0 ||
            mfuel<0 ||
            Pk<0 || Pa<0){
        in.clear(std::ios::failbit);
        return in;
    }
    en =engine(mathShell, mathbr, mathnozzle, mathtz,fl,Dmid,mfuel, Pk, Pa);
    return in;
}
