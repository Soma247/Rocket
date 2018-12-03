#include "ballisticcalc.h"
#include <iomanip>

bool RocketCalculator::iscorrect(){return model&&
            atmo&&
            model->isheadcorrect()&&indat.iscorrect();
                                  }

void RocketCalculator::setModel(std::unique_ptr<RocketModel> rm){
    model.reset(rm.release());
}

void RocketCalculator::setModel(RocketModel *rm){
    model.reset(rm);
}

void RocketCalculator::setAtmosphere(std::unique_ptr<AbstractAtmosphere> aa){
    atmo.reset(aa.release());
}

void RocketCalculator::setAtmosphere(AbstractAtmosphere *aa){
    atmo.reset(aa);
}
struct baldat{
    double t=0;
    double V=0;//скорость
    double H=0;//высота
    double X=0;//горизонт дальность
    double teth=0;//угол наклона
    double mass=0;
};

void RocketCalculator::calculate(){
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

        double V=15;//скорость
        double H=y0;//высота
        double X=x0;//горизонт дальность
        double teth=B;//угол наклона
        double mass=M0;
        double dV{0},dteth{0},dH{0},dX{0};
        double q{0};
        double P{0};
        std::pair<double,double>cyacx;

        std::cout<<"second c"<<std::endl;
        for(double t=0;t<indat.Taverage;t+=0.5){//одна траектория по времени полета

            V+=dV;
            H+=dH;
            X+=dX;
            teth+=dteth;
            if(t<enpar.t)mass-=enpar.masspsec;
            else mass=model->getmassend();

            /*      std::cout<<"t= "<<t<<" V= "<<V<<" m= "<<mass
                        <<" teth= "<<teth*180/M_PI
                       <<" H= "<<H
                      <<" X= "<<X<<std::endl;*/


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
            //   std::cout<<"cx="<<cyacx.second<<"dens="<<atmo->get_density(H)<<std::endl;
            teth=atan(A*X+B);//dteth=(P*sin(alpha)+    cyacx.first*(alpha)*q*model->getS()-   mass*9.81*cos(teth))/(mass*V);
            dH=V*sin(teth);
            dX=V*cos(teth);
            /*     std::cout<<" dV= "<<dV<<" dm= "<<enpar.masspsec
                        <<" dteth= "<<dteth*180/M_PI
                       <<" dH= "<<dH
                      <<" dX= "<<dX<<std::endl<<std::endl;*/

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
                     <<" mcend="<<model->getmasscenterend()<<std::endl;
            return;
        }
        ballisticData.clear();

    }
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
