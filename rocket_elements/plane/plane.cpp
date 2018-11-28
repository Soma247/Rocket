#include <math.h>
#include <iostream>
#include "plane.h"
#include "aerodynamics/aerodynamics.h"
double plane::massCenter() const{return 0.6*params.broot+params.XfromNose;}

double plane::getCp(double SmidLA,double M) const{
    if(M<1)return S()*(0.1*M+0.06)/SmidLA;
    return Aerodynamics::CxWavPlaneOneConsole(SmidLA,
                                    params.broot,params.btip,
                                    (params.broot+params.btip)/(params.broot+params.btip-params.xrf-params.xrr-params.xtf-params.xtr),
                                    params.croot,params.ctip,
                                    params.h,params.angleCmax,M);
}

double plane::getCxtr(double SmidLA, double M, double sound_sp, double cin_visc) const{
    return Aerodynamics::CxTrPlane(SmidLA,params.h,params.broot,params.btip,params.croot,params.ctip,M,sound_sp,cin_visc);
}

double plane::getcyaConsole(double Dmid, double SmidLA, double M, double sound_sp, double cin_visc) const{
    return Aerodynamics::CyaOnePlaneConsole(Dmid,SmidLA,params.XfromNose,params.broot,
                                            params.btip,params.croot,params.ctip,
                                            params.h,params.angleCmax,M,sound_sp,cin_visc);
}

plane::plane(matherial math, std::string pname,
             double Xfromnose,
             double Broot, double Btip,
             double Croot, double Ctip,
             double Xtip, double Xrf,
             double Xrr, double Xtf, double Xtr,
             double H, double N):name{pname}{

    params.mat=math;
    params.XfromNose=Xfromnose;
    params.broot=Broot;
    params.btip=Btip;
    params.croot=Croot;
    params.ctip=Ctip;
    params.xtip=Xtip;
    params.xrf=Xrf;
    params.xrr=Xrr;
    params.xtf=Xtf;
    params.xtr=Xtr;
    params.h=H;
    params.n=N;
    params.angle0=atan(params.h/params.xtip);
    params.angleCmax=atan(params.h/(params.xtip-params.xrf+params.xtf));
    params.massn=params.n*0.5*(params.btip+params.broot)*params.h*0.5*(params.ctip+params.croot)*params.mat.Ro;
}
