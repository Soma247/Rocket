#include <iostream>
#include <math.h>
#include "nosecone.h"
#include "aerodynamics/aerodynamics.h"
double nosecone::S() const{
    return 0.25*M_PI*dend*dend;
}

double nosecone::mass() const{
    return mat.Ro*(S()-0.25*M_PI*pow(dend-2*delt,2))*getL()/3.0;
}

double nosecone::massCenter() const{
    return 3*getL()/4.0;
}

double nosecone::getL() const{
    return len;
}

double nosecone::getCp(double Dmid, double M) const{
    return Aerodynamics::CxpNoseCone(len,dend,Dmid,M);
}

double nosecone::getDend() const{
    return dend;
}

std::string nosecone::getname() const{
    return name;
}

std::string nosecone::toString() const{
    return "nosecone";
}

void nosecone::setdelt(double d){
    delt=d;
}
void nosecone::setDend(double d){
    dend=d;
}

nosecone::nosecone(matherial math, double Dend, double length, double delta):
    mat{math},dend{Dend},len{length},delt{delta}
{
}



