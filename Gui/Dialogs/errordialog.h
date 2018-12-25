#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

namespace Ui {
class errorDialog;
}

class errorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit errorDialog(QString text=QString("some error"),bool critical=true,QWidget *parent = nullptr);
    ~errorDialog();
    void setdata(QString text=QString("some error"),bool critical=true);

private slots:
    void on_pushButton_clicked();

private:
    bool crit;
    QString mes;
    Ui::errorDialog *ui;
};

#endif // ERRORDIALOG_H
