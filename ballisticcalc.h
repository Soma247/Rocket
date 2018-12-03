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

//V0^2*t^2 - X^2 + 2X*Vtar*T - Vtar^2*t^2 -H^2=0;
//(V0^2-Vtar^2)*t^2+(2*x*Vtar)*t -(X^2+H^2)=0;
//x1,2=(-b+-sqrt(b^2-4*a*c))/2*a
//x=-2*Xmax*Vtar+-sqrt((2*Xmax*Vtar)^2+4*((X^2+H^2)(V0^2-Vtar^2)))/2*(V0^2-Vtar^2)

class RocketCalculator{

    InputData indat;


    std::unique_ptr<RocketModel> model;//модель ракеты (нос, отсеки, ду, несущие плоскости и параметры вцелом)
    std::unique_ptr<AbstractAtmosphere> atmo;//характеристики атмосферы
public:
    RocketCalculator(AbstractAtmosphere* atm):atmo{atm}
    {
    }
    RocketCalculator(const InputData& data, AbstractAtmosphere* atm):indat{data},atmo{atm}
    {
    }
    bool iscorrect();
    void setModel(std::unique_ptr<RocketModel> rm);
    void setModel(RocketModel* rm);
    void setAtmosphere(std::unique_ptr<AbstractAtmosphere> aa);
    void setAtmosphere(AbstractAtmosphere* aa);
    void testprint();//проверка загрузки конкретной модели
    void openProject(std::string proFile);

    void calculate();
};

#endif // BALLISTICCALC_H
