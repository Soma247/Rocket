#include "equipment.h"
equipment::equipment(std::string eqname, double X, double mass):
    name{eqname},eqmass{mass},Xm{X}
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
