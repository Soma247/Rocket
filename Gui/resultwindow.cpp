#include "resultwindow.h"
#include "ui_resultwindow.h"

resultWindow::resultWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::resultWindow)
{
    ui->setupUi(this);
}

resultWindow::~resultWindow()
{
    delete ui;
}
