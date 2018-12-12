#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include <QMainWindow>
#include "Gui/ItemModel/balcalcitemmodel.h"
#include <QItemSelection>
namespace Ui {
class WorkWindow;
}

class WorkWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WorkWindow(QWidget *parent = nullptr);
    ~WorkWindow();


public slots:
    void updateActions();

private slots:
    void on_pushButton_clicked();
    void insertChild();
    void selectionChangedSlot(const QItemSelection &, const QItemSelection &);
    void on_treeView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::WorkWindow *ui;
};

#endif // WORKWINDOW_H
