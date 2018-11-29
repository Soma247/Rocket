#ifndef CONOID_H
#define CONOID_H
#include "rocket_elements/rocketmodule.h"

class conoid:public Module{
public:
    virtual double S()const override;
    virtual double mass()const override;
    virtual double massCenter()const override;
    virtual double getCp(double Dmid, double M)const override;

    virtual double getX0()const override;
    virtual void setX0(double x)override;

    virtual double getL()const override;
    virtual void setL(double length)override;

    virtual double getDend()const override;
    virtual void setDend(double d)override;

    virtual std::string getname()const override;
    virtual void setname(std::string n)override;

    virtual std::string toString()const override;

    virtual double getdelt()const;
    virtual void setdelt(double d);

    virtual double getDbegin()const;
    virtual void setDbegin(double dbeg);

    virtual ~conoid()override{}
    virtual coneparam getparams()const{
        return coneparam{mat,dbegin,dend,len,delt};
    }
    conoid(){}
    conoid(matherial math, double Dbegin, double Dend, double length, double delta);
private:
    matherial mat;
    double dbegin=0;
    double dend=0;
    double len=0;
    double delt=0;
    double X0=0;
    std::string name;
};

std::ostream& operator<<(std::ostream& os, const conoid& cone);
std::istream& operator>>(std::istream& in, conoid& cone);









#endif // CONOID_H
