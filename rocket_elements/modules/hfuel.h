#ifndef HFUEL_H
#define HFUEL_H

#include <string>

const std::string fuelheader{"Hfuel"};
struct fuel{
    std::string name;
    double AL=0;//% в топливе
    double Prelst=0;
    double rot=0;
    double tst=0;
    double rst=0;
    double kst=0;
    double ukoef=0;
    double upow=0;
    double U(double pk);
    bool operator==(const fuel& second)const;
    bool iscorrect()const;
    bool operator!=(const fuel&second)const;
    fuel(){}
    fuel(std::string modulename, double Alper,
         double PrelativeST,double RoT, double Tst,
         double Rst, double Kst, double Ukoef, double Upow);
    ~fuel(){}
};
    std::ostream &operator<<(std::ostream &os, const fuel &fl);
    std::istream &operator>>(std::istream &in, fuel& fl);
#endif // HFUEL_H
