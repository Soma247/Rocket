#include <QApplication>
#include <iostream>
#include <memory>
#include "ballisticcalc.h"
#include "Gui/workwindow.h"


int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    WorkWindow ww("D:/Qt/projects/Rocket/materials.txt","D:/Qt/projects/Rocket/hardfuels.txt");
    ww.show();
    std::ifstream ifs("D:/Qt/projects/Rocket/materials.txt");
    try{/*
        std::list<double> m;
        std::cout<<"--------------------------"<<m.front()<<" "<<m.back()<<std::endl;
        matherial corp("orpl",7810,1860);
        matherial tzp("tzp",1600,1600);
        matherial nozzle("steel",4700,4700);
        matherial br("br",1300,1300);
        fuel fl("fuel",15,2460,1800,3300,290,1.16,5.75,0.4);
        double Dmax=0.255;
        InputData indat(Dmax,40000,1000,120000,350,1500,corp,nozzle,br,tzp,fl,10,0.06);
        ballisticCalculator rc(
                            indat,
                            std::make_unique<RocketModel>(Dmax,true),
                            std::make_unique<fInterpAtmosphere>("1.txt")
        );
        rc.setNosecone(br,0.19,0.5,0.05);
        rc.addConoid(nozzle,0.2,0.22,0.5,0.05);
        rc.addConoid(nozzle,0.22,0.24,0.7,0.05);
        rc.addConoid(nozzle,0.24,0.255,0.1,0.05);
        rc.addEquipment("gc",1.0,25);
        rc.addEquipment("bch",1.6,45);
        rc.addplane(nozzle,1.05,0.25,0.15,0.03,0.01,0.1,0.01,0.01,0.01,0.01,0.15);
        rc.addplane(nozzle,5.55,0.45,0.25,0.05,0.02,0.15,0.05,0.05,0.05,0.05,0.35);
        std::cout<<"calc:"<<std::endl;
        rc.calculate();*/
    }
    catch(const std::out_of_range& org){
        std::cout<<org.what()<<std::endl;
    }
    catch (const InvalidIALparameter&ifn){
        std::cout<<"atmo:invalid filename:"<<ifn.what()<<std::endl;
    }
    catch (const IDexception& idex){
        std::cout<<"invalid input data parameter"<<idex.what()<<std::endl;
    }
    catch (const std::invalid_argument& ia){
        std::cout<<"invalid argument "<<ia.what()<<std::endl;
    }
    catch (const std::exception& e){
        std::cout<<"some exception"<<e.what()<<std::endl;
    }
    catch (...){
        std::cout<<"some bug"<<std::endl;
    }
   return app.exec();
}
