#include "ballisticcalc.h"
#include <iomanip>
#include <fstream>


const std::string indatheader="input_data";
ballisticCalculator::ballisticCalculator(AbstractAtmosphere *atm):atmo{atm}
{
}

ballisticCalculator::ballisticCalculator(const InputData &data, AbstractAtmosphere *atm):indat{data},atmo{atm}
{
}

ballisticCalculator::ballisticCalculator(const InputData &data, std::unique_ptr<AbstractAtmosphere> atm):indat{data}
{atmo.reset(atm.release());
}

ballisticCalculator::ballisticCalculator(const InputData &data, std::unique_ptr<RocketModel> rm, std::unique_ptr<AbstractAtmosphere> atm):indat{data}{
    model.reset(rm.release());
    atmo.reset(atm.release());
}

bool ballisticCalculator::iscorrect(){
    return model&&atmo&&
            model->isheadcorrect()&&
            indat.iscorrect();
}

void ballisticCalculator::setModel(std::unique_ptr<RocketModel> rm){
    model.reset(rm.release());
}

void ballisticCalculator::setModel(RocketModel *rm){
    model.reset(rm);
}

void ballisticCalculator::setAtmosphere(std::unique_ptr<AbstractAtmosphere> aa){
    atmo.reset(aa.release());
}

void ballisticCalculator::setAtmosphere(AbstractAtmosphere *aa){
    atmo.reset(aa);
}

void ballisticCalculator::setNosecone(material math, double Dend, double len, double delta){
    if(model)
        model->setNosecone(math,Dend,len,delta);
}

void ballisticCalculator::setTailStab(material math, double Xfromnose, double Broot, double Btip, double Croot, double Ctip, double Xtip, double Xrf, double Xrr, double Xtf, double Xtr, double H){
    if(model)
        model->setTailStab(math,Xfromnose,
                        Broot,Btip,
                        Croot,Ctip,
                        Xtip,Xrf,
                        Xrr,Xtf,
                        Xtr,H);
}

void ballisticCalculator::setEngine(material mathShell, material mathbr, material mathnozzle, material mathtzp, fuel fuel, double fuelmass, double Pk, double Pa){
    if(model)
        model->setEngine(mathShell, mathbr, mathnozzle,mathtzp,
                         fuel, fuelmass, Pk, Pa);
}

void ballisticCalculator::addConoid(material math, double Dbegin, double Dend, double length, double delta){
    if(model)
        model->addConoid(math, Dbegin, Dend, length,delta);
}

void ballisticCalculator::insertConoid(material math, double Dbegin, double Dend, double length, double delta, size_t num){
    if(model)
        model->insertConoid(math,Dbegin,Dend,length,delta,num);
}

void ballisticCalculator::addplane(material math, double Xfromnose, double Broot, double Btip, double Croot, double Ctip, double Xtip, double Xrf, double Xrr, double Xtf, double Xtr, double H){
    if(model)
        model->addplane(math,Xfromnose,
                        Broot,Btip,
                        Croot,Ctip,
                        Xtip,Xrf,
                        Xrr,Xtf,
                        Xtr,H);
}

void ballisticCalculator::setscalePlane(size_t num, double scale){
    if(model)
        model->setscalePlane(num,scale);
}

void ballisticCalculator::addEquipment(std::string eqname, double X, double mass){
    if(model)
        model->addEquipment(eqname,X,mass);
}

void ballisticCalculator::ejectConoid(size_t num){
    if(model)
        model->ejectConoid(num);
}

void ballisticCalculator::ejectPlane(size_t num){
    if(model)
        model->ejectPlane(num);
}

void ballisticCalculator::ejectEquipment(size_t num){
    if(model)
        model->ejectEquipment(num);
}

void ballisticCalculator::ejectTailStab()
{
    if(model)
        model->ejectTailStab();
}

void ballisticCalculator::openProject(std::string proFile)
{
    std::ifstream in(proFile);
    if(!in)throw error_reading_file(std::string("file ")+=proFile+=" is corrupted");
    RocketModel* rm=new RocketModel();
    InputData idat;
    char delim{0};
    in>>idat>>delim>>*rm;
    if(!idat.iscorrect()||
            delim!=','||(
            !in&&!in.eof()))throw error_reading_file(std::string("file ")+=proFile+=" is corrupted");
    setInputData(idat);
    setModel(rm);
}

OutputData ballisticCalculator::calculate(double Vend,double dt){

 //   std::cout<<"calc started"<<std::endl;
    if(!model||!iscorrect()||dt<=0)throw std::runtime_error("bc:1");

    double y0=15;
    double x0=0;
    double alpha=5*M_PI/180;//угол атаки
    double teth0=indat.teth0+atan((indat.Hmax-y0)/(indat.mstone));
        if(teth0>M_PI/2)teth0=atan((indat.Hmax-y0)/indat.mstone);

    for(double i=0.3;i<0.95;i+=0.01){//подбор относительной массы топлива

        model->setEngine(indat.enmatShell,indat.enmatbr,
                         indat.enmatnozzle,indat.enmatTz,indat.enfl,
                         i*model->getheadmass()/(1-i),indat.enPk,indat.enPa);

        double M0=model->getmass();//масса
        double A=(indat.Hmax-tan(teth0)*indat.mstone-y0)/(indat.mstone*indat.mstone);
        double B=tan(teth0);
      //  double C=y0;
     //   std::cout<<"A="<<A<<" B="<<B<<std::endl;
  //      std::cout<<"massfuel"<< i*model->getheadmass()/(1-i)<<std::endl;
        engineparameters enpar=model->getEngineParams();
        std::vector<baldat> ballisticData;
        ballisticData.reserve(static_cast<size_t>(indat.TaverageC()/dt));
        double V=35;//скорость
        double H=y0;//высота
        double X=x0;//горизонт дальность
        double teth=teth0;//угол наклона
        double mass=M0;
        double massend=model->getmassend();
        double dV{0},dH{0},dX{0};
        double q{0};
        double P{0};
        double masscenter=model->getmasscenter();
        double massendcenter=model->getmasscenterend();
        std::pair<double,double>cyacx;
        double dmasscenter=(model->getmasscenterend()-model->getmasscenter())/indat.TaverageC();
        for(double t=0;t<=indat.TaverageC();t+=1){//одна траектория по времени полета
            V+=dV;
            H+=dH;
            X+=dX;
          //  teth+=dteth;

            q=atmo->get_density(H)*V*V/2;
            cyacx= model->getCyaaCx(
                        V/atmo->get_sound_sp(H),
                        alpha*180/M_PI,
                        atmo->get_sound_sp(H),
                        atmo->get_cinem_viscidity(H),
                        t<enpar.t
                        );

            P=(t<enpar.t)?enpar.Pvacuum:0.0;
            dV=(P*cos(alpha)-cyacx.second*q*model->getS()-
                mass*9.81*sin(teth))/mass;
            teth=atan(2*A*X+B);//dteth=(P*sin(alpha)+    cyacx.first*(alpha)*q*model->getS()-   mass*9.81*cos(teth))/(mass*V);
            dH=V*sin(teth);
            dX=V*cos(teth);
//std::cout<<"dv="<<dV<<" P="<<P<<" 2="<<cyacx.second*q*model->getS()<<" 3="<<mass*9.81*sin(teth)<<std::endl;
            if(V<0||H<0||(H>=indat.Hmax&&X>=indat.mstone))break;
            ballisticData.push_back(baldat(t,V,H,X,teth,mass,cyacx.second,cyacx.first,
                                               (masscenter-model->getXp(V/atmo->get_sound_sp(H),
                                               atmo->get_sound_sp(H),
                                               atmo->get_cinem_viscidity(H)))/model->getLength()
                                           ));
            if(t<enpar.t){
                mass-=enpar.masspsec;
                if(mass<massend)mass=massend;
                masscenter+=dmasscenter;
            }
            else {
                mass=massend;
                masscenter=massendcenter;
            }
/*
            std::cout<<std::setw(3)<<"Fm= "
                     <<std::setw(4)<<9.81*sin(teth)<<std::endl<<"T"
                    <<std::setw(4)<<ballisticData.back().t
                   <<std::setw(6)<<" V= "<<ballisticData.back().V
                  <<std::setw(5)<<" m= "<<ballisticData.back().mass
                  <<std::setw(6)<<" st= "<<ballisticData.back().stability
                 <<std::setw(5)<<" teth= "<<ballisticData.back().teth*180/M_PI
                <<std::setw(6)<<" H= "<<ballisticData.back().H
               <<std::setw(6)<<" X= "<<ballisticData.back().X<<std::endl
                 <<std::setw(6)<<" mc= "<<masscenter<<std::endl;
            std::cout<<"cy="<<cyacx.first<<" cx="<<cyacx.second<<std::endl;*/
        }
        std::cout<<"Xend="<<X<<" Hend="<<H<<" Vend="<<V<<std::endl<<std::endl;
        if(X>=indat.mstone && H>=indat.Hmax && V>=Vend){
            for(const auto& dat:ballisticData)
                std::cout<<"t= "<<std::setw(4)<<dat.t
                       <<std::setw(6)<<" V= "<<dat.V
                      <<std::setw(5)<<" m= "<<dat.mass
                      <<std::setw(6)<<" st= "<<std::setw(6)<<dat.stability
                                <<std::setw(5)<<" cya= "<<dat.cy
                                           <<std::setw(5)<<" cx= "<<dat.cx
                     <<std::setw(5)<<" teth= "<<dat.teth*180/M_PI
                    <<std::setw(6)<<" H= "<<dat.H
                   <<std::setw(6)<<" X= "<<dat.X<<std::endl;

            std::cout<<std::endl<<"length="<<model->getLength()<<std::endl
                    <<" englen="<<model->getEngineParams().Leng<<std::endl
                   <<" mass0="<<model->getmass()<<std::endl
                  <<" massend="<<model->getmassend()<<std::endl
                 <<" mc0="<<model->getmasscenter()<<std::endl
                <<" mcend="<<model->getmasscenterend()<<std::endl
               <<" balance0="<<model->getbalanceStart()<<std::endl;
            if(ballisticData.size())
                std::cout<<" balanceEnd="<<model->getbalanceEnd(
                               ballisticData[ballisticData.size()-1].V/
                                  atmo->get_sound_sp(ballisticData[ballisticData.size()-1].H),
                               atmo->get_sound_sp(ballisticData[ballisticData.size()-1].H),
                               atmo->get_cinem_viscidity(ballisticData[ballisticData.size()-1].H));

            zones zns;

            for(double teth=5*M_PI/180;teth<M_PI/2;teth+=5*M_PI/180){
                double M0=model->getmass();//масса
                engineparameters enpar=model->getEngineParams();
                double Vmax=0;
                V=35;//скорость
                H=y0;//высота
                X=0;//горизонт дальность
                double mass=M0;
                massend=model->getmassend();
                dV=0;dH=0;dX=0;
                for(double t=0;H>0;t+=1){//одна траектория по времени полета
                    V+=dV;
                    H+=dH;
                    X+=dX;
                    if(Vmax<V)Vmax=V;
                    else{
                        if(V>1500&&(V+dV)<1500){
                            zns[1500].push_back(point(X,H));
                        }
                        else if(V>1000&&(V+dV)<1000){
                            zns[1000].push_back(point(X,H));
                        }
                        else if(V>750&&(V+dV)<750){
                            zns[750].push_back(point(X,H));
                        }
                        else if(V>500&&(V+dV)<500){
                            zns[500].push_back(point(X,H));
                        }
                        else if(V>250&&(V+dV)<250){
                            zns[250].push_back(point(X,H));
                        }
                        else if(V<250)break;
                    }
                    q=atmo->get_density(H)*V*V/2;
                    cyacx= model->getCyaaCx(
                                V/atmo->get_sound_sp(H),
                                alpha*180/M_PI,
                                atmo->get_sound_sp(H),
                                atmo->get_cinem_viscidity(H),
                                t<enpar.t
                                );

                    P=(t<enpar.t)?enpar.Pvacuum:0.0;
                    dV=(P*cos(alpha)-cyacx.second*q*model->getS()-
                        mass*9.81*sin(teth))/mass;
                    dH=V*sin(teth);
                    dX=V*cos(teth);
                }
            }
            for(auto& z:zns){
                std::cerr<<z.first<<':'<<std::endl;
                for(auto&p:z.second)
                    std::cerr<<p.first<<' '<<p.second<<std::endl;
            }

            return OutputData{
                ballisticData,
                model->getheadData(),
                model->getEngineParams(),
                model->getmass(),
                model->getmassend(),
                model->getmasscenter(),
                model->getmasscenterend(),
                zns};
        }
        ballisticData.clear();

    }
    return OutputData();//не найден
}

RocketHeadData ballisticCalculator::getheadData(){
    if(model)return model->getheadData();
    return RocketHeadData();
}

std::vector<size_t> ballisticCalculator::state() const{
   std::vector<size_t>temp;
   if(model){
    temp=model->state();
    temp.push_back(indat.iscorrect());
   }
    return temp;
}

void ballisticCalculator::setInputData(const InputData &data){
    indat=data;
}


bool InputData::iscorrect()const{
   // std::cout<<"iscorrid"<<(Dmid>0 && Hmax>Hmin && Hmin>0 && Xmax>0 && mstone>0 && Vtar>0)<<std::endl;
    return /*Dmid>0 && */Hmax>Hmin && Hmin>0 && Xmax>mstone && mstone>0 && Vtar>0;
}

double InputData::LmaxC() const{
    return sqrt(Hmax*Hmax+mstone*mstone);
}

double InputData::TaverageC() const{
    return (Xmax-mstone)/Vtar;
}

double InputData::VavgC() const{
    return LmaxC()/TaverageC();
}

InputData::InputData(double Hmaximum, double Hminimum, double Xmaximum, double Vtarget, double milestone, material enMatShell, material enMatnozzle, material enMatbr, material enMatTz, fuel enFl, double dteth, double Pk, double Pa):
 //   Dmid{DmidMax},
    Hmax{Hmaximum},
    Hmin{Hminimum},
    Xmax{Xmaximum},
    Vtar{Vtarget},
    enmatShell{enMatShell},
    enmatnozzle{enMatnozzle},
    enmatbr{enMatbr},
    enmatTz{enMatTz},
    enfl{enFl},
    mstone{milestone},
    teth0{dteth},
    enPk{Pk},
    enPa{Pa}{
    if(!iscorrect())throw IDexception("incorrect input parameter");//заменить на подходящую
}

void ballisticCalculator::saveProject(std::string proFile) const
{
    if(indat.iscorrect()&&model){
       std::ofstream out(proFile);
       if(!out)throw error_reading_file(std::string("file ")+=proFile+=" is corrupted");
       out<<indat<<","<<*model;
    }
}

void ballisticCalculator::clear(bool isxplane)
{
    indat=InputData();
    model.reset(new RocketModel(isxplane));
}



std::ostream &operator<<(std::ostream &os, const InputData &indat){
    return os<<indatheader<<'{'
            <<indat.Hmax<<','
           <<indat.Hmin<<','
            <<indat.Vtar<<','
           <<indat.Xmax<<','
          <<indat.enPa<<','
         <<indat.enPk<<','
        <<indat.mstone<<','
       <<indat.teth0<<','
      <<indat.enmatShell<<','
     <<indat.enmatTz<<','
    <<indat.enmatbr<<','
    <<indat.enmatnozzle<<','
    <<indat.enPa<<','
    <<indat.enPk<<','
    <<indat.enfl<<'}';
}

std::istream &operator>>(std::istream &in, InputData &input){
    std::string name,header;
    char delim1{0},delim2{0},delim3{0},delim4{0},delim5{0},delim6{0},
         delim7{0},delim8{0},delim9{0},delim10{0},delim11{0},delim12{0},delim13{0},delim14{0},footer{0};
    int tmp{0};
    InputData indat;
    while((tmp=in.get())!=EOF && isspace(tmp));
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=indatheader){
        in.clear(std::ios::failbit);
        return in;
    }
    in>>indat.Hmax>>delim1
       >>indat.Hmin>>delim2
        >>indat.Vtar>>delim3
       >>indat.Xmax>>delim4
      >>indat.enPa>>delim5
     >>indat.enPk>>delim6
    >>indat.mstone>>delim7
   >>indat.teth0>>delim8
  >>indat.enmatShell>>delim9
 >>indat.enmatTz>>delim10
>>indat.enmatbr>>delim11
>>indat.enmatnozzle>>delim12
>>indat.enPa>>delim13
>>indat.enPk>>delim14
>>indat.enfl>>footer;
    if(!in)return in;
    if(delim1!=delim2||
            delim2!=delim3||
            delim3!=delim4||
            delim4!=delim5||
            delim5!=delim6||
            delim6!=delim7||
            delim7!=delim8||
            delim8!=delim9||
            delim9!=delim10||
            delim10!=delim11||
            delim11!=delim12||
            delim12!=delim13||
            delim13!=delim14||
            delim14!=',' || footer!='}'|| !indat.iscorrect()){
        in.clear(std::ios::failbit);
        return in;
    }
    input=indat;
    return in;
}


const char *InputData::IDexception::what() const _GLIBCXX_USE_NOEXCEPT{
    return msg.c_str();
};

