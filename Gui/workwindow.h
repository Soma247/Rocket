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
#include "Dialogs/choosewindow.h"
#include "Dialogs/about.h"
#include "Dialogs/editfuels.h"
#include "resultwindow.h"

namespace Ui {
class WorkWindow;
}

class WorkWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WorkWindow(QString matfile, QString hfuelsfile, QString lastprojectsfile, QWidget *parent = nullptr);
    ~WorkWindow();

  bool readMaterials(QString matfile);
  bool readHardfuels(QString hfuelsfile);



private slots:
    void on_pushButton_clicked();
    void selectionChangedSlot(const QItemSelection &, const QItemSelection &);
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void on_action_open_triggered();

    void on_action_save_as_triggered();

    void on_action_save_triggered();

    void on_action_new_triggered();

    void on_about_triggered();

    void on_btn_balcalculate_clicked();

    void on_flmatedit_triggered();

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
    void openFile();
    void saveFile();
    void openFile(std::string filename);
    void savefnames();
    void saveFile(std::string filename);
    void newProject();
    void savemats();
    void savefuels();

private:
    Ui::WorkWindow *ui;
    addConoidDialog* addconedial;
    SetNoseConeDialog* ncdial;
    addPlaneDialog* addplane;
    addequipmentDialog* addeqdial;
    std::vector<fuel> hardfuels;
    std::vector<material>materials;
    std::list<std::pair<QString,QString>> fnames;
    std::string profile;
    std::string matfile;
    std::string flfile;
    errorDialog* errd;
    SetFlyTask* setflytaskdial;
    chooseWindow* choosedial;
    about* aboutdial;
    EditFuels* editfuelmatdial;
    resultWindow* resultw;

    size_t curindex=0;
    bool isfirstSetInputData=true;
};

#endif // WORKWINDOW_H
