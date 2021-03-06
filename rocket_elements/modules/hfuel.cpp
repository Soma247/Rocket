#include "hfuel.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const fuel &fl){
    return os<<fuelheader<<'{'<<fl.name<<','<<fl.AL<<','
            <<fl.Prelst<<',' <<fl.rot<<','<<fl.tst<<','
           <<fl.rst<<','<<fl.kst<<','<<fl.ukoef<<','
          <<fl.upow<<'}';
}

std::istream &operator>>(std::istream &in, fuel &fl){
    std::string name,header;
    char delim1{0},delim2{0},delim3{0},
    delim4{0},delim5{0},delim6{0},
    delim7{0},footer{0};
    int tmp{0};
    fuel tmpfuel;

    while((tmp=in.get())!=EOF && isspace(tmp));
    if(tmp==EOF)return in;
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=fuelheader){
        std::cout<<"wrheader:"<<header<<std::endl;
        in.clear(std::ios::failbit);
        return in;
    }
    while((tmp=in.get())!=EOF && tmp!=',')
        name.push_back(static_cast<char>(tmp));
    if(tmp==EOF)return in;
    tmpfuel.name=name;
    in>>tmpfuel.AL>>delim1
            >>tmpfuel.Prelst>>delim2
            >>tmpfuel.rot>>delim3
            >>tmpfuel.tst>>delim4
            >>tmpfuel.rst>>delim5
            >>tmpfuel.kst>>delim6
            >>tmpfuel.ukoef>>delim7
            >>tmpfuel.upow>>footer;

    if(!in)return in;
    if(delim1!=delim2||
            delim2!=delim3||
            delim3!=delim4||
            delim4!=delim5||
            delim5!=delim6||
            delim6!=delim7||
              delim7!=',' ||
               footer!='}'|| !tmpfuel.iscorrect()){
        in.clear(std::ios::failbit);
        return in;
    }
    fl=tmpfuel;
    return in;
}

bool fuel::operator==(const fuel &second)const{
    return name==second.name&&
            AL==second.AL&&
            Prelst==second.Prelst&&
            rot==second.rot&&
            tst==second.tst&&
            rst==second.rst&&
            kst==second.kst&&
            ukoef==second.ukoef&&
            upow==second.upow;
}

bool fuel::iscorrect() const{return AL>=0 && Prelst>=0 &&
            rot>=0 && tst>=0 &&
            rst>=0 && kst>=0 &&
            ukoef>=0 && upow>=0;
                            }

bool fuel::operator!=(const fuel &second)const{
    return !(*this==second);
}

fuel::fuel(std::string modulename, double Alper, double PrelativeST, double RoT, double Tst, double Rst, double Kst, double Ukoef, double Upow):
    name{modulename},AL{Alper},Prelst{PrelativeST},rot{RoT},tst{Tst},rst{Rst},kst{Kst},ukoef{Ukoef},upow{Upow}
{
}
