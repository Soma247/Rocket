#ifndef SETNOSECONEDIALOG_H
#define SETNOSECONEDIALOG_H

#include <QDialog>
#include <Gui/ItemModel/balcalcitemmodel.h>
namespace Ui {
class SetNoseConeDialog;
}

class SetNoseConeDialog : public QDialog{
    Q_OBJECT
    Ui::SetNoseConeDialog *ui;
    balcalcItemModel* bcmodel;
    const std::vector<material>*mats;
    bool isDvalid=false;
    bool isLvalid=false;
    bool isdeltvalid=false;

public:
    explicit SetNoseConeDialog(const std::vector<material>* materials, balcalcItemModel* model,QWidget *parent = nullptr);
    ~SetNoseConeDialog();
    void setdata(bool edit=false);
    void checkD();
    void checkL();
    void checkdelt();
    bool isDataValid();

private slots:
    void on_SetNoseConeDialog_accepted();
    void on_le_L_editingFinished();
    void on_le_D_editingFinished();
    void on_le_delt_editingFinished();
};

#endif // SETNOSECONEDIALOG_H
