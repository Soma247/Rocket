#ifndef ADDCONOIDDIALOG_H
#define ADDCONOIDDIALOG_H

#include <QDialog>
#include <Gui/ItemModel/balcalcitemmodel.h>
namespace Ui {
class addConoidDialog;
}

class addConoidDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addConoidDialog(balcalcItemModel* model,QWidget *parent = nullptr);
    ~addConoidDialog();

private slots:
    void on_buttonBox_accepted();
    void on_le_D1_editingFinished();

    void on_le_D2_editingFinished();

private:
    Ui::addConoidDialog *ui;
    balcalcItemModel* bcmodel;
};

#endif // ADDCONOIDDIALOG_H
