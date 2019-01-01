#ifndef SETFLYTASK_H
#define SETFLYTASK_H

#include <QDialog>
#include "Gui/ItemModel/balcalcitemmodel.h"
namespace Ui {
class SetFlyTask;
}

class SetFlyTask : public QDialog{
    Q_OBJECT
    Ui::SetFlyTask *ui;
    const std::vector<material>* mats;
    const std::vector<fuel>* fls;
    balcalcItemModel* bcmodel;

    bool isHmaxValid=false;
    bool isHminValid=false;
    bool isXmaxValid=false;
    bool isMilestoneValid=false;
    bool isVtargetValid=false;
    bool isTethValid=false;
    bool isPaValid=false;
    bool isPkValid=false;
    bool isVendValid=false;
    bool isalphaValid=false;

public:
    explicit SetFlyTask(const std::vector<material>* materials,
             const std::vector<fuel>* fuels, balcalcItemModel* model,
                        QWidget *parent = nullptr);
    ~SetFlyTask();
    void setdata (bool edit =false);
    bool isDataValid();

private slots:
    void on_le_Hmax_editingFinished();
    void on_le_Hmin_editingFinished();
    void on_le_Xmax_editingFinished();
    void on_le_mstone_editingFinished();
    void on_le_Vtar_editingFinished();
    void on_le_teth_editingFinished();
    void on_le_Pa_editingFinished();
    void on_le_Pk_editingFinished();
    void on_buttonBox_accepted();
    void on_le_alpha_editingFinished();
    void on_le_Vend_editingFinished();
};

#endif // SETFLYTASK_H
