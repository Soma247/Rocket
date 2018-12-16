#ifndef SETNOSECONEDIALOG_H
#define SETNOSECONEDIALOG_H

#include <QDialog>
#include <Gui/ItemModel/balcalcitemmodel.h>
namespace Ui {
class SetNoseConeDialog;
}

class SetNoseConeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetNoseConeDialog(balcalcItemModel* model,QWidget *parent = nullptr);
    ~SetNoseConeDialog();

private slots:
    void on_SetNoseConeDialog_accepted();

private:
    Ui::SetNoseConeDialog *ui;
    balcalcItemModel* bcmodel;
};

#endif // SETNOSECONEDIALOG_H
