#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include <QtCharts/qchart.h>
#include <ballisticcalc.h>
namespace Ui {
class resultWindow;
}

class resultWindow : public QMainWindow{
    Q_OBJECT
    QString fname;
    Ui::resultWindow *ui;
    OutputData odat;
protected:
    void closeEvent( QCloseEvent *__e );

public:
    explicit resultWindow(QMainWindow *parent = nullptr);
    ~resultWindow();
    void setdata(const OutputData& outdata, QString filename);
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // RESULTWINDOW_H
