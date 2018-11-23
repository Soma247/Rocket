#include <iostream>
#include "abstractatmosphere.h"
#include "rocketcalculator.h"
#include <memory>
#include "rocket_elements/modules/engine.h"
struct myclass{
    myclass(std::initializer_list<std::string> l){
        for(const std::string& s:l)
            std::cout<<s<<std::endl;
    }
};

int main(){

    try{
        std::vector<std::unique_ptr<AbstractAtmosphere>> v;
        fInterpAtmosphere atm;//("../Rocket/1.txt");
        atm.loadfromFile("../Rocket/1.txt");

        v.push_back(std::unique_ptr<AbstractAtmosphere>(new fInterpAtmosphere("../Rocket/1.txt")));
        for(auto& p:v)
            std::cout<<p->get_sound_sp(1800)<<std::endl;
        v.pop_back();

        //инициализация юника в калькуляторе новой моделью
        RocketCalculator rc;
        rc.setModel(std::unique_ptr<RocketModel>(new RocketModel("123")));
        rc.testprint();

        rc.setModel(new RocketModel("345"));
        rc.testprint();
// InputData(double DmidMax, double Hmaximum, double Hminimum, double Xmaximum, double Vtarget,  double milestone):
        InputData(0.6,150000,8000,300000,550,6500);

        myclass {"first","second","third"};//проверка списка инициализации


        std::cout<<"__________________________________"<<std::endl;
        /*
        struct fuel{
            std::string name;
            double ALpercent;//% в топливе
            double PudST;
            double roT;
            double Tst;
            double Rst;
            double kst;
            double Ukoef;
            double Upow;
            double U(double pk);
        };
        */
        matherial corp("orpl",1400,1400);
        matherial tzp("tzp",1600,1600);
        matherial nozzle("steel",4700,4700);
        matherial br("br",1300,1300);
        fuel fl{"fuel",15,2460,1800,3300,290,1.16,5.75,0.4};
        engine en(corp,br,nozzle,tzp,fl,1.6,16405,10,0.06);
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
