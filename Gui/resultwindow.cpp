#include "resultwindow.h"
#include "ui_resultwindow.h"

resultWindow::resultWindow(const OutputData& outdata,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::resultWindow),
    odat{outdata}
{
    ui->setupUi(this);
     std::cerr<<"line series for charts"<<std::endl;
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
    for(const auto& b:odat.traect){
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
    QChart*chart_traect=new QChart();
    QChart*chart_V=new QChart();
    QChart*chart_CxCy=new QChart();

    chart_traect->addSeries(traectory);
    chart_V->addSeries(V);
    chart_CxCy->addSeries(Cx);
    chart_CxCy->addSeries(Cy);
    chart_CxCy->setTitle("Аэродинамические коэффициенты");

    chart_traect->createDefaultAxes();
    chart_traect->axisX()->setTitleText("Дальность(м)");
    chart_traect->axisY()->setTitleText("Высота(м)");


    chart_V->createDefaultAxes();
    chart_V->axisX()->setTitleText("Время(с)");
    chart_V->axisY()->setTitleText("Скорость(м/с)");

    chart_CxCy->createDefaultAxes();
    chart_CxCy->axisX()->setTitleText("Время(с)");
    chart_CxCy->axisY()->setTitleText("Значение");

    //chartview setup
    ui->chart_traect->setChart(chart_traect);
    ui->chart_traect->setRenderHint(QPainter::Antialiasing);
    ui->chart_V->setChart(chart_V);
    ui->chart_V->setRenderHint(QPainter::Antialiasing);
    ui->chart_cxcy->setChart(chart_CxCy);
    ui->chart_cxcy->setRenderHint(QPainter::Antialiasing);
}

resultWindow::~resultWindow()
{
    delete ui;
}
