#ifndef DIALOG_SETMETHOD_H
#define DIALOG_SETMETHOD_H

#include <QDialog>
#include "optimisation.h"

namespace Ui {
class Dialog_SetMethod;
}

class Dialog_SetMethod : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SetMethod(OptimisationParameters& param, QWidget *parent = nullptr);
    ~Dialog_SetMethod();

private slots:
    void on_buttonBox_accepted();

    void on_LeftBottomX_SpinBox_valueChanged(double arg1);

    void on_LeftBottomY_SpinBox_valueChanged(double arg1);

    void on_RightTopX_SpinBox_valueChanged(double arg1);

    void on_RightTopY_SpinBox_valueChanged(double arg1);

private:
    Ui::Dialog_SetMethod *ui;
    OptimisationParameters& param;

signals:
    void optimisationParametersChanged();
};

#endif // DIALOG_SETMETHOD_H
