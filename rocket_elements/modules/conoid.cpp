#include "conoid.h"
#include <math.h>

double conoid::S() const{
    return 0.25*M_PI*dend*dend;
}

double conoid::mass() const{
    return mat.Ro* 0.25*M_PI*pow(0.5*(dend+dbegin-4*delt),2);
}

double conoid::massCenter() const{

}

double conoid::getCp(double Dmid, double M) const{

}

double conoid::getX0() const{
    return X0;
}

void conoid::setX0(double x){
    X0=x;
}

double conoid::getL() const{
    return len;
}

void conoid::setL(double length){
    len=length;
}

double conoid::getDend() const{
    return dend;
}

void conoid::setDend(double d){
    dend=d;
}

std::string conoid::getname() const{
    return name;
}

void conoid::setname(std::string n){
    name=n;
}

std::string conoid::toString() const{
    return "conoid";
}

double conoid::getdelt() const{
    return delt;
}

void conoid::setdelt(double d){
    delt=d;
}

double conoid::getDbegin() const{
    return dbegin;
}

void conoid::setDbegin(double dbeg){
    dbegin=dbeg;
}

conoid::conoid(matherial math, double Dbegin, double Dend, double length, double delta, double X):
    mat{math},dbegin{Dbegin},dend{Dend},len{length},delt{delta},X0{X}
{
}

