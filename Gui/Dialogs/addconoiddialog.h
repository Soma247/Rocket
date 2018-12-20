#ifndef ADDCONOIDDIALOG_H
#define ADDCONOIDDIALOG_H

#include <QDialog>
#include <Gui/ItemModel/balcalcitemmodel.h>

namespace Ui {
class addConoidDialog;
}

class addConoidDialog : public QDialog{
    Q_OBJECT
    Ui::addConoidDialog *ui;
    balcalcItemModel* bcmodel;
    const std::vector<material>* mats;
    bool isD1valid=false;
    bool isD2valid=false;
    bool isLvalid=false;
    bool isdeltvalid=false;
    size_t curindex=overmaxindex;

    void checkD1();
    void checkD2();
    void checkL();
    void checkdelt();

public:
static const size_t overmaxindex=300;
    bool isDataValid();
    explicit addConoidDialog(const std::vector<material>* mats,balcalcItemModel* model,QWidget *parent = nullptr);
    ~addConoidDialog();
    void setdata(size_t index=overmaxindex);





private slots:
    void on_buttonBox_accepted();
    void on_le_D1_editingFinished();
    void on_le_D2_editingFinished();
    void on_le_L_editingFinished();
    void on_le_delt_editingFinished();
};

#endif // ADDCONOIDDIALOG_H
