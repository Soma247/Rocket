#ifndef PLANE_H
#define PLANE_H
#include "rocket_elements/abstractrocketelement.h"
struct planeparams{
    material mat;
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
    double Kaa=0;
    double S(){return 0.5*(broot+btip)*h;}
};

class plane:public abstractRocketElement{
protected:
    planeparams params;
    std::string name;
    bool tailstab=false;

public:
    virtual double Smid()const override;//S площадь миделя всех элементов

    virtual double mass()const override;
    virtual void setmass(double m);

    virtual double massCenter()const override;
    virtual void setmassCenter(double){}//

    virtual double getX0()const override;
    virtual void setX0(double x0)override;

    virtual double getL()const override;//заглушка
    virtual void setL(double)override{}//заглушка

    virtual std::string getname()const override;
    virtual void setname(std::string n)override;

    virtual std::string toString()const override;

    virtual double getCp(double SmidLA, double M)const;

    double getXp(double M)const;
    virtual double getCxtr(double SmidLA,double M,double sound_sp, double cin_visc)const;
    virtual double getcyaConsole(double Dmid, double SmidLA, double M,double sound_sp, double cin_visc)const;
    virtual ~plane()override{}

    virtual planeparams getparams()const;
    plane(){}
    plane(material math,
          double Xfromnose,
          double Broot, double Btip,
          double Croot, double Ctip,
          double Xtip, double Xrf,
          double Xrr, double Xtf,
          double Xtr, double H,double N=4
    );
};


std::ostream &operator<<(std::ostream &os, const plane &pl);
std::istream &operator>>(std::istream &in, plane &pl);


#endif // PLANE_H
