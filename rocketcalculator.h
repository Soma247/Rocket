#ifndef ROCKETCALCULATOR_H
#define ROCKETCALCULATOR_H
#include "rocketmodel.h"
#include "abstractatmosphere.h"
#include "memory"
#include <math.h>
#include "rocketcalcexceptions.h"


struct hardFuel{

};

struct InputData{
    double Dmid;
    double Hmax;//максимальная высота цели
    double Hmin;//минимальная высота полета ракеты
    double Xmax;//дальняя граница зоны поражения
    double Lmax;//наклонная дальность
    double Vtar;//скорость цели
    double mstone;//Рубеж выполнения задания (минимальная дальность от точки старта)

    double Taverage;//среднее время полета к цели
    double Vavg;//средняя скорость полета к цели на наибольшее расстояние
    InputData(){}
    InputData(double DmidMax, double Hmaximum, double Hminimum, double Xmaximum, double Vtarget,  double milestone);
    bool iscorrect()const;

};

//V0^2*t^2 - X^2 + 2X*Vtar*T - Vtar^2*t^2 -H^2=0;
//(V0^2-Vtar^2)*t^2+(2*x*Vtar)*t -(X^2+H^2)=0;
//x1,2=(-b+-sqrt(b^2-4*a*c))/2*a
//x=-2*Xmax*Vtar+-sqrt((2*Xmax*Vtar)^2+4*((X^2+H^2)(V0^2-Vtar^2)))/2*(V0^2-Vtar^2)

class RocketCalculator{

    InputData indat;
    hardFuel fueltype;

    std::unique_ptr<RocketModel> model;//модель ракеты (нос, отсеки, ду, несущие плоскости и параметры вцелом)
    std::unique_ptr<AbstractAtmosphere> atmo;//характеристики атмосферы
public:
    RocketCalculator(){}
    RocketCalculator(const InputData& data):indat{data}{}
    void setModel(std::unique_ptr<RocketModel> rm);
    void setModel(RocketModel* rm);
    void setAtmosphere(std::unique_ptr<AbstractAtmosphere> aa);
    void setAtmosphere(AbstractAtmosphere* aa);
    void testprint();//проверка загрузки конкретной модели
};

#endif // ROCKETCALCULATOR_H
