#include "ballisticcalc.h"
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

OutputData ballisticCalculator::calculate(){
    if(!model||!iscorrect())throw std::runtime_error("bc:1");

    double y0=15;
    double x0=0;
    double teth0=indat.teth0+atan((indat.Hmax-y0)/(indat.mstone));
    if(teth0>M_PI/2)teth0=atan((indat.Hmax-y0)/indat.mstone);

    for(double i=0.3;i<0.95;i+=0.01){//подбор относительной массы топлива

        model->setEngine(indat.enmatShell,indat.enmatbr,
                         indat.enmatnozzle,indat.enmatTz,indat.enfl,
                         i*model->getheadmass()/(1-i),indat.enPk,indat.enPa);

        double M0=model->getmass();//масса
        double A=(indat.Hmax-tan(teth0)*indat.mstone-y0)/(indat.mstone*indat.mstone);
        double B=tan(teth0);
        engineparameters enpar=model->getEngineParams();
        std::vector<baldat> ballisticData;
        ballisticData.reserve(static_cast<size_t>(indat.TaverageC()));
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


        for(double t=0;t<=indat.TaverageC();t+=indat.dt){//одна траектория по времени полета
            V+=dV;
            H+=dH;
            X+=dX;
            q=atmo->get_density(H)*V*V/2;
            cyacx= model->getCyaaCx(
                        V/atmo->get_sound_sp(H),
                        indat.alpha*180/M_PI,
                        atmo->get_sound_sp(H),
                        atmo->get_cinem_viscidity(H),
                        t<enpar.t
                        );

            P=(t<enpar.t)?enpar.Pvacuum:0.0;

            dV=(P*cos(indat.alpha)-cyacx.second*q*model->getS()-
                mass*9.81*sin(teth))*indat.dt/mass;

            teth=atan(2*A*X+B);//dteth=(P*sin(alpha)+    cyacx.first*(alpha)*q*model->getS()-   mass*9.81*cos(teth))/(mass*V);
            dH=V*sin(teth)*indat.dt;
            dX=V*cos(teth)*indat.dt;

            if(V<0||H<0||(H>=indat.Hmax&&X>=indat.mstone)||t>indat.TaverageC())break;
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
        }
        if(X>=indat.mstone && H>=indat.Hmax && V>=indat.Vend){
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
                                indat.alpha*180/M_PI,
                                atmo->get_sound_sp(H),
                                atmo->get_cinem_viscidity(H),
                                t<enpar.t
                                );

                    P=(t<enpar.t)?enpar.Pvacuum:0.0;
                    dV=(P*cos(indat.alpha)-cyacx.second*q*model->getS()-
                        mass*9.81*sin(teth))/mass;
                    dH=V*sin(teth);
                    dX=V*cos(teth);
                }
            }

            /*struct OutputData{
    std::vector<baldat>traect;
    RocketHeadData headData;
    engineparameters engpar;
    double massfull;
    double massempty;
    double xmfull;
    double xmempty;
    double len;
    zones zns;
    bool iscorrect()const;
};*/
            return OutputData(
                ballisticData,
                        model->getheadData(),
                        model->getEngineParams(),
                        model->getTailConeParams(),
                        model->getmass(),
                        model->getmassend(),
                        model->getmasscenter(),
                        model->getmasscenterend(),
                        model->getLength(),
                        zns);
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
    return  Hmax>Hmin &&
            Hmin>0 &&
            Xmax>mstone
            && mstone>0
            && Vtar>0
            && alpha>0
            &&alpha<(45*M_PI/180)
            && Vend>0&&
            dt>0&&dt<TaverageC();
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

InputData::InputData(double Hmaximum, double Hminimum, double Xmaximum, double Vtarget, double milestone, double VendMin, double alphaRad, material enMatShell, material enMatnozzle, material enMatbr, material enMatTz, fuel enFl, double dteth, double Pk, double Pa, double deltat):
    //   Dmid{DmidMax},
    Hmax{Hmaximum},
    Hmin{Hminimum},
    Xmax{Xmaximum},
    Vtar{Vtarget},
    mstone{milestone},
    teth0{dteth},
    Vend{VendMin},
    alpha{alphaRad},
    enmatShell{enMatShell},
    enmatnozzle{enMatnozzle},
    enmatbr{enMatbr},
    enmatTz{enMatTz},
    enfl{enFl},
    enPk{Pk},
    enPa{Pa},
    dt{deltat}{
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
        <<indat.Vend<<','
        <<indat.alpha<<','
        <<indat.enmatShell<<','
        <<indat.enmatTz<<','
        <<indat.enmatbr<<','
        <<indat.enmatnozzle<<','
        <<indat.enPa<<','
        <<indat.enPk<<','
        <<indat.enfl<<','
        <<indat.dt<<'}';
    }

    std::istream &operator>>(std::istream &in, InputData &input){
        std::string name,header;
        char delim1{0},delim2{0},delim3{0},delim4{0},delim5{0},delim6{0},
        delim7{0},delim8{0},delim9{0},delim10{0},delim11{0},delim12{0},delim13{0},delim14{0},delim15{0},
        delim16{0},delim17{0},footer{0};
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
                >>indat.Vend>>delim9
                >>indat.alpha>>delim10
                >>indat.enmatShell>>delim11
                >>indat.enmatTz>>delim12
                >>indat.enmatbr>>delim13
                >>indat.enmatnozzle>>delim14
                >>indat.enPa>>delim15
                >>indat.enPk>>delim16
                >>indat.enfl>>delim17
                >>indat.dt>>footer;
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
                delim14!=delim15||
                delim15!=delim16||
                delim16!=delim17||
                delim17!=',' || footer!='}'|| !indat.iscorrect()){
            in.clear(std::ios::failbit);
            return in;
        }
        input=indat;
        return in;
    }


    const char *InputData::IDexception::what() const _GLIBCXX_USE_NOEXCEPT{
        return msg.c_str();
    };


    bool OutputData::iscorrect() const{
        return traect.size()&&
                zns.size()&&
                headData.iscorrect()&&
                massfull>0&&
                massempty>0&&
                xmfull>0&&
                xmempty>0;
    }

    baldat::baldat(double time, double Vel, double High, double Xi, double tetha, double massi, double cxi, double cyi, double stabi):
        t{time},V{Vel},H{High},X{Xi},teth{tetha},mass{massi},cx{cxi},cy{cyi},stability{stabi}{}

    ballisticCalculator::error_reading_file::error_reading_file(std::string mes):str{mes}{}
