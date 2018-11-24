#ifndef ENGINE_H
#define ENGINE_H
#include "rocket_elements/rocketmodule.h"

struct engineparameters{
    double PprivUdSt=0;
    double PpUd=0;
    double Tk=0;
    double PpUdVoid=0;//удельный импульс тяги в пустоте
    double Dfuel=0;
    double Dv=0;
    double t=0;//время работы двигателя 0.4D/u,
    double masspsec=0;
    double Pvacuum=0;//потребная тяга в пустоте
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
};

struct fuel{
    std::string name;
    double ALpercent;//% в топливе
    double PudST;
    double roT;
    double Tst;
    double Rst;
    double kst;
    double Ukoef;
    double Upow;
    double U(double pk);
};

class engine:public Module{
public:
    virtual double getX0()const override;
    virtual void setX0(double x)override;

    virtual double getL()const override;
    virtual void setL(double length)override;

    virtual double getDend()const override;
    virtual void setDend(double d)override;

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

    virtual double getDbegin()const{return d;}
    virtual void setDbegin(double){}//

    engine(){}
    engine(matherial mathShell, matherial mathbr,
           matherial mathnozzle, matherial mathtz, fuel fuel,
           double Dmid, double mfuel, double Pk, double Pa);

private:
    matherial matShell;
    matherial matnozzle;
    matherial matbr;
    matherial matTz;
    engineparameters params;
    fuel fl;
    double d=0;
    double len=0;
    double delt=0;
    double X0=0;
    std::string name;
};












#endif // ENGINE_H
