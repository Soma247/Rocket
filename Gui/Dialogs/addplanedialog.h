#ifndef ADDPLANEDIALOG_H
#define ADDPLANEDIALOG_H

#include <QDialog>
#include "Gui/ItemModel/balcalcitemmodel.h"
namespace Ui {
class addPlaneDialog;
}

class addPlaneDialog : public QDialog{
    Q_OBJECT
    Ui::addPlaneDialog *ui;
    balcalcItemModel* bcmodel;
    const std::vector<material>*mats;
    bool isXvalid=true;
    bool isBrootValid=true;
    bool isBtipValid=true;
    bool isCrootValid=true;
    bool isCtipValid=true;
    bool isXtipValid=true;
    bool isXrfValid=true;
    bool isXrrValid=true;
    bool isXtfValid=true;
    bool isXtrValid=true;
    bool isHvalid=true;
    bool isTail=false;
    bool isEdit=false;
    int curindex=0;

public:
    explicit addPlaneDialog(const std::vector<material>* materials, balcalcItemModel* model,QWidget *parent = nullptr);
    ~addPlaneDialog();
    void setdata (bool tail=false, bool edit =false, int index=0);
    bool isDataValid();

private slots:
    void on_le_broot_editingFinished();
    void on_le_Btip_editingFinished();
    void on_le_Croot_editingFinished();
    void on_le_Ctip_editingFinished();
    void on_le_Xtip_editingFinished();
    void on_le_X_editingFinished();
    void on_le_Xrf_editingFinished();
    void on_le_Xrr_editingFinished();
    void on_le_Xtf_editingFinished();
    void on_le_Xtr_editingFinished();
    void on_le_H_editingFinished();
    void on_buttonBox_accepted();
};

#endif // ADDPLANEDIALOG_H
