#ifndef ROCKETMODEL_H
#define ROCKETMODEL_H
#include<string>
#include <vector>
#include <memory>
#include "rocket_elements/equipment.h"
#include "rocket_elements/plane/plane.h"
#include "rocket_elements/modules/conoid.h"
#include "rocket_elements/modules/nosecone.h"
#include "rocket_elements/modules/engine.h"
#include "aerodynamics/aerodynamics.h"
#include <algorithm>

using equipmentparameters=std::pair<double,double>;
struct RocketHeadData{
    coneparam nconepar;
    planeparams tailstabpar;
    std::vector<coneparam>conespar;
    std::vector<planeparams>planespar;
    std::vector<equipmentparameters>equipspar;
};



class RocketModel{
public:
    RocketModel(){}
    RocketModel(double Dmid, bool isXplane):Dmax{Dmid},isxplane{isXplane}{}

    RocketHeadData getheadData();
    coneparam getNCparams()const;
    coneparam getTailConeParams()const;
    coneparam getModuleParams(size_t num)const;
    engineparameters getEngineParams()const;
    planeparams getPlaneParams(size_t num)const;
    std::pair<double,double> geteqparams(size_t num)const;

    size_t getModulesCount()const{return pconoids.size();}
    size_t getEquipmentsCount()const{return pequipments.size();}
    size_t getPlanesCount()const{return pplanes.size();}

    double getmass()const;
    double getheadmass()const;
    double getmasscenter()const;
    double getmassend()const;
    double getmasscenterend()const;
    double getLength()const;  


    void setNosecone(material math,double Dend, double len, double delta);
    void setTailStab(material math,
                  double Xfromnose,
                  double Broot, double Btip,
                  double Croot, double Ctip,
                  double Xtip, double Xrf,
                  double Xrr, double Xtf,
                  double Xtr, double H);
    void setEngine(material mathShell, material mathbr, material mathnozzle, material mathtzp,
                   fuel fuel,double fuelmass, double Pk, double Pa);
    void addConoid(material math, double Dbegin, double Dend, double length, double delta);
    void insertConoid(material math, double Dbegin, double Dend, double length, double delta, size_t num=100);
    void addplane(material math,
                  double Xfromnose,
                  double Broot, double Btip,
                  double Croot, double Ctip,
                  double Xtip, double Xrf,
                  double Xrr, double Xtf,
                  double Xtr, double H);
    void setscalePlane(size_t num, double scale);
    void addEquipment(std::string eqname, double X, double mass);

    void ejectConoid(size_t num);
    void ejectPlane(size_t num);
    void ejectEquipment(size_t num);

    std::pair<double,double> getCyaaCx(double M,double alphagrad, double sound_sp, double cin_visc,bool engineisactive)const;
    double getXp(double M, double sound_sp, double cin_visc)const;
    double getbalanceEnd(double M=3,double sound_sp=340, double cin_visc=1.4E-5);
    double getbalanceStart(double M=1,double sound_sp=340, double cin_visc=1.4E-5);
    double getS()const{return SmidLA;}
    bool isheadcorrect(){
        update();
        return pnosecone&&
                Dmax>0;
    }

    std::vector<size_t>state()const;

    friend std::ostream& operator<<(std::ostream &os, const RocketModel& rm);
    friend std::istream& operator>>(std::istream &in, RocketModel& rm);

protected:
    double getCp(double M, bool engineisactive)const;
    double getCxTr(double M,double sound_sp, double cin_visc)const;
    double getCya(double M, double sound_sp, double cin_visc)const;
    double getCx0(double M,double sound_sp, double cin_visc, bool engineisactive)const;
    double getCxai(double cyala,double M,double alphagrad,double sound_sp, double cin_visc)const;

    void update();

    double Dmax=0;
    double SmidLA=0;
    double Lnc=0;
    double Lcyl=0;
    bool isxplane=true;
    std::unique_ptr<nosecone> pnosecone;
    std::unique_ptr<plane> ptailplane;
    std::unique_ptr<engine> pengine;
    std::unique_ptr<conoid> ptailcone;
    std::vector<conoid>pconoids;
    std::vector<plane>pplanes;
    std::vector<equipment>pequipments;
};



#endif // ROCKETMODEL_H
