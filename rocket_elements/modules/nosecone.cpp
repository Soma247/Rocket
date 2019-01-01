#include <iostream>
#include <math.h>
#include "nosecone.h"
#include "aerodynamics/aerodynamics.h"

const std::string noseconeheader{"nosecone"};

double nosecone::Smid() const{
    return 0.25*M_PI*dend*dend;
}

double nosecone::mass() const{
    return mat.Ro*(Smid()-0.25*M_PI*pow(dend-2*delt,2))*getL()/3.0;
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

coneparam nosecone::getparams() const{
    return coneparam{mat,0,dend,len,delt,mass()};
}
void nosecone::setDend(double d){
    dend=d;
}

nosecone::nosecone(material math, double Dend, double length, double delta):
    mat{math},dend{Dend},len{length},delt{delta}
{
}




std::ostream &operator<<(std::ostream &os, const nosecone &ncone){
    coneparam par=ncone.getparams();
    return os<<noseconeheader<<'{'<<par.mat<<','<<par.dend
            <<','<<par.len<<','<<par.delt<<'}';
}

std::istream &operator>>(std::istream &in, nosecone &ncone){
    std::string header;
    material mat;
    char delim1{0},delim2{0},delim3{0},footer{0};
    int tmp{0};
    double dend{0},len{0},delt{0};

    while((tmp=in.get())!=EOF && isspace(tmp));
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=noseconeheader){
        in.clear(std::ios::failbit);
        return in;
    }
    in>>mat>>delim1>>dend>>delim2>>len>>delim3>>delt>>footer;
    if(!in)return in;
    if(delim1!=delim2 ||
            delim2!=delim3||
            delim3!=',' ||
            footer!='}'||
            dend<0 ||
            len<0){
        in.clear(std::ios::failbit);
        return in;
    }
    ncone=nosecone(mat,dend,len,delt);
    return in;
}
