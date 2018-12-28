#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include <QtCharts/qchart.h>
#include <ballisticcalc.h>
namespace Ui {
class resultWindow;
}

class resultWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit resultWindow(const OutputData& outdata,QWidget *parent = nullptr);
    ~resultWindow();

private:
    Ui::resultWindow *ui;
    OutputData odat;
};

#endif // RESULTWINDOW_H
