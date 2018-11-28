#ifndef ROCKETMODULE_H
#define ROCKETMODULE_H
#include"abstractrocketelement.h"
struct coneparam{
    matherial mat;
    double dbeg=0;
    double dend=0;
    double len=0;
    double delt=0;
};

class Module:public abstractRocketElement{
public:
    virtual double S()const override=0;//S площадь миделя элемента
    virtual double mass()const override=0;
    virtual double massCenter()const override=0;
    virtual double getX0()const override=0;
    virtual void setX0(double x)override=0;
    virtual double getL()const override=0;
    virtual void setL(double d)override=0;
    virtual std::string getname()const override=0;
    virtual void setname(std::string n)override=0;
    virtual std::string toString()const override;

    virtual double getCp(double Dmid, double M)const=0;
    virtual double getDend()const=0;
    virtual void setDend(double d)=0;
    virtual ~Module()override{}
};
#endif // ROCKETMODULE_H
