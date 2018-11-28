#include <iostream>
#include <memory>
#include "abstractatmosphere.h"
#include "rocketmodel.h"
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


      plane pl(br,"1",3.5,0.45,0.25,0.08,0.03,0.25,0.05,0.05,0.05,0.05,0.35);
      std::cout<<pl.getCp(0.25*0.255*0.255*M_PI,3)<<std::endl;
      std::cout<<pl.getCxtr(0.25*0.255*0.255*M_PI,3,340,1.4E-5)<<std::endl;
      std::cout<<pl.getcyaConsole(0.255,0.25*0.255*0.255*M_PI,3,340,1.4E-5)<<
                 " "<<57.3*4/sqrt(5*5-1)<<std::endl<<std::endl;

      RocketModel rm(0.20,true);
      rm.setNosecone(corp,0.2,0.45,0.05);
      rm.setEngine(corp,br,nozzle,tzp,fl,100,10,0.06);
      rm.addConoid(corp,0.2,0.24,0.5,0.05);
      rm.addConoid(corp,0.24,0.25,0.5,0.05);
      rm.addEquipment("gc",1.25,45);
      rm.addplane(br,"destab",0.3,0.25,0.15,0.03,0.01,0.1,0.01,0.01,0.01,0.01,0.15);
      rm.addplane(br,"wing",1.05,0.25,0.15,0.03,0.01,0.1,0.01,0.01,0.01,0.01,0.15);
      rm.addplane(br,"wing",3.55,0.45,0.25,0.05,0.02,0.15,0.05,0.05,0.05,0.05,0.35);
      std::cout<<"rmass="<<rm.getmass()<<" rlen= "<<rm.getLength()<<" rxm= "<<rm.getmasscenter()<<std::endl;
      std::cout<<"rmasse="<<rm.getmassend()<<" rlen= "<<rm.getLength()<<" rxme= "<<rm.getmasscenterend()<<std::endl;
      for(double M=0.0,a=5;M<5;M+=0.1)
      std::cout<<" "<<M<<" xp "<<rm.getXp(M,300,0.0006)<<std::endl<<
                 " cy="<<rm.getCxCyaa(M,a,300,0.0006,true).first<<" cx="<<rm.getCxCyaa(M,a,300,0.0006,true).second<<std::endl;
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
