#include "optimisation.h"
#include "ui_optimisation.h"
#include "dialog_setmethod.h"
#include "Optimizers/AbstrFunction.h"
#include "Optimizers/NelderMeadOptimizer.h"
#include "Optimizers/SimpleStochasticOptimizer.h"
#include <sstream>
#include <iomanip>

Optimisation::Optimisation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Optimisation)
{
    ui->setupUi(this);
    connect(this, &Optimisation::AreaChanged, this, &Optimisation::ChangeArea);
    connect(this, &Optimisation::MethodOrFunctionChanged, this, &Optimisation::ChangeParamLabel);

    setOptimisationParameters();

    ChangeArea();
    ChangeParamLabel();

    ui->customPlot->addGraph();
    ui->customPlot->setInteractions(QCP::iSelectPlottables);
    connect(ui->customPlot, &QCustomPlot::mousePress, this, &Optimisation::pointSelected);
}

Optimisation::~Optimisation()
{
    //delete scene;
    delete ui;
}

void Optimisation::ChangeArea() {
    ui->customPlot->xAxis->setRange(area->get_l(0), area->get_r(0));
    ui->customPlot->yAxis->setRange(area->get_l(1), area->get_r(1));
    ui->customPlot->replot();
}

void Optimisation::ChangeParamLabel() {
    std::ostringstream out;
    out << "Function: " << func->name() << "; Method: " << optimizer->name();
    ui->parametersLabel->setText(QString::fromStdString(out.str()));
}


void Optimisation::on_SetMethodButton_clicked()
{
    Dialog_SetMethod dialog(param, this);
    QObject::connect(&dialog, &Dialog_SetMethod::optimisationParametersChanged, this, &Optimisation::setOptimisationParameters);
    dialog.exec();
}


void Optimisation::setOptimisationParameters() {

    if (param.rosenbrock)
        func = std::make_shared<Rosenbrock<>>();
    if (param.sphere)
        func = std::make_shared<Sphere<>>();
    area = std::make_shared<RectArea>(param.lbx, param.lby, param.rtx, param.rty);
    emit AreaChanged();
    if (param.nelder_mead)
        optimizer = std::make_shared<NelderMeadOptimizer>(param.nmalpha, param.nmbeta, param.nmgamma);
    if (param.simple_stochastic)
        optimizer = std::make_shared<SimpleStochasticOptimizer>(param.ssp, param.ssdelta);
    stopcr = std::make_shared<SC>(param.scepsilon, param.scN);
    emit MethodOrFunctionChanged();
}

void Optimisation::pointSelected(QMouseEvent* event) {
    double x = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    double y = ui->customPlot->yAxis->pixelToCoord(event->pos().y());

    optimResult res = optimizer->optimize(func, {{x, y}}, *area, stopcr);
    std::vector<double> xs, ys;
    for (auto iter = res.tr.xs.begin(); iter != res.tr.xs.end(); ++iter) {
        xs.push_back((*iter).at(0));
        ys.push_back((*iter).at(1));
    }
    ui->customPlot->graph(0)->setData(QVector<double>(xs.begin(), xs.end()), QVector<double>(ys.begin(), ys.end()));
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->replot();

    std::ostringstream out;
    out << std::setprecision(4) << "Result. x = (" << res.x.at(0) << ", " << res.x.at(1) << "); f(x) = " << res.f << "; number of iterations = " << res.tr.N;
    ui->resultLabel->setText(QString::fromStdString(out.str()));
}
