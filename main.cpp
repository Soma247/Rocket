#include <iostream>
#include <memory>
#include "ballisticcalc.h"
struct myclass{
    myclass(std::initializer_list<std::string> l){
        for(const std::string& s:l)
            std::cout<<s<<std::endl;
    }
};

int main(){

    try{
        matherial corp("orpl",7810,1860);
        matherial tzp("tzp",1600,1600);
        matherial nozzle("steel",4700,4700);
        matherial br("br",1300,1300);
        fuel fl("fuel",15,2460,1800,3300,290,1.16,5.75,0.4);

        RocketModel* rm=new RocketModel(0.20,true);
        rm->setNosecone(br,0.2,0.45,0.05);
        rm->addConoid(nozzle,0.2,0.24,0.5,0.05);
        rm->addConoid(nozzle,0.24,0.25,0.5,0.05);
        rm->addConoid(nozzle,0.25,0.255,0.5,0.05);
        rm->addEquipment("gc",1.0,25);
        rm->addEquipment("bch",1.6,45);
        rm->addplane(nozzle,1.05,0.25,0.15,0.03,0.01,0.1,0.01,0.01,0.01,0.01,0.15);
        rm->addplane(nozzle,5.55,0.45,0.25,0.05,0.02,0.15,0.05,0.05,0.05,0.05,0.35);

        RocketCalculator rc(InputData(0.255,40000,1000,120000,350,1500,corp,nozzle,br,tzp,fl,10,0.06),new fInterpAtmosphere("1.txt"));
        rc.setModel(rm);
        std::cout<<"calc:"<<std::endl;
        rc.calculate();
    }
    catch(const std::out_of_range& org){
        std::cout<<org.what()<<std::endl;
    }
    catch (const InvalidIALparameter&ifn){
        std::cout<<"invalid filename:"<<ifn.what()<<std::endl;
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
    return 0;
}
