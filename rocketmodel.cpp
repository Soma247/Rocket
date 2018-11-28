#include "rocketmodel.h"





coneparam RocketModel::getNCparams() const{return pnosecone->getparams();}
coneparam RocketModel::getTailConeParams() const{return ptailcone->getparams();}
coneparam RocketModel::getModuleParams(size_t num)const{
    if(num>pconoids.size())throw std::out_of_range("moduleparams num is out of range");
    return pconoids[num].getparams();
}
engineparameters RocketModel::getEngineParams() const{return pengine->getparams();}
planeparams RocketModel::getPlaneParams(size_t num)const{
    if(num>pconoids.size())throw std::out_of_range("planeeparams num is out of range");
    return pplanes[num].getparams();
}
std::pair<double, double> RocketModel::geteqparams(size_t num)const{
    if(num>pequipments.size())throw std::out_of_range("eqparams num is out of range");
    return std::pair<double,double>(pequipments[num].getX0(),pequipments[num].getL());
}


void RocketModel::setNosecone(matherial math, double Dend, double len, double delta){
    pnosecone.reset(new nosecone(math,Dend,len,delta));
    update();
}
void RocketModel::setEngine(matherial mathShell, matherial mathbr, matherial mathnozzle, matherial mathtzp, fuel fuel, double fuelmass, double Pk, double Pa){

    pengine.reset(new engine(mathShell, mathbr,mathnozzle, mathtzp,fuel, Dmax, fuelmass, Pk, Pa));
    engineparameters par=pengine->getparams();
    ptailcone.reset(new conoid(mathShell,Dmax,par.Da,par.Leng-par.LforwardBottom-par.Lcyl,0.08));//конус обратный от сопла до миделя по длине сопла
    update();
}

void RocketModel::addConoid(matherial math, double Dbegin, double Dend, double length, double delta){
    pconoids.push_back(conoid{math,Dbegin,Dend,length,delta});
    update();
}
void RocketModel::insertConoid(matherial math, double Dbegin, double Dend, double length, double delta, size_t num){
    if(num>pconoids.size())
        pconoids.push_back(conoid(math,Dbegin,Dend,length,delta));
    else
        pconoids.insert(pconoids.begin()+num,conoid(math,Dbegin,Dend,length,delta));
    update();
}

void RocketModel::addplane(matherial math, std::string pname, double Xfromnose,
                           double Broot, double Btip, double Croot, double Ctip,
                           double Xtip, double Xrf, double Xrr, double Xtf,
                           double Xtr, double H){
    pplanes.push_back(plane(math,pname,Xfromnose,Broot,Btip,Croot,Ctip,
                                                 Xtip,Xrf,Xrr,Xtf,Xtr,H));
    update();
}
void RocketModel::addEquipment(std::string eqname, double X, double mass){
    pequipments.push_back(equipment(eqname,X,mass));
    update();
}



void RocketModel::ejectConoid(size_t num){
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

std::pair<double, double> RocketModel::getCxCyaa(double M, double alphagrad, double sound_sp, double cin_visc, bool engineisactive) const{
    if(M<1E-1)M=1E-1;
    double Cx0=getCx0(M,sound_sp,cin_visc,engineisactive);
    double Cya=getCya(M,sound_sp,cin_visc);
 //   std::cout<<"cx0="<<Cx0<<" Cya="<<Cya<<std::endl;
    return std::pair<double,double>(Cya-(Cx0/57.3),
                                    Cx0+alphagrad*alphagrad*(Cya+2*(0.1*(Dmax*sqrt(fabs(M*M-1))/Lnc)-0.2)/57.3)/57.3);
}


double RocketModel::getmass()const{
    double mass=pnosecone->mass()+pengine->mass()+ptailcone->mass();
    for(const conoid& c:pconoids)
        mass+=c.mass();
    for(const equipment& e:pequipments)
        mass+=e.mass();
    for(const plane& p:pplanes)
        mass+=p.mass();
    return mass;
}

double RocketModel::getmasscenter()const{
    double mx=pnosecone->mass()*pnosecone->massCenter()+
            pengine->mass()*(pengine->getX0()+pengine->massCenter())+
            ptailcone->mass()*(ptailcone->massCenter()+ptailcone->getX0());

    for(const conoid& c:pconoids)
        mx+=c.mass()*(c.massCenter()+c.getX0());
    for(const equipment& e:pequipments)
        mx+=e.mass()*(e.massCenter()+e.getX0());
    for(const plane& p:pplanes)
        mx+=p.mass()*(p.massCenter()+p.getX0());
    return mx/getmass();
}

double RocketModel::getmassend() const
{
    double mass=pnosecone->mass()+pengine->getmassend()+ptailcone->mass();
    for(const conoid& c:pconoids)
        mass+=c.mass();
    for(const equipment& e:pequipments)
        mass+=e.mass();
    for(const plane& p:pplanes)
        mass+=p.mass();
    return mass;
}

double RocketModel::getmasscenterend() const
{
    double mx=pnosecone->mass()*pnosecone->massCenter()+
            pengine->getmassend()*(pengine->getX0()+pengine->getmasscenterend())+
            ptailcone->mass()*(ptailcone->massCenter()+ptailcone->getX0());

    for(const conoid& c:pconoids)
        mx+=c.mass()*(c.massCenter()+c.getX0());
    for(const equipment& e:pequipments)
        mx+=e.mass()*(e.massCenter()+e.getX0());
    for(const plane& p:pplanes)
        mx+=p.mass()*(p.massCenter()+p.getX0());
    return mx/getmassend();
}

double RocketModel::getLength() const{
    double length=pnosecone->getL()+pengine->getL();
    for (const conoid& c:pconoids)
        length+=c.getL();
    return length;
}

double RocketModel::getCp(double M, bool engineisactive)const{

    double cp=pnosecone->getCp(Dmax,M);
    for(const conoid& c:pconoids)
        cp+=c.getCp(Dmax,M);

    engineparameters par=pengine->getparams();
    cp+=Aerodynamics::CxpSternBottom(ptailcone->getL(),Dmax,ptailcone->getDend(),ptailcone->getDend(),M,engineisactive);
    cp*=0.25*Dmax*Dmax*M_PI/SmidLA;
    for(const plane& p:pplanes)
        cp+=4*p.getCp(SmidLA,M);
    return cp;//отнесен ко всему ЛА
}

double RocketModel::getCxTr(double M, double sound_sp, double cin_visc)const{
    double cxt=Aerodynamics::CxTrF(SmidLA,Dmax,Lnc,Lcyl,ptailcone->getL(),ptailcone->getDend(),M,sound_sp,cin_visc);
    for(const plane& p:pplanes)
        cxt+=p.getCxtr(SmidLA,M,sound_sp,cin_visc);
    return cxt;//отнесен ко всему ЛА
}

double RocketModel::getCya(double M, double sound_sp, double cin_visc) const{
    double k=isxplane?4/sqrt(2.0):2;
    double cya=Aerodynamics::CyaNoseCyll(Dmax,SmidLA,Lnc,Lcyl,M);
    for(const plane& p:pplanes)
        cya+=k*p.getcyaConsole(Dmax,SmidLA,M,sound_sp,cin_visc);
    return cya;
}

double RocketModel::getCx0(double M, double sound_sp, double cin_visc, bool engineisactive) const{
    return getCp(M,engineisactive)+getCxTr(M,sound_sp,cin_visc);
}

void RocketModel::update(){
    if(pnosecone && pengine && ptailcone){
        double L=pnosecone->getL();
        int i=1;
        Lnc=pnosecone->getL();
        pnosecone->setname(std::to_string(i++));
        for(conoid& c:pconoids){
            c.setX0(L);
            if(c.getDend()<Dmax)Lnc=L;
            c.setname(std::to_string(i++));
            L+=c.getL();
        }
        pengine->setX0(L);

        pengine->setname(std::to_string(++i));
        engineparameters par=pengine->getparams();
        ptailcone->setX0(L+par.LforwardBottom+par.Lcyl);
        ptailcone->setname(std::to_string(++i));
        double Smidplanes=0;
        for(const plane& p:pplanes)
            Smidplanes+=p.S();
        SmidLA=Smidplanes+0.25*M_PI*Dmax*Dmax;
        Lcyl=getLength()-Lnc-ptailcone->getL();


    }
}

