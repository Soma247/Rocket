#include "abstractatmosphere.h"
#include "interpolation/newtoninterp.h"

fInterpAtmosphere::fInterpAtmosphere(){}

fInterpAtmosphere::~fInterpAtmosphere(){
    high.clear();
    temperature.clear();
    sound_sp.clear();
    Pression.clear();
    density.clear();
    cinematic_viscidity.clear();
}

void fInterpAtmosphere::loadfromFile(const std::string& fname){
    std::ifstream ifs(fname);
    ifs.exceptions(ifs.exceptions()|std::ios_base::badbit);

    if(!ifs)throw InvalidIALparameter(fname);
    int linecount=0;
    while(ifs){
        InterpAtmoLine temp;
        char c1{0},c2{0},c3{0},c4{0},c5{0},c6{0},c7{0};
        ifs>>c1>>temp.high>>
                c2>>temp.temperature>>
                c3>>temp.sound_sp>>
                c4>>temp.Pression>>
                c5>>temp.density>>
                c6>>temp.cinematic_viscidity>>
                c7;

        if(!ifs.eof()&&
               (!ifs||
                c1!='{'||
                c2!=c3||
                c3!=c4||
                c4!=c5||
                c5!=c6||
                c6!=','||
                c7!='}'
               // ||!temp.iscorrect()
               )
           ){
            std::cout<<char(c1)<<temp.high<<
                    char(c2)<<temp.temperature<<
                    char(c3)<<temp.sound_sp<<
                    char(c4)<<temp.Pression<<
                    char(c5)<<temp.density<<
                    char(c6)<<temp.cinematic_viscidity<<
                    char(c7)<<std::endl;
            throw std::invalid_argument(std::string("line ")+std::to_string(linecount));

        }
        else if(!ifs.eof()){
            high.push_back(temp.high);
            temperature.push_back(temp.temperature);
            sound_sp.push_back(temp.sound_sp);
            Pression.push_back(temp.Pression);
            density.push_back(temp.density);
            cinematic_viscidity.push_back(temp.cinematic_viscidity);
        }
        linecount++;
    }
}

double fInterpAtmosphere::get_sound_sp(double h) const{
    return newtonInterp::interp2d(high,sound_sp,h,2);
}

double fInterpAtmosphere::get_cinem_viscidity(double h) const{
    return newtonInterp::interp2d(high,cinematic_viscidity,h,2);
}

double fInterpAtmosphere::get_Pression(double h) const{
    return newtonInterp::interp2d(high,Pression,h,2);
}

double fInterpAtmosphere::get_density(double h) const{
    return newtonInterp::interp2d(high,density,h,2);
}



std::string AbstractAtmosphere::toString()
{
    return "AbstractAtmosphere";
}

InvalidIALparameter::InvalidIALparameter(const std::string &fname):message{fname}{}


