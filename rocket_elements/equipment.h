#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include "abstractrocketelement.h"

class equipment:public abstractRocketElement{
public:
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
    virtual ~equipment()override{}
    equipment(){}
    equipment(double X, double mass, std::string eqname);
protected:
    double eqmass=0;
    double Xm=0;
    std::string name;
};


#endif // EQUIPMENT_H
