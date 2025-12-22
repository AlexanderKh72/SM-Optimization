#include "dialog_setmethod.h"
#include "ui_dialog_setmethod.h"

Dialog_SetMethod::Dialog_SetMethod(OptimisationParameters& param, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_SetMethod)
    , param(param)
{
    ui->setupUi(this);
    ui->splitter_vertical->setSizes({1,1,3});


    ui->RosenbrockButton->setChecked(param.rosenbrock);
    ui->SphereButton->setChecked(param.sphere);
    ui->LeftBottomX_SpinBox->setValue(param.lbx);
    ui->LeftBottomY_SpinBox->setValue(param.lby);
    ui->RightTopX_SpinBox->setValue(param.rtx);
    ui->RightTopY_SpinBox->setValue(param.rty);
    ui->NM_radioButton->setChecked(param.nelder_mead);
    ui->NMalpha_SpinBox->setValue(param.nmalpha);
    ui->NMbeta_SpinBox->setValue(param.nmbeta);
    ui->NMgamma_SpinBox->setValue(param.nmgamma);
    ui->SS_radioButton->setChecked(param.simple_stochastic);
    ui->SSp_SpinBox->setValue(param.ssp);
    ui->SSdelta_SpinBox->setValue(param.ssdelta);
    ui->SCepsilon_SpinBox->setValue(param.scepsilon);
    ui->SCN_SpinBox->setValue(param.scN);
}

Dialog_SetMethod::~Dialog_SetMethod()
{
    delete ui;
}

void Dialog_SetMethod::on_buttonBox_accepted()
{
    param.rosenbrock = ui->RosenbrockButton->isChecked();
    param.sphere = ui->SphereButton->isChecked();
    param.lbx = ui->LeftBottomX_SpinBox->value();
    param.lby = ui->LeftBottomY_SpinBox->value();
    param.rtx = ui->RightTopX_SpinBox->value();
    param.rty = ui->RightTopY_SpinBox->value();
    param.nelder_mead = ui->NM_radioButton->isChecked();
    param.nmalpha = ui->NMalpha_SpinBox->value();
    param.nmbeta = ui->NMbeta_SpinBox->value();
    param.nmgamma = ui->NMgamma_SpinBox->value();
    param.simple_stochastic = ui->SS_radioButton->isChecked();
    param.ssp = ui->SSp_SpinBox->value();
    param.ssdelta = ui->SSdelta_SpinBox->value();
    param.scepsilon = ui->SCepsilon_SpinBox->value();
    param.scN = ui->SCN_SpinBox->value();

    emit optimisationParametersChanged();
}

void Dialog_SetMethod::on_LeftBottomX_SpinBox_valueChanged(double arg1)
{
    ui->RightTopX_SpinBox->setMinimum(arg1);
}


void Dialog_SetMethod::on_LeftBottomY_SpinBox_valueChanged(double arg1)
{
     ui->RightTopY_SpinBox->setMinimum(arg1);
}


void Dialog_SetMethod::on_RightTopX_SpinBox_valueChanged(double arg1)
{
    ui->LeftBottomX_SpinBox->setMaximum(arg1);
}


void Dialog_SetMethod::on_RightTopY_SpinBox_valueChanged(double arg1)
{
    ui->LeftBottomY_SpinBox->setMaximum(arg1);
}

