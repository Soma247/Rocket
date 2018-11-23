#ifndef ABSTRACTATMOSPHERE_H
#define ABSTRACTATMOSPHERE_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include "rocketcalcexceptions.h"


    class AbstractAtmosphere{
    public:
        virtual ~AbstractAtmosphere(){}
        virtual double get_sound_sp(double h)const=0;
        virtual double get_cinem_viscidity(double h)const=0;
        virtual double get_Pression(double h)const=0;
        virtual double get_density(double h)const=0;
        virtual std::string toString();
    };
    
    
    struct InterpAtmoLine{
        double high;
        double temperature;
        double sound_sp;
        double Pression;
        double density;
        double cinematic_viscidity;
        bool iscorrect()const{
            if(high<0||high>80000||//m
                    temperature>1000||temperature<190||//K
                    sound_sp<280||sound_sp>350||//m/sec
                    Pression<1||Pression>127800||//Pa
                    density<0||density>1.5||//kg/m^3
                    cinematic_viscidity<0.000012525||
                    cinematic_viscidity>0.716//m^2/sec
                    )return false;
            return true;
        }
    };
    

    
    class fInterpAtmosphere:public AbstractAtmosphere{
        std::vector<double> high;
        std::vector<double> temperature;
        std::vector<double> sound_sp;
        std::vector<double> Pression;
        std::vector<double> density;
        std::vector<double> cinematic_viscidity;
    
    public:
        fInterpAtmosphere();
        fInterpAtmosphere(fInterpAtmosphere& fi)=delete;
        fInterpAtmosphere(const std::string& fname){
            loadfromFile(fname);
        }
        fInterpAtmosphere(fInterpAtmosphere&& fi)=delete;
        ~fInterpAtmosphere()override;
        void loadfromFile(const std::string& fname);
    
        double get_sound_sp(double h)const override;
        double get_cinem_viscidity(double h)const override;
        double get_Pression(double h)const override;
        double get_density(double h)const override;
    };

#endif // ABSTRACTATMOSPHERE_H
