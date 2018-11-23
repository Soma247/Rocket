#include "nosecone.h"
#include <iostream>
#include <math.h>
double nosecone::S() const{
    return 0.25*M_PI*dend*dend;
}

double nosecone::mass() const{
    return mat.Ro*(S()-0.25*M_PI*pow(dend-2*delt,2))*getL()/3.0;
}

double nosecone::massCenter() const{
    return 2*getL()/3.0;
}

double nosecone::getX0() const{
    return 0;
}

double nosecone::getL() const{
    return len;
}

double nosecone::getCp(double Dmid, double M) const{
    return 0;
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

nosecone::nosecone(matherial math, double Dend, double length, double delta):
    mat{math},dend{Dend},len{length},delt{delta}
{
}

void nosecone::setDend(double d){
    dend=d;
}

