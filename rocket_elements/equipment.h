#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include "abstractrocketelement.h"

class equipment:public abstractRocketElement{
protected:
    std::string name;
    double eqmass=0;
    double Xm=0;
public:
    virtual double Smid()const override;//S площадь миделя элемента

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
    virtual ~equipment()override{}
    equipment(){}
    equipment(std::string eqname, double X, double mass);
};

std::ostream& operator<<(std::ostream& os, const equipment& eq);
std::istream& operator>>(std::istream& in, equipment& eq);




#endif // EQUIPMENT_H
