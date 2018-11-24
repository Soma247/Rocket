#ifndef ABSTRACTROCKETELEMENT_H
#define ABSTRACTROCKETELEMENT_H
#include <string>

class abstractRocketElement{
public:
    virtual double S()const=0;//S площадь миделя элемента
    virtual double mass()const=0;
    virtual double massCenter()const=0;

    virtual double getX0()const=0;
    virtual void setX0(double x)=0;

    virtual double getL()const=0;
    virtual void setL(double L)=0;

    virtual std::string getname()const=0;
    virtual void setname(std::string n)=0;

    virtual std::string toString()const;
    virtual ~abstractRocketElement(){}
};


struct matherial{
    std::string name="air";
    //matherial parameters
    double Ro=1;//плотность
    double SigmaV=0;//предел прочности
    matherial(){}
    matherial(const std::string& matherialName, double Ro, double sigma):name{matherialName},Ro{Ro},SigmaV{sigma}{}
    matherial(const matherial&mat){
        name=mat.name;
        Ro=mat.Ro;
        SigmaV=mat.SigmaV;
    }
};


#endif // ABSTRACTROCKETELEMENT_H
