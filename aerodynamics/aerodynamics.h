#ifndef AERODYNAMICS_H
#define AERODYNAMICS_H
#include "aerodynamics_tables.h"

struct Planedata{
    double L;//размах 1 консоли
    double hordRoot;//корневая хорда
    double hordTip;//концевая хорда
    double cRoot;//толщина профиля крыла
    double cTip;
    double cmaxhordRoot;
    double cmaxhordTip;
    double L0;//расстояние от носа до передней точки бортовой хорды
};

struct LaData{
    double SmidLA;
    double Dmid;
    double Dstern;
    double Lnose;
    double Lcyl;
    double Lstern;
};


struct Aerodynamics{
//Сопротивление трения частей ла, S/SmidLa
static double CxTrF(double SmidLA, double Dmid, double Lnose, double Lcyl,
                    double LStern, double Dstern, double M, double sound_sp, double cin_visc, double ht=20.0);
static double CxTrPlane(double SmidLA, double h, double Broot,
                        double Btip, double croot, double ctip,
                        double M, double sound_sp, double cin_visc,
                        double ht=20.0);

//сопротивление давления корпуса, S/SmidKorp
static double CxpNoseCone(double len, double diameter, double Dmid, double M);
static double CxpConoid(double len, double d1, double d2, double Dmid, double M);
static double CxpSternBottom(double len, double Dmid,double DStern, double Dnozzle, double M, bool isEnActive);

//волновое сопротиление крыла s/smidla
static double CxWavPlaneOneConsole(double SmidLA, double broot, double btip, double k, double croot, double ctip, double h, double anglecmax, double M);



//производная коэфф. подъемной силы для частей ла
static double CyaNoseCyll(double Dmid, double SmidLA, double Lnose, double Lcyl, double M);
static double CyaOnePlaneConsole(double Dmid, double SmidLA,double X0,
                                 double broot, double btip, double croot,
                                 double ctip,double h,  double anglecmax,
                                 double M,double sound_sp, double cin_visc);


static double xpf(double Dmid,double Lnose,double Lcyl, double M);
};


#endif // AERODYNAMICS_H
