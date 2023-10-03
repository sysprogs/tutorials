#pragma once
#include <QDialog>

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewDialog(QWidget *parent = 0);
    ~NewDialog();

protected slots:

private:
    Ui::NewDialog *ui;
};

