#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include <QtCharts/qchart.h>

namespace Ui {
class resultWindow;
}

class resultWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit resultWindow(QWidget *parent = nullptr);
    ~resultWindow();

private:
    Ui::resultWindow *ui;
};

#endif // RESULTWINDOW_H
