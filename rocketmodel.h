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
#include <iostream>
class RocketModel
{
public:

    RocketModel(){}
    RocketModel(double Dmid, bool isXplane):Dmax{Dmid},isxplane{isXplane}{}


    void setNosecone(matherial math,double Dend, double len, double delta);

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
    double getmasscenter()const;
    double getmassend()const;
    double getmasscenterend()const;
    double getLength()const;

    void setEngine(matherial mathShell, matherial mathbr, matherial mathnozzle, matherial mathtzp,
                   fuel fuel,double fuelmass, double Pk, double Pa);

    void addConoid(matherial math, double Dbegin, double Dend, double length, double delta);
    void insertConoid(matherial math, double Dbegin, double Dend, double length, double delta, size_t num=100);

    void addplane(matherial math,std::string pname,
                  double Xfromnose,
                  double Broot, double Btip,
                  double Croot, double Ctip,
                  double Xtip, double Xrf,
                  double Xrr, double Xtf,
                  double Xtr, double H);

    void addEquipment(std::string eqname, double X, double mass);

    void ejectConoid(size_t num);
    void ejectPlane(size_t num);
    void ejectEquipment(size_t num);

    std::pair<double,double> getCxCyaa(double M,double alphagrad, double sound_sp, double cin_visc,bool engineisactive)const;

    double getXp(double M, double sound_sp, double cin_visc)const{
        if(M<0.5)M=0.5;
        double cyan=Aerodynamics::CyaNoseCyll(Dmax,SmidLA,Lnc,Lcyl,M)*0.25*Dmax*Dmax*M_PI/SmidLA;
        double xpf=Lnc*(2/3.0+Aerodynamics::xpf(Dmax,Lnc,Lcyl,M))*cyan;
        double k=isxplane?2/sqrt(2):1;
        double cyap=0;
        for(const plane& p:pplanes){
            planeparams par=p.getparams();
            xpf+=k*2*p.getcyaConsole(Dmax,SmidLA,M,sound_sp,cin_visc)*p.getXp(M);
            cyap+=k*2*p.getcyaConsole(Dmax,SmidLA,M,sound_sp,cin_visc);
        }
        return xpf/(cyan+cyap);
    }
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
    std::unique_ptr<engine> pengine;
    std::unique_ptr<conoid> ptailcone;
    std::vector<conoid>pconoids;
    std::vector<plane>pplanes;
    std::vector<equipment>pequipments;
};

#endif // ROCKETMODEL_H
