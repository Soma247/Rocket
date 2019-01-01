#ifndef ABSTRACTATMOSPHERE_H
#define ABSTRACTATMOSPHERE_H
#include <vector>
#include <string>
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
    bool iscorrect()const;
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
