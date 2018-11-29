#ifndef ENGINE_H
#define ENGINE_H
#include "rocket_elements/rocketmodule.h"

const std::string engineheader{"HFengine"};
const std::string fuelheader{"Hfuel"};
struct fuel{
    std::string name;
    double AL=0;//% в топливе
    double Prelst=0;
    double rot=0;
    double tst=0;
    double rst=0;
    double kst=0;
    double ukoef=0;
    double upow=0;
    double U(double pk);
    bool iscorrect(){return AL>=0 && Prelst>=0 &&
                              rot>=0 && tst>=0 &&
                               rst>=0 && kst>=0 &&
                                 ukoef>=0 && upow>=0;
    }
    fuel(){}
    fuel(std::string modulename, double Alper, double PrelativeST,double RoT, double Tst, double Rst, double Kst, double Ukoef, double Upow):
        name{modulename},AL{Alper},Prelst{PrelativeST},rot{RoT},tst{Tst},rst{Rst},kst{Kst},ukoef{Ukoef},upow{Upow}
    {
    }
};
    std::ostream &operator<<(std::ostream &os, const fuel &fl);

    std::istream &operator>>(std::istream &in, fuel& fl);



struct engineparameters{
    matherial matShell;
    matherial matnozzle;
    matherial matbr;
    matherial matTz;
    fuel fl;
    double Pk=0;
    double Pa=0;
    double PprivUdSt=0;
    double PpUd=0;
    double Tk=0;
    double PpUdVoid=0;//удельный импульс тяги в пустоте
    double Dfuel=0;
    double Dv=0;
    double t=0;//время работы двигателя 0.4D/u,
    double masspsec=0;
    double Pvacuum=0;//потребная тяга в пустоте
    double Dcyl=0;
    double Dcr=0;//диаметр критического сечения сопла
    double Da=0;//диаметр выходного сечения сопла
    double La=0;//полная длина сопла
    double Ly=0;//длина утопленной части сопла
    double Lc=0;//длина выступающей части сопла
    double Dc=0;//диаметр сопла в месте выхода из камеры сгорания
    double LforwardBottom=0;//длины переднего и заднего днищ
    double LbackwardBottom=0;
    double Lcyl=0;
    double Leng=0;//полная длина двигателя
    double mfuel=0;
    double mbottoms=0;
    double mcyl=0;
    double mbr=0;
    double mnozzle=0;
    double mtz=0;
    double muk=0;
    double mengine=0;
    double Xnozzle=0;
    double Xmfuel=0;
    double Xengine=0;
    double Xemptyengine=0;
    double Xend=0;
    double engineDiameter=0;
    double len=0;
    double delt=0;
};



class engine:public Module{
public:
    virtual double getX0()const override;
    virtual void setX0(double x)override;

    virtual double getL()const override;
    virtual void setL(double)override;

    virtual double getDend()const override;
    virtual void setDend(double)override;

    virtual std::string getname()const override;
    virtual void setname(std::string n)override;

    virtual double S()const override;
    virtual double mass()const override;
    virtual double massCenter()const override;
    virtual double getCp(double Dmid, double M)const override;
    virtual std::string toString()const override;

    virtual ~engine()override{}

    virtual double getdelt()const;
    virtual void setdelt(double d);

    virtual double getDbegin()const{return params.engineDiameter;}
    virtual void setDbegin(double){}//
    virtual double getmassend()const{
        return mass()-params.mfuel-params.mtz/2.0;
    }
    virtual double getmasscenterend(){
        return (massCenter()*(mass()-params.mtz)-params.mfuel*params.Xmfuel)/getmassend();
    }

    virtual engineparameters getparams()const{return params;}

    engine(){}
    engine(matherial mathShell, matherial mathbr,
           matherial mathnozzle, matherial mathtz, fuel fuel,
           double Dmid, double mfuel, double Pk, double Pa);

private:
    engineparameters params;
    double X0=0;
    std::string name;
};

/*
std::ostream &operator<<(std::ostream &os, const engine &en){
    engineparameters par=en.getparams();
    return os<<engineheader<<'{'<<par.matShell<<','<<par.matbr<<','<<par.matnozzle
            <<','<<par.matTz<<','<<par.fl<<par.Dcyl<<','<<par.mfuel<<','<<par.Pk<<','<<par.Pa<<'}';
}

std::istream &operator>>(std::istream &in, conoid &cone){
    std::string header;
    matherial mat;
    char delim1{0},delim2{0},delim3{0},delim4{0},delim5{0},footer{0};
    int tmp{0};
    double dbeg{0},dend{0},len{0},delt{0},x0{0};

    while((tmp=in.get())!=EOF && isspace(tmp));
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=coneheader){
        in.clear(std::ios::failbit);
        return in;
    }
    in>>mat>>delim1>>dbeg>>delim2>>dend>>delim3>>len>>delim4>>delt>>delim5>>x0>>footer;
    if(!in)return in;
    if(delim1!=delim2 ||
            delim2!=delim3||
            delim3!=delim4||
            delim4!=delim5||
              delim5!=',' ||
               footer!='}'||
                   dbeg<0 ||
                   dend<0 ||
                    len<0 || x0<0){
        in.clear(std::ios::failbit);
        return in;
    }
    cone=conoid(mat,dbeg,dend,len,delt);
    cone.setX0(x0);
    return in;
}








*/


#endif // ENGINE_H
