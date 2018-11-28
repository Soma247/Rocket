#include "rocketcalculator.h"

void RocketCalculator::setModel(std::unique_ptr<RocketModel> rm){
    model.reset(rm.release());
}

void RocketCalculator::setModel(RocketModel *rm){
    model.reset(rm);
}

void RocketCalculator::setAtmosphere(std::unique_ptr<AbstractAtmosphere> aa){
    atmo.reset(aa.release());
}

void RocketCalculator::setAtmosphere(AbstractAtmosphere *aa){
    atmo.reset(aa);
}


bool InputData::iscorrect()const{
    return Dmid>0 && Hmax>Hmin && Hmin>0 && Xmax>0 && mstone>0 && Vtar>0;
}

InputData::InputData(double DmidMax, double Hmaximum, double Hminimum, double Xmaximum, double Vtarget, double milestone):
    Dmid{DmidMax},
    Hmax{Hmaximum},
    Hmin{Hminimum},
    Xmax{Xmaximum},
    Vtar{Vtarget},
    mstone{milestone}{
    if(!iscorrect())throw IDexception("incorrect input parameter");//заменить на подходящую
    Taverage=(Xmax-mstone)/Vtar;
    Lmax=sqrt(Hmax*Hmax+mstone*mstone)/(Taverage+7);
    Vavg=Lmax/Taverage;
}
