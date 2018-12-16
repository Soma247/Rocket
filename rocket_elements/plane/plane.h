#ifndef PLANE_H
#define PLANE_H
#include "rocket_elements/abstractrocketelement.h"
struct planeparams{
    matherial mat;
    double XfromNose=0;
    double broot=0;
    double btip=0;
    double croot=0;
    double ctip=0;
    double xrf=0;//forward root
    double xtip=0;
    double xrr=0;//rear root
    double xtf=0;
    double xtr=0;//back tip
    double h=0;//длина одного пера
    double angle0=0;
    double angleCmax=0;
    double massn=0;
    double n=4;
    double Kaa;
};

class plane:public abstractRocketElement{
public:
    virtual double S()const override{return 0.5*(params.croot+params.ctip)*params.h*params.n;}//S площадь миделя всех элементов

    virtual double mass()const override{return  params.massn;}
    virtual void setmass(double m){params.massn=m;}

    virtual double massCenter()const override;
    virtual void setmassCenter(double){}//

    virtual double getX0()const override{return params.XfromNose;}
    virtual void setX0(double x0)override{params.XfromNose=x0;}

    virtual double getL()const override{return params.broot;}//заглушка
    virtual void setL(double)override{}//заглушка

    virtual std::string getname()const override{return  name;}
    virtual void setname(std::string n)override{name=n;}

    virtual std::string toString()const override{return "plane";}

    virtual double getCp(double SmidLA, double M)const;

    double getXp(double M)const{
        double k=M>1?0.45:0.35;
        return (0.5*(params.broot+params.btip)*k)+params.XfromNose;
    }
    virtual double getCxtr(double SmidLA,double M,double sound_sp, double cin_visc)const;
    virtual double getcyaConsole(double Dmid, double SmidLA, double M,double sound_sp, double cin_visc)const;
    virtual ~plane()override{}

    virtual planeparams getparams()const{
        return params;
    }
    plane(){}
    plane(matherial math,
          double Xfromnose,
          double Broot, double Btip,
          double Croot, double Ctip,
          double Xtip, double Xrf,
          double Xrr, double Xtf,
          double Xtr, double H,double N=4
    );

protected:
    planeparams params;
    std::string name;
    bool tailstab=false;
};


std::ostream &operator<<(std::ostream &os, const plane &pl);

std::istream &operator>>(std::istream &in, plane &pl);


#endif // PLANE_H
