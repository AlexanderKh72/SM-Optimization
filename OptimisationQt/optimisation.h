#ifndef OPTIMISATION_H
#define OPTIMISATION_H

#include <QWidget>
#include <QGraphicsScene>
#include "Optimizers/AbstrOptimizer.h"
#include "Optimizers/AbstrFunction.h"
#include "Optimizers/RectArea.h"

struct OptimisationParameters {
    bool rosenbrock = true;
    bool sphere = false;
    double lbx = 0;
    double lby = 0;
    double rtx = 1;
    double rty = 1;
    bool nelder_mead = true;
    double nmalpha = 1;
    double nmbeta = 0.5;
    double nmgamma = 2;
    bool simple_stochastic = false;
    double ssp = 0.5;
    double ssdelta = 1;
    double scepsilon = 0.001;
    int scN = 50;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class Optimisation;
}
QT_END_NAMESPACE

class Optimisation : public QWidget
{
    Q_OBJECT

public:
    Optimisation(QWidget *parent = nullptr);
    ~Optimisation();

private slots:
    void ChangeArea();
    void ChangeParamLabel();
    void on_SetMethodButton_clicked();
    void setOptimisationParameters(

    );
    void pointSelected(QMouseEvent* event);

private:
    Ui::Optimisation *ui;
    OptimisationParameters param;
    std::shared_ptr<AbstrOptimizer> optimizer;
    std::shared_ptr<AbstrFunction> func;
    std::shared_ptr<AbstrStopCriteria> stopcr;
    std::shared_ptr<RectArea> area;

signals:
    void AreaChanged();
    void MethodOrFunctionChanged();
};
#endif // OPTIMISATION_H
