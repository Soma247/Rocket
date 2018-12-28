#ifndef EDITFUELS_H
#define EDITFUELS_H

#include <QDialog>
#include "ballisticcalc.h"
namespace Ui {
class EditFuels;
}

class EditFuels : public QDialog
{
    Q_OBJECT
    Ui::EditFuels *ui;
    std::vector<material>* mats;
    std::vector<fuel>* fls;

    bool isfl_PudstValid=false;
    bool isfl_RstValid=false;
    bool isfl_TstValid=false;
    bool isfl_alValid=false;
    bool isfl_kValid=false;
    bool isfl_nameValid=false;
    bool isfl_roValid=false;
    bool isfl_ukoefValid=false;
    bool is_fl_upowValid=false;
    bool ismat_nameValid=false;
    bool ismat_roValid=false;
    bool ismat_sigmaValid=false;

public:
    explicit EditFuels(std::vector<material>* materials,
                        std::vector<fuel>* fuels, QWidget *parent = nullptr);
    ~EditFuels();
signals:
    void matupdated();
    void flupdated();
private slots:
    void on_cb_fuels_currentIndexChanged(const QString &arg1);
    void on_cb_mats_currentIndexChanged(const QString &arg1);
    void on_le_mat_name_editingFinished();
    void on_le_mat_ro_editingFinished();
    void on_le_mat_sigma_editingFinished();
    void on_le_fl_name_editingFinished();
    void on_le_fl_al_editingFinished();
    void on_le_fl_Pudst_editingFinished();
    void on_le_fl_ro_editingFinished();
    void on_le_fl_Tst_editingFinished();
    void on_le_fl_Rst_editingFinished();
    void on_le_fl_k_editingFinished();
    void on_le_fl_ukoef_editingFinished();
    void on_le_fl_upow_editingFinished();
    void on_btn_savemat_clicked();
    void on_btn_savefl_clicked();
    bool matiscorrect();
    bool fliscorrect();
    void on_pushButton_3_clicked();
    void update();
};

#endif // EDITFUELS_H
