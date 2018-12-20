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
    explicit errorDialog(QString text=QString("some error"),QWidget *parent = nullptr);
    ~errorDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::errorDialog *ui;
};

#endif // ERRORDIALOG_H
