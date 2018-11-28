#ifndef ENGINE_H
#define ENGINE_H
#include "rocket_elements/rocketmodule.h"
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
    fuel(){}
    fuel(std::string modulename, double Alper, double PrelativeST,double RoT, double Tst, double Rst, double Kst, double Ukoef, double Upow):
        name{modulename},AL{Alper},Prelst{PrelativeST},rot{RoT},tst{Tst},rst{Rst},kst{Kst},ukoef{Ukoef},upow{Upow}
    {
    }
};
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












#endif // ENGINE_H
