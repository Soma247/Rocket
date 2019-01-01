#include "equipment.h"
#include <iostream>
const std::string equipmentheader{"equipment"};
equipment::equipment(std::string eqname, double X, double mass):
    name{eqname},eqmass{mass},Xm{X}
{
}
double equipment::Smid() const{return 0;}

double equipment::mass() const{return eqmass;}

void equipment::setmass(double m){eqmass=m;}

double equipment::massCenter() const{return Xm;}

void equipment::setmassCenter(double xm){Xm=xm;}

std::string equipment::getname() const{return name;}

void equipment::setname(std::string n){name=n;}

std::string equipment::toString() const{return "equipment";}


std::ostream &operator<<(std::ostream &os, const equipment &eq){
    return os<<equipmentheader<<'{'<<eq.getname()<<','<<eq.mass()<<','<<eq.massCenter()<<'}';
}

std::istream &operator>>(std::istream &in, equipment &eq){
    std::string name,header;
    char delim{0},footer{0};
    int tmp{0};
    double mass{0},masscenter{0};
    while((tmp=in.get())!=EOF && isspace(tmp));
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=equipmentheader){
        in.clear(std::ios::failbit);
        return in;
    }
    while((tmp=in.get())!=EOF && tmp!=',')
        name.push_back(static_cast<char>(tmp));
    if(tmp==EOF)return in;

    in>>mass>>delim>>masscenter>>footer;
    if(!in)return in;
    if(delim!=',' || footer!='}'|| mass<0 || masscenter<0){
        in.clear(std::ios::failbit);
        return in;
    }
    eq=equipment(name,masscenter,mass);
    return in;
}

