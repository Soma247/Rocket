#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "resultwindow.h"
#include "ui_resultwindow.h"


resultWindow::resultWindow(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::resultWindow)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
}

resultWindow::~resultWindow()
{
    delete ui;
}

void resultWindow::setdata(const OutputData &outdata,QString filename)
{
    fname=filename;
    odat=outdata;
    //line series for charts
    QLineSeries* traectory=new QLineSeries();
    QLineSeries* V=new QLineSeries();
    QLineSeries* Cx=new QLineSeries();
    QLineSeries* Cy=new QLineSeries();
    //blur bloc 5x1
    for(int i=3;i;--i)
        for(size_t i=2;i<odat.traect.size()-2;++i){
            odat.traect.at(i).cx=(odat.traect.at(i-1).cx+odat.traect.at(i+1).cx+odat.traect.at(i-2).cx+odat.traect.at(i+2).cx)/4;
            odat.traect.at(i).cy=(odat.traect.at(i-1).cy+odat.traect.at(i+1).cy+odat.traect.at(i-2).cy+odat.traect.at(i+2).cy)/4;
        }
    double xmax{0},ymax{0},cxmin{10},cxmax{0},cymin{10},cymax{0},tmax{0},vmax{0};
    for(const auto& b:odat.traect){
        if(tmax<b.t)tmax=b.t;
        if(xmax<b.X)xmax=b.X;
        if(ymax<b.H)ymax=b.H;
        if(vmax<b.V)vmax=b.V;
        if(cxmax<b.cx)cxmax=b.cx;
        if(cymax<b.cy)cymax=b.cy;
        if(cxmin>b.cx)cxmin=b.cx;
        if(cymin>b.cy)cymin=b.cy;

        traectory->append(b.X,b.H);
        V->append(b.t,b.V);
        Cx->append(b.t,b.cx);
        Cy->append(b.t,b.cy);
    }
    traectory->setName("Траектория полета");
    V->setName("Скорость полета(м/с)");
    Cx->setName("Cx");
    Cy->setName("Cya");

    //qcharts
    QChart* chart_traect=new QChart();
    QChart* chart_V=new QChart();
    QChart* chart_CxCy=new QChart();
    QChart* chart_Zones=new QChart();
    chart_traect->addSeries(traectory);
    chart_V->addSeries(V);
    chart_CxCy->addSeries(Cx);
    chart_CxCy->addSeries(Cy);
    chart_CxCy->setTitle("Аэродинамические коэффициенты");
    chart_Zones->setTitle("Скоростные зоны:");

    double xzmax{0},yzmax{0};

    //blur
    for(int i=3;i>0;--i)
        for(auto& zone:odat.zns)
            for(size_t i=2;i<zone.second.size()-2;++i)
                zone.second.at(i).second=(zone.second.at(i-1).second+
                                          zone.second.at(i-2).second+
                                          zone.second.at(i+1).second+
                                          zone.second.at(i+2).second)/4;


    for(auto& zone:odat.zns){
        QLineSeries*temp=new QLineSeries();
        temp->setName(QString("V=%1").arg(zone.first));
        for(auto&point:zone.second){
            temp->append(point.first,point.second);
            if(xzmax<point.first)xzmax=point.first;
            if(yzmax<point.second)yzmax=point.second;
        }
        chart_Zones->addSeries(temp);
    }

    chart_traect->createDefaultAxes();
    chart_traect->axisX()->setTitleText("Дальность(м)");
    chart_traect->axisY()->setTitleText("Высота(м)");
    chart_traect->axisX()->setMax(1000+((int(xmax)/1000)*1000));
    chart_traect->axisY()->setMax(1000+((int(ymax)/1000)*1000));
    chart_traect->axisY()->setMin(0);

    chart_V->createDefaultAxes();
    chart_V->axisX()->setTitleText("Время(с)");
    chart_V->axisY()->setTitleText("Скорость(м/с)");
    chart_V->axisX()->setMax(10+((int(tmax)/10)*10));
    chart_V->axisY()->setMax(100+((int(vmax)/100)*100));
    chart_V->axisY()->setMin(0);

    chart_CxCy->createDefaultAxes();
    chart_CxCy->axisX()->setTitleText("Время(с)");
    chart_CxCy->axisY()->setTitleText("Значение");
    chart_CxCy->axisX()->setMax(10+((int(tmax)/10)*10));

    chart_Zones->createDefaultAxes();
    chart_Zones->axisX()->setTitleText("Дальность(м)");
    chart_Zones->axisY()->setTitleText("Высота(м)");
    chart_Zones->axisX()->setMax(1000+((int(xzmax)/1000)*1000));
    chart_Zones->axisY()->setMax(1000+((int(yzmax)/1000)*1000));

    //chartview setup
    ui->chart_traect->setChart(chart_traect);
    ui->chart_traect->setRenderHint(QPainter::Antialiasing);
    ui->chart_V->setChart(chart_V);
    ui->chart_V->setRenderHint(QPainter::Antialiasing);
    ui->chart_cxcy->setChart(chart_CxCy);
    ui->chart_cxcy->setRenderHint(QPainter::Antialiasing);
    ui->chart_zones->setChart(chart_Zones);
    ui->chart_zones->setRenderHint(QPainter::Antialiasing);
    ui->table_all->clear();
    ui->table_all->setRowCount(int(odat.traect.size()));
    ui->table_all->setColumnCount(9);
    ui->table_all->setShowGrid(true);
    ui->table_all->setHorizontalHeaderLabels(QStringList{"t(с)","V(м/с)","X(м)","Y(м)","mass(кг)","Cx","Cya","teta(град)","mzCy"});
    ui->table_all->verticalHeader()->hide();

    int i=0;
    for(auto& b:odat.traect){
        ui->table_all->setItem(i,0, new QTableWidgetItem(QString("%1").arg(b.t)));
        ui->table_all->setItem(i,1, new QTableWidgetItem(QString("%1").arg(b.V)));
        ui->table_all->setItem(i,2, new QTableWidgetItem(QString("%1").arg(b.X)));
        ui->table_all->setItem(i,3, new QTableWidgetItem(QString("%1").arg(b.H)));
        ui->table_all->setItem(i,4, new QTableWidgetItem(QString("%1").arg(b.mass)));
        ui->table_all->setItem(i,5, new QTableWidgetItem(QString("%1").arg(b.cx)));
        ui->table_all->setItem(i,6, new QTableWidgetItem(QString("%1").arg(b.cy)));
        ui->table_all->setItem(i,7, new QTableWidgetItem(QString("%1").arg(b.teth)));
        ui->table_all->setItem(i,8, new QTableWidgetItem(QString("%1").arg(b.stability)));
        i++;
    }
    ui->table_all->resizeColumnsToContents();
    ui->table_all->resizeRowsToContents();




    /*тут текст отчета
  *
  *
  *
*/

    QString mes=QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                        "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                        "<style type=\"text/css\">p, li {white-space: pre-wrap; }</style></head>"
                        "<body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                        " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">"
                        "Параметры ракеты:</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
                        " margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                        "Длина головной части: %1 м;</span></p><p style=\" margin-top:0px; margin-bottom:0px;"
                        " margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\""
                        " font-size:8pt;\">Масса головной части: %2 кг;</span></p><p style=\" margin-top:0px;"
                        " margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                        " text-indent:0px;\"><span style=\" font-size:8pt;\">Диаметр РДТТ: %3 м;"
                        "</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
                        " margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                        "Максимальный диаметр: %4 м;</span></p>").arg(odat.headData.headLen)
            .arg(odat.headData.headMass)
            .arg(odat.headData.headDend)
            .arg(odat.headData.headDmax);


    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
         " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
                 " margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">"
                 "Характеристики ракеты:</span></p>");

    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Длина ракеты(м): %1;</span></p>").arg(odat.len,0,'D',2);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Стартовая масса(кг): %1;</span></p>").arg(odat.massfull,0,'D',2);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Конечная масса(кг): %1;</span></p>").arg(odat.massempty,0,'D',2);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Положение центра масс на старте(кг): %1;</span></p>").arg(odat.xmfull,0,'D',2);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Положение центра масс в конце полета(кг): %1;</span></p>").arg(odat.xmempty,0,'D',2);


    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
         " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
                 " margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">"
                 "Отсеки:</span><span style=\" font-size:7pt;\"> D0(м)    D1(м)    L(м)    масса(кг), материал:</span></p>");
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Отсек 1: %1  %2  %3  %4  %5;</span></p>").arg(odat.headData.nconepar.dbeg,0,'D',2)
            .arg(odat.headData.nconepar.dend,0,'D',2)
            .arg(odat.headData.nconepar.len,0,'D',2)
            .arg(odat.headData.nconepar.mass,0,'D',1)
            .arg(QString::fromStdString(odat.headData.nconepar.mat.name));

    for(size_t i=0;i<odat.headData.conespar.size();i++){
        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                     " text-indent:0px;\"><span style=\" font-size:8pt;\">Отсек %1: %2  %3  %4  %5  %6;</span>"
                     "</p>").arg(i+2)
                .arg(odat.headData.conespar.at(i).dbeg,0,'D',2).
                arg(odat.headData.conespar.at(i).dend,0,'D',2)
                .arg(odat.headData.conespar.at(i).len,0,'D',2)
                .arg(odat.headData.conespar.at(i).mass,0,'D',1)
                .arg(QString::fromStdString(odat.headData.conespar.at(i).mat.name));
    }

    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Отсек %1(хвостовой): %2  %3  %4  %5  %6;</span></p>")
            .arg(odat.headData.conespar.size()+2)
            .arg(odat.tailconepar.dbeg,0,'D',2)
            .arg(odat.tailconepar.dend,0,'D',2)
            .arg(odat.tailconepar.len,0,'D',2)
            .arg(odat.tailconepar.mass,0,'D',1)
            .arg(QString::fromStdString(odat.tailconepar.mat.name));

    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
         " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";


    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;"
                 " font-weight:600;\">Плоскости(1 конс):</span><span style=\" font-size:7pt;\">"
                 " S(кв.м)  X(м)  масса (кг)  материал:</span></p>");

    for(size_t i=0;i<odat.headData.planespar.size();i++){
        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Плоскость %1: %2  %3  %4  %5;</span></p>")
                .arg(i+1)
                .arg(odat.headData.planespar.at(i).S(),0,'D',2)
                .arg(odat.headData.planespar.at(i).XfromNose,0,'D',2)
                .arg(odat.headData.planespar.at(i).massn/4,0,'D',2)
                .arg(QString::fromStdString(odat.headData.planespar.at(i).mat.name));

        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Broot= %1, Btip= %2, Croot=%3, Ctip=%4, H=%5, стреловидность=%6 град;</span></p>")
                .arg(odat.headData.planespar.at(i).broot)
                .arg(odat.headData.planespar.at(i).btip)
                .arg(odat.headData.planespar.at(i).croot)
                .arg(odat.headData.planespar.at(i).ctip)
                .arg(odat.headData.planespar.at(i).h)
                .arg(atan(odat.headData.planespar.at(i).xtip/odat.headData.planespar.at(i).h)*180/M_PI);
    }

    if(odat.headData.tailstabpar.massn>0){
        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                     " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                     "Хвост. стаб : %1  %2  %3  %4;</span></p>")
                .arg(odat.headData.tailstabpar.S(),0,'D',2)
                .arg(odat.headData.tailstabpar.XfromNose,0,'D',2)
                .arg(odat.headData.tailstabpar.massn/4,0,'D',2)
                .arg(QString::fromStdString(odat.headData.tailstabpar.mat.name));

        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                     " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                     "Broot= %1, Btip= %2, Croot=%3, Ctip=%4, H=%5, стреловидность=%6 град;</span></p>")
                .arg(odat.headData.tailstabpar.broot)
                .arg(odat.headData.tailstabpar.btip)
                .arg(odat.headData.tailstabpar.croot)
                .arg(odat.headData.tailstabpar.ctip)
                .arg(odat.headData.tailstabpar.h)
                .arg(atan(odat.headData.tailstabpar.xtip/odat.headData.tailstabpar.h)*180/M_PI);
    }

    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Грузы:</span><span style=\" font-size:7pt;\">Координата(м)  масса (кг):</span></p>");
    for(auto&e:odat.headData.equipspar)
        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">%1: %2  %3;</span></p>").arg(QString::fromStdString(e.name),8).arg(e.X,0,'D',2).arg(e.m,0,'D',2);
    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";



    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Макс. высота полета ракеты(м): %1;</span></p>").arg(yzmax);

    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Максимальная дальность полета ракеты(м): %1;</span></p>").arg(xzmax);

    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
         " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";

    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;"
                 " font-weight:600;\">Параметры ДУ:</span><span style=\" font-size:7pt;\"></span></p>");

    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Время работы двигателя(с): %1;</span></p>").arg(odat.engpar.t);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Длина двигателя(м): %1;</span></p>").arg(odat.engpar.Leng);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Масса двигателя(кг): %1;</span></p>").arg(odat.engpar.mengine);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Масса заряда топлива(кг): %1;</span></p>").arg(odat.engpar.mfuel);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Масса теплозащитного покрытия(кг): %1;</span></p>").arg(odat.engpar.mtz);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Диаметр критического сечения сопла(м): %1;</span></p>").arg(odat.engpar.Dcr);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Диаметр среза сопла(м): %1;</span></p>").arg(odat.engpar.Da);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Длина сопла(м): %1;</span></p>").arg(odat.engpar.La);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "материал корпуса: %1, материал бронирования: %2, материал ТЗП: %3, материал сопла: %4;</span></p>")
            .arg(QString::fromStdString(odat.engpar.matShell.name))
            .arg(QString::fromStdString(odat.engpar.matbr.name))
            .arg(QString::fromStdString(odat.engpar.matTz.name))
            .arg(QString::fromStdString(odat.engpar.matnozzle.name));

    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
         " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Параметры топлива:</span></p>");
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Наименование: %1;</span></p>").arg(QString::fromStdString(odat.engpar.fl.name));
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Плотность(кг/м^3): %1;</span></p>").arg(odat.engpar.fl.rot);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Процентное содержание AL: %1%;</span></p>").arg(odat.engpar.fl.AL);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Удельный импульс тяги(м/с): %1;</span></p>").arg(odat.engpar.fl.Prelst);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Показатель адиабаты: %1;</span></p>").arg(odat.engpar.fl.kst);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Газовая постоянная(ДЖ/кг*град): %1;</span></p>").arg(odat.engpar.fl.rst);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Температура горения(К): %1;</span></p>").arg(odat.engpar.fl.tst);

    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Закон горения(мм/с): %1*Pk^%2;</span></p>").arg(odat.engpar.fl.ukoef).arg(odat.engpar.fl.upow);

    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Давление на срезе сопла(мПА): %1;</span></p>").arg(odat.engpar.Pa);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                 " -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">"
                 "Давление в камере сгорания(мПА): %1;</span></p>").arg(odat.engpar.Pk);
    mes+="</body></html>";
    ui->textBrowser->setHtml(mes);
}

void resultWindow::on_pushButton_2_clicked()//exit
{
    QMessageBox msgBox(this);
    msgBox.setInformativeText("Подтвердите закрытие окна.");
    msgBox.setStyleSheet(ui->textBrowser->styleSheet());
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    //поправить вид диалога
    int ret = msgBox.exec();
    if(ret==QMessageBox::Ok){
        dynamic_cast<QMainWindow*>(parent())->show();
        this->hide();
    }
}

void resultWindow::on_pushButton_3_clicked()//save
{
    if(!fname.size()){
        fname = QFileDialog::getExistingDirectory(
                this,
                tr("Выберите директорию проекта"),
                QDir::currentPath() );
        if(fname.size())
            fname+="new_project.bcpr";
        else return;
    }

    //project dir info
    QStringList parsedfname=fname.split("/");
    QString filesname=parsedfname.back().split(".").first();
    QString dir;
    for(auto it=parsedfname.begin();it!=parsedfname.end()&&it!=parsedfname.end()-1;++it)
        dir+=*it+='/';
    //save charts
    QPixmap traect=ui->chart_traect->grab();
    QPixmap V=ui->chart_V->grab();
    QPixmap CxCy=ui->chart_cxcy->grab();
    QPixmap zones=ui->chart_zones->grab();
    traect.save(dir+filesname+"T.jpg","jpg");
    V.save(dir+filesname+"V.jpg","jpg");
    CxCy.save(dir+filesname+"CxCy.jpg","jpg");
    zones.save(dir+filesname+"zones.jpg","jpg");
    //save table
    std::ofstream tabout((dir+filesname+"table.csv").toStdString());
    if(tabout){
        std::stringstream text;
        text<<"t(с) V(м/с) X(м) Y(м) mass(кг) Cx Cya teta(град) mzCy"<<std::endl;
        for(const auto& b:odat.traect){
            text<<b.t<<' '
               <<b.V<<' '
              <<b.X<<' '
             <<b.H<<' '
            <<b.mass<<' '
            <<b.cx<<' '
            <<b.cy<<' '
            <<b.teth<<' '
            <<b.stability
            <<std::endl;
        }

        QString str{QString::fromStdString(text.str())};
        str.replace('.',',');
        tabout<<str.toStdString();
    }
    else{
        QMessageBox msgBox(this);
        msgBox.setStyleSheet(ui->chart_traect->styleSheet());
        msgBox.setInformativeText(QString("файл %1 недоступен").arg(QString(dir+filesname+"table.csv")));
        msgBox.setStandardButtons(QMessageBox::Ok);
    }
    std::ofstream textout((dir+filesname+"summary.txt").toStdString());
    if(textout){
        textout<<"Проект "<<filesname.toStdString()<<':'<<std::endl<<std::endl;
        textout<<ui->textBrowser->toPlainText().toStdString();
    }
    else{
        QMessageBox msgBox(this);
        msgBox.setStyleSheet(ui->chart_traect->styleSheet());
        msgBox.setInformativeText(QString("файл %1 недоступен").arg(QString(dir+filesname+"summary.txt")));
        msgBox.setStandardButtons(QMessageBox::Ok);
    }
}

void resultWindow::closeEvent(QCloseEvent *__e)
{
    __e->ignore();
    on_pushButton_2_clicked();
}
