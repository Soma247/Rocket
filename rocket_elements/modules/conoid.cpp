#include <math.h>
#include "conoid.h"
#include "aerodynamics/aerodynamics.h"
#include <iostream>

const std::string coneheader{"conoid"};

double conoid::Smid() const{
    return 0.25*M_PI*dend*dend;
}

double conoid::mass() const{
    return mat.Ro* 0.25*M_PI*pow(0.5*(dend+dbegin-4*delt),2);
}

double conoid::massCenter() const{
    return 0.25*len*(dend*dend+2*dend*dbegin+3*dend*dend)/(dbegin*dbegin+dend*dend+dbegin*dend);
}

double conoid::getCp(double Dmid, double M) const{
    return Aerodynamics::CxpConoid(len,dbegin,dend,Dmid,M);
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

conoid::conoid(material math, double Dbegin, double Dend, double length, double delta):
    mat{math},dbegin{Dbegin},dend{Dend},len{length},delt{delta}
{
}


std::ostream &operator<<(std::ostream &os, const conoid &cone){
    coneparam par=cone.getparams();
    return os<<coneheader<<'{'<<par.mat<<','<<par.dbeg<<','<<par.dend
            <<','<<par.len<<','<<par.delt<<','<<cone.getX0()<<'}';
}

std::istream &operator>>(std::istream &in, conoid &cone){
    std::string header;
    material mat;
    char delim1{0},delim2{0},delim3{0},delim4{0},delim5{0},footer{0};
    int tmp{0};
    double dbeg{0},dend{0},len{0},delt{0},x0{0};

    while((tmp=in.get())!=EOF && isspace(tmp));
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=coneheader){
        in.clear(std::ios::failbit);
        return in;
    }
    in>>mat>>delim1>>dbeg>>delim2>>dend>>delim3>>len>>delim4>>delt>>delim5>>x0>>footer;
    if(!in)return in;
    if(delim1!=delim2 ||
            delim2!=delim3||
            delim3!=delim4||
            delim4!=delim5||
              delim5!=',' ||
               footer!='}'||
                   dbeg<0 ||
                   dend<0 ||
                    len<0 || x0<0){
        in.clear(std::ios::failbit);
        return in;
    }
    cone=conoid(mat,dbeg,dend,len,delt);
    cone.setX0(x0);
    return in;
}
