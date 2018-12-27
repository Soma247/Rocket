#include "rocketmodel.h"
#include <sstream>
const std::string rocketmodelheader{"RocketModel"};

RocketHeadData RocketModel::getheadData(){
    RocketHeadData data;
    if(pnosecone){
        data.nconepar=pnosecone->getparams();
        data.headLen+=data.nconepar.len;
    }
    if(ptailplane)data.tailstabpar=ptailplane->getparams();
    for(auto&c:pconoids){
        data.conespar.push_back(c.getparams());
        data.headLen+=c.getL();
    }
    for(auto&p:pplanes)
        data.planespar.push_back(p.getparams());
    for(size_t i=0;i<pequipments.size();++i)
        data.equipspar.push_back(geteqparams(i));
    data.headDend=Dengine;
    data.headMass=getheadmass();
    data.headDmax=Dmax;
    return data;
}

coneparam RocketModel::getNCparams() const{return pnosecone?pnosecone->getparams():coneparam();}
coneparam RocketModel::getTailConeParams() const{return ptailcone?ptailcone->getparams():coneparam();}
coneparam RocketModel::getModuleParams(size_t num)const{
    if(num>pconoids.size())throw std::out_of_range("moduleparams num is out of range");
    return pconoids[num].getparams();
}
engineparameters RocketModel::getEngineParams() const{return pengine?pengine->getparams():engineparameters();}
planeparams RocketModel::getPlaneParams(size_t num)const{
    if(num>pconoids.size())throw std::out_of_range("planeeparams num is out of range");
    return pplanes[num].getparams();
}

equipmentparameters RocketModel::geteqparams(size_t num)const{
    if(num>pequipments.size())throw std::out_of_range("eqparams num is out of range");
    return {pequipments[num].getname(),
                pequipments[num].massCenter(),
                pequipments[num].mass()};
}


void RocketModel::setNosecone(material math, double Dend, double len, double delta){
    pnosecone.reset(new nosecone(math,Dend,len,delta));
    update();
}

void RocketModel::setTailStab(material math, double Xfromnose, double Broot, double Btip, double Croot, double Ctip, double Xtip, double Xrf, double Xrr, double Xtf, double Xtr, double H)
{
    ptailplane.reset(new plane(math,Xfromnose,Broot,Btip,Croot,Ctip,
                                     Xtip,Xrf,Xrr,Xtf,Xtr,H));
    update();
}
void RocketModel::setEngine(material mathShell, material mathbr,
                            material mathnozzle, material mathtzp,
                            fuel fuel, double fuelmass, double Pk, double Pa){
    pengine.reset(new engine(mathShell, mathbr,mathnozzle,
                             mathtzp,fuel, Dengine, fuelmass, Pk, Pa));
    engineparameters par=pengine->getparams();
    ptailcone.reset(new conoid(mathShell,Dengine,par.Da,
                               par.Leng-par.LforwardBottom-par.Lcyl,0.08));
    //конус обратный от сопла до миделя по длине сопла
    update();
}

void RocketModel::addConoid(material math, double Dbegin, double Dend,
                            double length, double delta){
    pconoids.push_back(conoid{math,Dbegin,Dend,length,delta});
    update();
}
void RocketModel::insertConoid(material math, double Dbegin,
                               double Dend, double length, double delta, size_t num){
    if(num>pconoids.size())
        pconoids.push_back(conoid(math,Dbegin,Dend,length,delta));
    else
        pconoids.insert(pconoids.begin()+num,conoid(math,Dbegin,Dend,length,delta));
    update();
}

void RocketModel::addplane(material math, double Xfromnose,
                           double Broot, double Btip, double Croot, double Ctip,
                           double Xtip, double Xrf, double Xrr, double Xtf,
                           double Xtr, double H){
    pplanes.push_back(plane(math,Xfromnose,Broot,Btip,Croot,Ctip,
                                                 Xtip,Xrf,Xrr,Xtf,Xtr,H));
    update();
}
void RocketModel::addEquipment(std::string eqname, double X, double mass){
   // std::cout<<"romodel:add eq"<<std::endl;
    pequipments.push_back(equipment(eqname,X,mass));
    update();
}


void RocketModel::ejectConoid(size_t num){
  //  std::cerr<<"RM:eject "<<num<<std::endl;
    if(num>pconoids.size())throw std::out_of_range("conoid num is out of range");
    pconoids.erase(pconoids.begin()+num);
    update();
}
void RocketModel::ejectPlane(size_t num){
    if(num>pplanes.size())throw std::out_of_range("plane num is out of range");
    pplanes.erase(pplanes.begin()+num);
    update();
}
void RocketModel::ejectEquipment(size_t num){
    if(num>pequipments.size())throw std::out_of_range("eq num is out of range");
    pequipments.erase(pequipments.begin()+num);
    update();
}

std::pair<double, double> RocketModel::getCyaaCx(double M, double alphagrad, double sound_sp, double cin_visc, bool engineisactive) const{
    if(M<1E-1)M=1E-1;
    double Cx0=getCx0(M,sound_sp,cin_visc,engineisactive);
    double Cya=getCya(M,sound_sp,cin_visc);
    return std::pair<double,double>(Cya-(Cx0/57.3),
                                    Cx0+alphagrad*alphagrad*(Cya+2*(0.1*(Dengine*sqrt(fabs(M*M-1))/Lnc)-0.2)/57.3)/57.3);
}

double RocketModel::getXp(double M, double sound_sp, double cin_visc) const{
    if(M<0.5)M=0.5;
    double cyan=Aerodynamics::CyaNoseCyll(Dmax,SmidLA,Lnc,Lcyl,M)*0.25*Dmax*Dmax*M_PI/SmidLA;
    double xpf=Lnc*(2/3.0+Aerodynamics::xpf(Dmax,Lnc,Lcyl,M))*cyan;
    double k=isxplane?2/sqrt(2):1;
    double cyap=0;
    for(const plane& p:pplanes){
        planeparams par=p.getparams();
        xpf+=k*2*p.getcyaConsole(Dmax,SmidLA,M,sound_sp,cin_visc)*p.getXp(M);
        cyap+=k*2*p.getcyaConsole(Dmax,SmidLA,M,sound_sp,cin_visc);
    }

    if(ptailplane){
        planeparams par=ptailplane->getparams();
        xpf+=k*2*ptailplane->getcyaConsole(Dmax,SmidLA,M,sound_sp,cin_visc)*ptailplane->getXp(M);
        cyap+=k*2*ptailplane->getcyaConsole(Dmax,SmidLA,M,sound_sp,cin_visc);
    }
    return xpf/(cyan+cyap);
}




double RocketModel::getheadmass() const
{

    double mass{0};
    if(pnosecone)
        mass+=pnosecone->mass();
    if(ptailplane)
        mass+=ptailplane->mass();
    for(const conoid& c:pconoids)
        mass+=c.mass();
    for(const equipment& e:pequipments)
        mass+=e.mass();
    for(const plane& p:pplanes)
        mass+=p.mass();
    return mass;
}

double RocketModel::getmass()const{
    double mass{0};
    if(pnosecone)
        mass+=pnosecone->mass();
    if(pengine)
        mass+=pengine->mass();
    if(ptailcone)
        mass+=ptailcone->mass();
    if(ptailplane)
        mass+=ptailplane->mass();
    for(const conoid& c:pconoids)
        mass+=c.mass();
    for(const equipment& e:pequipments)
        mass+=e.mass();
    for(const plane& p:pplanes)
        mass+=p.mass();
    return mass;
}

double RocketModel::getmassend() const{
    double mass{0};
    if(pnosecone)
        mass+=pnosecone->mass();
    if(pengine)
        mass+=pengine->getmassend();
    if(ptailcone)
        mass+=ptailcone->mass();
    for(const conoid& c:pconoids)
        mass+=c.mass();
    for(const equipment& e:pequipments)
        mass+=e.mass();
    for(const plane& p:pplanes)
        mass+=p.mass();
    if(ptailplane)
        mass+=ptailplane->mass();
    return mass;
}

double RocketModel::getmasscenter()const{

    double mx{0};
    if(pnosecone)
        mx+=pnosecone->mass()*pnosecone->massCenter();
    if(pengine)
        mx+=pengine->mass()*(pengine->getX0()+pengine->massCenter());
    if(ptailcone)
        mx+=ptailcone->mass()*(ptailcone->massCenter()+ptailcone->getX0());

    for(const conoid& c:pconoids)
        mx+=c.mass()*(c.massCenter()+c.getX0());
    for(const equipment& e:pequipments)
        mx+=e.mass()*(e.massCenter()+e.getX0());
    for(const plane& p:pplanes)
        mx+=p.mass()*(p.massCenter()+p.getX0());
    if(ptailplane)
        mx+=ptailplane->mass()*(ptailplane->massCenter());
    return mx/getmass();
}



double RocketModel::getmasscenterend() const{
    double mx{0};
    if(pnosecone)
        mx+=pnosecone->mass()*pnosecone->massCenter();
    if(pengine)
        mx+=pengine->getmassend()*(pengine->getX0()+pengine->getmasscenterend());
  //  std::cout<<"peng mc:"<<pengine->getmasscenterend()<<std::endl;
    if(ptailcone)
        mx+=ptailcone->mass()*(ptailcone->massCenter()+ptailcone->getX0());
    for(const conoid& c:pconoids)
        mx+=c.mass()*(c.massCenter()+c.getX0());
    for(const equipment& e:pequipments)
        mx+=e.mass()*(e.massCenter()+e.getX0());
    for(const plane& p:pplanes)
        mx+=p.mass()*(p.massCenter()+p.getX0());
    if(ptailplane)
        mx+=ptailplane->mass()*(ptailplane->massCenter());
    return mx/getmassend();
}

double RocketModel::getLength() const{
    double length{0};
    if(pnosecone)
        length+=pnosecone->getL();
    if(pengine)
        length+=pengine->getL();
    for (const conoid& c:pconoids)
        length+=c.getL();
    return length;
}

double RocketModel::getCp(double M, bool engineisactive)const{
    if(!pnosecone||!pengine)throw std::logic_error("model uncomplete");

    double cp=pnosecone->getCp(pnosecone->getDend(),M);
 //   std::cout<<"getcp0000="<<cp<<std::endl;
    for(const conoid& c:pconoids)
        cp+=c.getCp(c.getDend(),M);
   // std::cout<<"getcp000="<<cp<<std::endl;
    engineparameters par=pengine->getparams();
    cp+=Aerodynamics::CxpSternBottom(ptailcone->getL(),Dengine,ptailcone->getDend(),ptailcone->getDend(),M,engineisactive);
//    std::cout<<"getcpstb="<<Aerodynamics::CxpSternBottom(ptailcone->getL(),Dengine,ptailcone->getDend(),ptailcone->getDend(),M,engineisactive)<<std::endl;
    cp*=0.25*Dmax*Dmax*M_PI/SmidLA;

   // std::cout<<"getcp0="<<cp<<std::endl;
    for(const plane& p:pplanes)
        cp+=4*p.getCp(SmidLA,M);
    //std::cout<<"getcp1="<<cp<<std::endl;
    if(ptailplane)
        cp+=4*ptailplane->getCp(SmidLA,M);
  //  std::cout<<"getcp2="<<cp<<std::endl;
    return cp;//отнесен ко всему ЛА
}

double RocketModel::getCxTr(double M, double sound_sp, double cin_visc)const{
    double cxt=Aerodynamics::CxTrF(SmidLA,Dmax,Lnc,Lcyl,ptailcone->getL(),ptailcone->getDend(),M,sound_sp,cin_visc);
    for(const plane& p:pplanes)
        cxt+=p.getCxtr(SmidLA,M,sound_sp,cin_visc);
    if(ptailplane)
        cxt+=ptailplane->getCxtr(SmidLA,M,sound_sp,cin_visc);
    return cxt;//отнесен ко всему ЛА
}

double RocketModel::getCya(double M, double sound_sp, double cin_visc) const{
    double k=isxplane?4/sqrt(2.0):2;
    double cya=Aerodynamics::CyaNoseCyll(Dmax,SmidLA,Lnc,Lcyl,M);
    for(const plane& p:pplanes)
        cya+=k*p.getcyaConsole(Dengine,SmidLA,M,sound_sp,cin_visc);
    if(ptailplane)
        cya+=k*ptailplane->getcyaConsole(Dengine,SmidLA,M,sound_sp,cin_visc);
    return cya;
}

double RocketModel::getCx0(double M, double sound_sp, double cin_visc, bool engineisactive) const{
    return 0.5*getCp(M,engineisactive)+getCxTr(M,sound_sp,cin_visc);//*0.5 корр коэфф
}

void RocketModel::setscalePlane(size_t num, double scale){
    if(num>=pplanes.size())throw std::out_of_range("rm::setscaleplane out of range");
    planeparams par=pplanes[num].getparams();
    std::string name=pplanes[num].getname();
    pplanes[num]=plane(par.mat,
                       num==pplanes.size()-1?getLength()-par.broot*scale:par.XfromNose,
                       par.broot*scale,
                       par.btip*scale,
                       par.croot*scale,
                       par.ctip*scale,
                       par.xtip*scale,
                       par.xrf*scale,
                       par.xrr*scale,
                       par.xtf*scale,
                       par.xtr*scale,
                       par.h*scale);
    update();
}

double RocketModel::getbalanceEnd(double M, double sound_sp, double cin_visc){
    return (getmasscenterend()-getXp(M,sound_sp,cin_visc))/getLength();
}

double RocketModel::getbalanceStart(double M, double sound_sp, double cin_visc){
    return (getmasscenter()-getXp(M,sound_sp,cin_visc))/getLength();
}

std::vector<size_t> RocketModel::state() const{/*
    std::cout<<"rm:conoids ";
    for(auto&c:pconoids)
        std::cout<<c.getL()<<" ";
    std::cout<<std::endl;
*/


    return {
             bool(pnosecone),
             bool(ptailplane),
             pconoids.size(),
             pplanes.size(),
             pequipments.size()
            };
}

void RocketModel::update(){
    if(pconoids.size())
        Dengine=pconoids.back().getDend();
    else if(pnosecone)
        Dengine=pnosecone->getDend();
    else Dengine=0;
    Dmax=Dengine;
    for(conoid& c:pconoids)
    if(c.getDend()>Dmax)
            Dmax=c.getDend();

    double Lhead=Lnc=pnosecone?pnosecone->getL():0;
    int i=1;
    for(conoid& c:pconoids){
        c.setX0(Lhead);
        if(c.getDend()<Dmax){
            Lnc+=c.getL();
        }
        c.setname(std::string("module ")+=std::to_string(i++));
        Lhead+=c.getL();
    }

    if(pnosecone && pengine && ptailcone){  
        pengine->setX0(Lhead);
        pengine->setname(std::to_string(++i));
        engineparameters par=pengine->getparams();
        ptailcone->setX0(Lhead+par.LforwardBottom+par.Lcyl);
        ptailcone->setname(std::to_string(++i));
        double Smidplanes=0;
        for(const plane& p:pplanes)
            Smidplanes+=p.Smid();
        if(ptailplane){
            Smidplanes+=ptailplane->Smid();
            ptailplane->setX0(getLength()-ptailplane->getL());
        }
        SmidLA=Smidplanes+0.25*M_PI*Dmax*Dmax;
        Lcyl=getLength()-Lnc-ptailcone->getL();
    }
}





std::ostream& operator<<(std::ostream &os, const RocketModel& rm){
    os<<rocketmodelheader<<'{'
                   <<rm.Dengine<<','
               <<rm.isxplane<<','
        <<bool(rm.pnosecone)<<','
          <<bool(rm.ptailplane)<<','
          <<bool(rm.pengine)<<','
         <<rm.pconoids.size()<<','
        <<rm.pplanes.size()<<','
       <<rm.pequipments.size();

    if(rm.pnosecone)
        os<<','<<*rm.pnosecone;
    if(rm.ptailplane)
        os<<','<<*rm.ptailplane;
    if(rm.pengine)
        os<<','<<*rm.pengine;
    for(const conoid& c:rm.pconoids)
        os<<','<<c;
    for(const plane& p:rm.pplanes)
        os<<','<<p;
    for(const equipment& eq:rm.pequipments)
        os<<','<<eq;
    return os<<'}';
}

std::istream &operator>>(std::istream &is, RocketModel &rm){
    std::string header;
    material mat;
    int tmp{0};
    size_t pconesize{0},pplansize{0},peqsize{0};
    char delim{0},delim1{0},delim2{0},delim3{0},delim4{0},delim5{0},
    delim6{0},delim7{0},footer{0};
    double Dmax{0};
    bool isxplane{true},pncone{false},ptplane{false},pengn{false};
    std::unique_ptr<nosecone> nc;
    std::unique_ptr<plane>tp;
    std::unique_ptr<engine>en;




    while((tmp=is.get())!=EOF && isspace(tmp));
    is.unget();
    while((tmp=is.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

std::cout<<"read header"<<std::endl;

    if(tmp==EOF)return is;
    if(header!=rocketmodelheader){
        is.clear(std::ios::failbit);
        return is;
    }
std::cout<<"read rocket params"<<std::endl;
    is>>Dmax>>delim1
       >>isxplane>>delim2
            >>pncone>>delim3
            >>ptplane>>delim4
            >>pengn>>delim5
            >>pconesize>>delim6
            >>pplansize>>delim7
            >>peqsize;

    if(!is)return is;
    if(delim1!=delim2 ||
            delim2!=delim3||
            delim3!=delim4||
            delim4!=delim5||
            delim5!=delim6||
            delim6!=delim7||
            delim7!=','||Dmax<0){
        is.clear(std::ios::failbit);
        return is;
    }
    std::vector<conoid>pcon(pconesize);
    std::vector<plane>planes(pplansize);
    std::vector<equipment>equip(peqsize);

std::cout<<"read pcone"<<std::endl;
    if(pncone){
        nc.reset(new nosecone);
        is>>delim>>*nc;
        if(!is)return is;
        if(delim!=','){std::cout<<"delim is:"<<delim<<"("<<int(delim)<<")"<<std::endl;is.clear(std::ios::failbit);return is;}
    }
    std::cout<<"read ptplane"<<std::endl;
        if(ptplane){
            tp.reset(new plane);
            is>>delim>>*tp;
            if(!is)return is;
            if(delim!=','){std::cout<<"delim is:"<<delim<<"("<<int(delim)<<")"<<std::endl;is.clear(std::ios::failbit);return is;}
        }
std::cout<<"read en"<<std::endl;
    if(pengn){
        en.reset(new engine);
        is>>delim>>*en;
        if(!is)return is;
        if(delim!=','){is.clear(std::ios::failbit);return is;}
    }

std::cout<<"read conoids"<<std::endl;
        for(size_t i=0;i<pconesize;i++){
            is>>delim>>pcon[i];
            if(!is)return is;
            if(delim!=','){is.clear(std::ios::failbit);return is;}
        }
std::cout<<"read planes"<<std::endl;
        for(size_t i=0;i<pplansize;i++){
            is>>delim>>planes[i];
            if(!is)return is;
            if(delim!=','){is.clear(std::ios::failbit);return is;}
        }
        std::cout<<"read eqs"<<std::endl;
        for(size_t i=0;i<peqsize;i++){
            is>>delim>>equip[i];
            if(!is)return is;
            if(delim!=','){is.clear(std::ios::failbit);return is;}
        }
        is>>footer;
        if(!is)return is;
        if(footer!='}'){is.clear(std::ios::failbit);return is;}

        rm.Dengine=Dmax;
        rm.isxplane=isxplane;
        if(pncone)rm.pnosecone.reset(nc.release());
        if(ptplane)rm.ptailplane.reset(tp.release());
        if(pengn)rm.pengine.reset(en.release());
        if(rm.pengine)rm.Dengine=rm.pengine->getDend();
        else rm.Dengine=rm.Dmax;
        rm.pconoids=pcon;
        rm.pplanes=planes;
        rm.pequipments=equip;
        rm.update();
    return is;
}
