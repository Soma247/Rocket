#ifndef ADDEQUIPMENTDIALOG_H
#define ADDEQUIPMENTDIALOG_H

#include <QDialog>
#include "Gui/ItemModel/balcalcitemmodel.h"

namespace Ui {
class addequipmentDialog;
}

class addequipmentDialog : public QDialog{
    Q_OBJECT
    Ui::addequipmentDialog *ui;
    balcalcItemModel* bcmodel;
    bool isXvalid=true;
    bool isMassValid=true;
    bool isNameValid=true;
    std::string curname;
    int curindex=0;
    static const int isNotIndex=-1;

public:
    explicit addequipmentDialog(balcalcItemModel* model,QWidget *parent = nullptr);
    ~addequipmentDialog();
    bool isDataValid();
    void setdata(int index=isNotIndex);
private slots:
    void on_le_name_editingFinished();
    void on_le_Xm_editingFinished();
    void on_le_m_editingFinished();
    void on_buttonBox_accepted();
};

#endif // ADDEQUIPMENTDIALOG_H
