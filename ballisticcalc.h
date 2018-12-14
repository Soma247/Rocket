#ifndef BALLISTICCALC_H
#define BALLISTICCALC_H
#include "rocketmodel.h"
#include "abstractatmosphere.h"
#include "memory"
#include <math.h>
#include "rocketcalcexceptions.h"

/*    double Hmax=600,tet0=M_PI/4,Xmax=1000,y0=15,x0=0,
     A=(Hmax-tan(tet0)*Xmax-y0)/(Xmax*Xmax),
       B=tan(tet0),
       C=y0;
       y(double x){return A*x*x+B*x+C;}
       teth(t)=atan((Hmax-y0)/Xmax+2*A*x(t)+B)

*/
struct baldat{
    double t=0;
    double V=0;//скорость
    double H=0;//высота
    double X=0;//горизонт дальность
    double teth=0;//угол наклона
    double mass=0;
    double cx=0;
    double cy=0;
};

struct InputData{
    double Dmid=0;
    double Hmax=0;//максимальная высота цели
    double Hmin=0;//минимальная высота полета ракеты
    double Xmax=0;//дальняя граница зоны поражения
    double Lmax=0;//наклонная дальность
    double Vtar=0;//скорость цели
    double mstone=0;//Рубеж выполнения задания (минимальная дальность от точки старта)
    double teth0=M_PI/8;//угол наклона траектории к линии визирования на старте(dy/dx,x=x0)
    double Taverage;//среднее время полета к цели
    double Vavg;//средняя скорость полета к цели на наибольшее расстояние
    matherial enmatShell;
    matherial enmatnozzle;
    matherial enmatbr;
    matherial enmatTz;
    fuel enfl;
    double enPk=10;
    double enPa=0.06;
    InputData(){}
    InputData(double DmidMax, double Hmaximum, double Hminimum, double Xmaximum, double Vtarget,  double milestone,
              matherial enmatShell,matherial enmatnozzle, matherial enmatbr,  matherial enmatTz, fuel enfl, double enPk,
    double enPa);
    bool iscorrect()const;

};
struct RocketHeadData{
    coneparam nconepar;
    std::vector<coneparam>conespar;
    std::vector<planeparams>planespar;
    std::vector<equipmentparameters>equipspar;
};
struct OutputData{
    std::vector<baldat>traect;
    RocketHeadData headData;
    engineparameters engpar;
    double massfull;
    double massempty;
    double xmfull;
    double xmempty;
    double xdaverage;
};



class ballisticCalculator{
    InputData indat;
    std::unique_ptr<RocketModel> model;//модель ракеты (нос, отсеки, ду, несущие плоскости и параметры вцелом)
    std::unique_ptr<AbstractAtmosphere> atmo;//характеристики атмосферы
public:
    ballisticCalculator(AbstractAtmosphere* atm);
    ballisticCalculator(const InputData& data, AbstractAtmosphere* atm);
    ballisticCalculator(const InputData& data, std::unique_ptr<AbstractAtmosphere>atm);
    ballisticCalculator(const InputData& data, std::unique_ptr<RocketModel> rm,
                                               std::unique_ptr<AbstractAtmosphere>atm);
    bool iscorrect();
    void setModel(std::unique_ptr<RocketModel> rm);
    void setModel(RocketModel* rm);
    void setAtmosphere(std::unique_ptr<AbstractAtmosphere> aa);
    void setAtmosphere(AbstractAtmosphere* aa);
    void setInputData(const InputData& data);
    InputData getInputData()const{return indat;}
    void setNosecone(matherial math,double Dend, double len, double delta);
    void setEngine(matherial mathShell, matherial mathbr, matherial mathnozzle, matherial mathtzp,
                   fuel fuel,double fuelmass, double Pk=10, double Pa=0.06);
    void addConoid(matherial math, double Dbegin, double Dend, double length, double delta);
    void insertConoid(matherial math, double Dbegin, double Dend, double length, double delta, size_t num=100);
    void addplane(matherial math,
                  double Xfromnose,
                  double Broot, double Btip,
                  double Croot, double Ctip,
                  double Xtip, double Xrf,
                  double Xrr, double Xtf,
                  double Xtr, double H);
    void setscalePlane(size_t num, double scale);
    void addEquipment(std::string eqname, double X, double mass);
    void openProject(std::string proFile);
    void saveProject(std::string proFile)const;
    OutputData calculate();
    std::vector<size_t>state()const;
    RocketHeadData getheadData();

};



#endif // BALLISTICCALC_H
