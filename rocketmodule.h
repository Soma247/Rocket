#ifndef ROCKETMODULE_H
#define ROCKETMODULE_H
#include <string>
#include <math.h>

struct matherial{
    std::string name="air";
    //matherial parameters
    double Ro=1;//плотность
    double SigmaV=0;//предел прочности
    matherial(){}
    matherial(const std::string& matherialName, double Ro, double sigma):name{matherialName},Ro{Ro},SigmaV{sigma}{}
    matherial(const matherial&mat){
        name=mat.name;
        Ro=mat.Ro;
        SigmaV=mat.SigmaV;
    }
};

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
    double U(double pk){
        return Ukoef*pow(pk,Upow);
    }
    fuel(){}
    fuel(const fuel& fl):name{fl.name},ALpercent{fl.ALpercent},PudST{fl.PudST},
        roT{fl.roT},Tst{fl.Tst},Rst{fl.Rst},kst{fl.kst},Ukoef{fl.Ukoef},
        Upow{fl.Upow}
    {

    }

};

struct engineparameters{
    double PprivUdSt=0;
    double PpUd=0;
    double Tk=0;
    double PpUdVoid=0;//удельный импульс тяги в пустоте
    double Dfuel=0;
    double Dv=0;
    double t=0;//время работы двигателя 0.4D/u,
    double masspsec=0;
    double Pvacuum=0;//потребная тяга в пустоте
    double Dcr=0;//диаметр критического сечения сопла
    double Da=0;//диаметр выходного сечения сопла
    double La=0;//полная длина сопла
    double Ly=0;//длина утопленной части сопла
    double Lc=0;//длина выступающей части сопла
    double Dc=0;//диаметр сопла в месте выхода из камеры сгорания
    double LforwardBottom=0;//длины переднего и заднего днищ
    double LbackwardBottom=0;
    double Lcyl=0;
    double Leng=0;//полная длина двигателя
    double mbottoms=0;
    double mcyl=0;
    double mbr=0;
    double mnozzle=0;
    double mtz=0;
    double muk=0;
    double mengine=0;
};


class abstractRocketElement{
public:
    virtual double S()const=0;//S площадь миделя элемента
    virtual double mass()const=0;
    virtual double massCenter()const=0;

    virtual double getX0()const=0;
    virtual void setX0(double x)=0;

    virtual double getL()const=0;
    virtual void setL(double L)=0;

    virtual std::string getname()const=0;
    virtual void setname(std::string n)=0;
    virtual std::string toString()const;
    virtual ~abstractRocketElement(){}
};

class equipment:public abstractRocketElement{
public:
    virtual double S()const override{return 0;}//S площадь миделя элемента

    virtual double mass()const override{return eqmass;}
    virtual void setmass(double m){eqmass=m;}

    virtual double massCenter()const override;
    virtual void setmassCenter(double xm){Xm=xm;}

    virtual double getX0()const override{return 0;}//заглушка
    virtual void setX0(double)override{}//заглушка

    virtual double getL()const override{return 0;}//заглушка
    virtual void setL(double)override{}//заглушка

    virtual std::string getname()const override{return name;}
    virtual void setname(std::string n)override{name=n;}

    virtual std::string toString()const override;
    virtual ~equipment()override{}
    equipment(){}
    equipment(double X, double mass, std::string eqname);
protected:
    double eqmass=0;
    double Xm=0;
    std::string name;
};

class Module:public abstractRocketElement{
public:
    virtual double S()const override=0;//S площадь миделя элемента
    virtual double mass()const override=0;
    virtual double massCenter()const override=0;
    virtual double getX0()const override=0;
    virtual void setX0(double x)override=0;
    virtual double getL()const override=0;
    virtual void setL(double d)override=0;
    virtual std::string getname()const override=0;
    virtual void setname(std::string n)override=0;
    virtual std::string toString()const override;

    virtual double getCp(double Dmid, double M)const=0;
    virtual double getDend()const=0;

    virtual void setDend(double d)=0;
    virtual ~Module()override{}
};



class nosecone:public Module{
public:
    virtual double S()const override;
    virtual double mass()const override;
    virtual double massCenter()const override;

    virtual double getX0()const override;
    virtual void setX0(double x)override{x0=x;}

    virtual double getL()const override;
    virtual void setL(double l)override{len=l;}

    virtual double getCp(double Dmid, double M)const override;

    virtual double getDend()const override;
    virtual void setDend(double d)override;

    virtual std::string getname()const override;
    virtual void setname(std::string n)override{name=n;}

    virtual std::string toString()const override;

    virtual double getdelt()const{return delt;}
    virtual void setdelt(double d);


    virtual ~nosecone()override{}
    nosecone(){}
    nosecone(matherial math, double Dend, double length, double delta);
private:
    matherial mat;
    double dend=0;
    double len=0;
    double delt=0;
    double x0=0;
    std::string name;
};



class conoid:public Module{
public:
    virtual double S()const override{return 0.25*M_PI*dend*dend;}
    virtual double mass()const override{return mat.Ro*
                   0.25*M_PI*pow(0.5*(dend+dbegin-4*delt),2);
    }
    virtual double massCenter()const override;
    virtual double getCp(double Dmid, double M)const override{}

    virtual double getX0()const override{return X0;}
    virtual void setX0(double x)override{X0=x;}

    virtual double getL()const override{return len;}
    virtual void setL(double length)override{len=length;}


    virtual double getDend()const override{return dend;}
    virtual void setDend(double d)override{};

    virtual std::string getname()const override{};
    virtual void setname(std::string n)override{name=n;}

    virtual std::string toString()const override{};

    virtual ~conoid()override{}

    virtual double getdelt()const{return delt;}
    virtual void setdelt(double d){}

    virtual double getDbegin()const{return dbegin;}
    virtual void setDbegin(double dbeg){dbegin=dbeg;}


    conoid(){}
    conoid(matherial math, double Dbegin, double Dend, double length, double delta,double X):
        mat{math},dbegin{Dbegin},dend{Dend},len{length},delt{delta},X0{X}
    {
    }
private:
    matherial mat;
    double dbegin=0;
    double dend=0;
    double len=0;
    double delt=0;
    double X0;
    std::string name;
};



class engine:public Module{
public:
    virtual double S()const override{return 0.25*M_PI*d*d;}
    virtual double mass()const override{return 0;}
    virtual double massCenter()const override{return 0;}
    virtual double getCp(double Dmid, double M)const override{return 0;}

    virtual double getX0()const override{return X0;}
    virtual void setX0(double x)override{X0=x;}

    virtual double getL()const override{return len;}
    virtual void setL(double length)override{len=length;}


    virtual double getDend()const override{return d;}
    virtual void setDend(double d)override{}

    virtual std::string getname()const override{return "";}
    virtual void setname(std::string n)override{name=n;}

    virtual std::string toString()const override{return "";}

    virtual ~engine()override{}

    virtual double getdelt()const{return delt;}
    virtual void setdelt(double d){}

    virtual double getDbegin()const{return d;}
    virtual void setDbegin(double){}//


    engine(){}
    engine(matherial mathShell, matherial mathbr,
           matherial mathnozzle, matherial mathtz, fuel fuel,
           double Dmid, double mfuel, double Pk, double Pa);

private:
    matherial matShell;
    matherial matnozzle;
    matherial matbr;
    matherial matTz;
    engineparameters params;
    fuel fl;
    double d=0;
    double len=0;
    double delt=0;
    double X0=0;
    std::string name;










};












#endif // ROCKETMODULE_H
