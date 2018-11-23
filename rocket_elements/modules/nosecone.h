#ifndef NOSECONE_H
#define NOSECONE_H
#include <rocket_elements/rocketmodule.h>

class nosecone:public Module{
public:
    virtual double S()const override;
    virtual double mass()const override;
    virtual double massCenter()const override;

    virtual double getX0()const override;
    virtual void setX0(double x)override{x0=x;}

    virtual double getL()const override;
    virtual void setL(double l)override{len=l;}

    virtual double getCp(double Dmid, double M)const override;

    virtual double getDend()const override;
    virtual void setDend(double d)override;

    virtual std::string getname()const override;
    virtual void setname(std::string n)override{name=n;}

    virtual std::string toString()const override;

    virtual double getdelt()const{return delt;}
    virtual void setdelt(double d);


    virtual ~nosecone()override{}
    nosecone(){}
    nosecone(matherial math, double Dend, double length, double delta);
private:
    matherial mat;
    double dend=0;
    double len=0;
    double delt=0;
    double x0=0;
    std::string name;
};


#endif // NOSECONE_H
