#ifndef ABSTRACTROCKETELEMENT_H
#define ABSTRACTROCKETELEMENT_H
#include <string>
#include <iostream>
const std::string materialheader{"material"};

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

struct material{
    std::string name="air";
    //matherial parameters
    double Ro=1;//плотность
    double SigmaV=0;//предел прочности
    material(){}
    material(const std::string& matherialName, double Ro, double sigma):name{matherialName},Ro{Ro},SigmaV{sigma}{}
    material(const material&mat){
        name=mat.name;
        Ro=mat.Ro;
        SigmaV=mat.SigmaV;
    }
    bool operator==(const material& second)const{
        return name==second.name&&
                Ro==second.Ro&&
                SigmaV==second.SigmaV;
    }
};
std::ostream& operator<<(std::ostream& os, const material&mat);
std::istream& operator>>(std::istream& in, material&mat);


#endif // ABSTRACTROCKETELEMENT_H
