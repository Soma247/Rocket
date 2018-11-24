#ifndef PLANE_H
#define PLANE_H
#include "rocket_elements/abstractrocketelement.h"

class plane:public abstractRocketElement{
    virtual double S()const override;//S площадь миделя элемента

    virtual double mass()const override;
    virtual void setmass(double m);

    virtual double massCenter()const override;
    virtual void setmassCenter(double xm);

    virtual double getX0()const override{return 0;}//заглушка
    virtual void setX0(double)override{}//заглушка

    virtual double getL()const override{return 0;}//заглушка
    virtual void setL(double)override{}//заглушка

    virtual std::string getname()const override;
    virtual void setname(std::string n)override;

    virtual std::string toString()const override;
    virtual ~plane()override{}
    plane(){}
    plane(double X, double mass, std::string eqname);
protected:
    double x0;
    double x05;
    double hordroot;
    double hordtip;
    double croot;
    double ctip;
    double xcfr;//forward root
    double xcbr;
    double xcft;
    double xcbt;//back tip
    matherial mat;
    double eqmass=0;
    double Xm=0;
    std::string name;
    double N;
    double turnAngle;
};

#endif // PLANE_H
