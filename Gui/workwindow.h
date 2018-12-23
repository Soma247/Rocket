#ifndef WORKWINDOW_H
#define WORKWINDOW_H
#include <QMainWindow>
#include "Gui/ItemModel/balcalcitemmodel.h"
#include <QItemSelection>
#include "Dialogs/addconoiddialog.h"
#include "Dialogs/setnoseconedialog.h"
#include "Dialogs/errordialog.h"
#include "Dialogs/addplanedialog.h"
#include "Dialogs/addequipmentdialog.h"
#include "Dialogs/setflytask.h"

namespace Ui {
class WorkWindow;
}

class WorkWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WorkWindow(QString matfile, QString hfuelsfile, QWidget *parent = nullptr);
    ~WorkWindow();

  bool readMaterials(QString matfile);
  bool readHardfuels(QString hfuelsfile);



private slots:
    void on_pushButton_clicked();
    void selectionChangedSlot(const QItemSelection &, const QItemSelection &);
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void insertChild();

public slots: //для реализации сигнала selectionChanged у QTreeView::selectionModel
    void updateActions(const QItemSelection &,const QItemSelection &);
    void AddConDialog();
    void SetNoseDialog();
    void EditNoseDialog();
    void EditConDialog();
    void RemoveConoid();
    void updateActions(); //слот для обновления состояния кнопок
    void SetTailStabDialog();
    void EditTailStabDialog();
    void RemoveTailStabDialog();
    void SetStabDialog();
    void EditStabDialog();
    void RemoveStabDialog();
    void AddEqDialog();
    void EditEqDialog();
    void RemoveEqDialog();
    void setFlyTaskDialog();

private:
    Ui::WorkWindow *ui;
    addConoidDialog* addconedial;
    SetNoseConeDialog* ncdial;
    addPlaneDialog* addplane;
    addequipmentDialog* addeqdial;
    std::vector<fuel> hardfuels;
    std::vector<material>materials;
    errorDialog* errd;
    SetFlyTask* setflytaskdial;

    size_t curindex=0;
    bool isfirstSetInputData=true;
};

#endif // WORKWINDOW_H
