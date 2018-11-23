#include "equipment.h"
equipment::equipment(double X, double mass, std::string eqname):
    eqmass{mass},Xm{X},name{eqname}
{
}
double equipment::S() const{return 0;}

double equipment::mass() const{return eqmass;}

void equipment::setmass(double m){eqmass=m;}

double equipment::massCenter() const{return Xm;}

void equipment::setmassCenter(double xm){Xm=xm;}

std::string equipment::getname() const{return name;}

void equipment::setname(std::string n){name=n;}

std::string equipment::toString() const{return "equipment";}
