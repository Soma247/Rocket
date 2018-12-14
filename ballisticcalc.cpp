#include "ballisticcalc.h"
#include <iomanip>

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

bool ballisticCalculator::iscorrect(){return model&&
            atmo&&
            model->isheadcorrect()&&indat.iscorrect();
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

void ballisticCalculator::setNosecone(matherial math, double Dend, double len, double delta){
    if(model)
        model->setNosecone(math,Dend,len,delta);
}

void ballisticCalculator::setEngine(matherial mathShell, matherial mathbr, matherial mathnozzle, matherial mathtzp, fuel fuel, double fuelmass, double Pk, double Pa){
    if(model)
        model->setEngine(mathShell, mathbr, mathnozzle,mathtzp,
                         fuel, fuelmass, Pk, Pa);
}

void ballisticCalculator::addConoid(matherial math, double Dbegin, double Dend, double length, double delta){
    if(model)
        model->addConoid(math, Dbegin, Dend, length,delta);
}

void ballisticCalculator::insertConoid(matherial math, double Dbegin, double Dend, double length, double delta, size_t num){
    if(model)
        model->insertConoid(math,Dbegin,Dend,length,delta,num);
}

void ballisticCalculator::addplane(matherial math, double Xfromnose, double Broot, double Btip, double Croot, double Ctip, double Xtip, double Xrf, double Xrr, double Xtf, double Xtr, double H){
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

void ballisticCalculator::openProject(std::string proFile)
{

}

OutputData ballisticCalculator::calculate(){
    std::cout<<"calc started"<<std::endl;
    if(!iscorrect())throw std::runtime_error("bc:1");

    for(double i=0.25;i<0.8;i+=0.01){//подбор относительной массы топлива

        model->setEngine(indat.enmatShell,indat.enmatbr,
                         indat.enmatnozzle,indat.enmatTz,indat.enfl,
                         i*model->getheadmass()/(1-i),indat.enPk,indat.enPa);

        double y0=15;
        double x0=0;
        std::cout<<"teth0="<<indat.teth0*180/M_PI<<std::endl;
        double M0=model->getmass();//масса
        double A=(indat.Hmax-tan(indat.teth0)*indat.Xmax-y0)/(indat.Xmax*indat.Xmax);
        double B=tan(indat.teth0+6*M_PI/180);
        //double C=y0;
        double alpha=5*M_PI/180;

        engineparameters enpar=model->getEngineParams();
        std::vector<baldat> ballisticData;
        ballisticData.reserve(static_cast<size_t>(indat.Taverage)*2);
        double V=15;//скорость
        double H=y0;//высота
        double X=x0;//горизонт дальность
        double teth=B;//угол наклона
        double mass=M0;
        double dV{0},dteth{0},dH{0},dX{0};
        double q{0};
        double P{0};
        std::pair<double,double>cyacx;
        for(double t=0;t<indat.Taverage;t+=0.5){//одна траектория по времени полета
            V+=dV;
            H+=dH;
            X+=dX;
            teth+=dteth;
            if(t<enpar.t)mass-=enpar.masspsec;
            else mass=model->getmassend();
            q=atmo->get_density(H)*V*V/2;
            cyacx= model->getCyaaCx(
                        V/atmo->get_sound_sp(H),
                        alpha*180/M_PI,
                        atmo->get_sound_sp(H),
                        atmo->get_cinem_viscidity(H),
                        t<enpar.t
                        );

            P=(t<enpar.t)?enpar.Pvacuum:0.0;
            dV=(P*cos(alpha)-
                cyacx.second*q*model->getS()-
                mass*9.81*sin(teth))/mass;
            teth=atan(A*X+B);//dteth=(P*sin(alpha)+    cyacx.first*(alpha)*q*model->getS()-   mass*9.81*cos(teth))/(mass*V);
            dH=V*sin(teth);
            dX=V*cos(teth);
            if(V<0||H<0||X>=indat.Xmax)break;
            ballisticData.push_back(baldat{t,V,H,X,teth,mass});
        }
        if(X>=indat.Xmax && H>=indat.Hmax && V>=2*indat.Vtar){
            for(const auto& dat:ballisticData)
                std::cout<<std::setw(3)<<"t= "
                        <<std::setw(4)<<dat.t
                       <<std::setw(6)<<" V= "<<dat.V
                      <<std::setw(5)<<" m= "<<dat.mass
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
            // std::cout<<std::endl<<*model<<std::endl;
            return OutputData{

            };
        }
        ballisticData.clear();

    }
}

RocketHeadData ballisticCalculator::getheadData(){
    if(model && model->isheadcorrect()){
        RocketHeadData dat;
        std::vector<size_t>flags=model->state();
        dat.nconepar=model->getNCparams();
        for(size_t i=0;i<flags.at(1);++i)
            dat.conespar.push_back(model->getModuleParams(i));
        for(size_t i=0;i<flags.at(2);++i)
            dat.planespar.push_back(model->getPlaneParams(i));
        for(size_t i=0;i<flags.at(2);++i)
            dat.equipspar.push_back(model->geteqparams(i));
        return dat;
    }
}

std::vector<size_t> ballisticCalculator::state() const{
    return model? model->state() : std::vector<size_t>{};
}

void ballisticCalculator::setInputData(const InputData &data){
    indat=data;
}


bool InputData::iscorrect()const{
    std::cout<<"iscorrid"<<(Dmid>0 && Hmax>Hmin && Hmin>0 && Xmax>0 && mstone>0 && Vtar>0)<<std::endl;
    return Dmid>0 && Hmax>Hmin && Hmin>0 && Xmax>0 && mstone>0 && Vtar>0;
}

InputData::InputData(double DmidMax, double Hmaximum, double Hminimum, double Xmaximum, double Vtarget, double milestone, matherial enMatShell, matherial enMatnozzle, matherial enMatbr, matherial enMatTz, fuel enFl, double Pk, double Pa):
    Dmid{DmidMax},
    Hmax{Hmaximum},
    Hmin{Hminimum},
    Xmax{Xmaximum},
    Vtar{Vtarget},
    enmatShell{enMatShell},
    enmatnozzle{enMatnozzle},
    enmatbr{enMatbr},
    enmatTz{enMatTz},
    enfl{enFl},
    enPk{Pk},
    enPa{Pa},
    mstone{milestone}{
    if(!iscorrect())throw IDexception("incorrect input parameter");//заменить на подходящую
    Taverage=(Xmax-mstone)/Vtar;
    Lmax=sqrt(Hmax*Hmax+mstone*mstone)/(Taverage+7);
    Vavg=Lmax/Taverage;
}

void ballisticCalculator::saveProject(std::string proFile) const
{

}
