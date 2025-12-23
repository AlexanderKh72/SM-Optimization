#include "optimisation.h"
#include "ui_optimisation.h"
#include "dialog_setmethod.h"
#include "Optimizers/AbstrFunction.h"
#include "Optimizers/NelderMeadOptimizer.h"
#include "Optimizers/SimpleStochasticOptimizer.h"
#include <sstream>
#include <iomanip>
#include <limits>

const int N_CONTOUR = 1000;
const int GRADIENT_LEVELS = 100;

Optimisation::Optimisation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Optimisation)
{
    ui->setupUi(this);
    connect(this, &Optimisation::AreaChanged, this, &Optimisation::ChangeArea);
    connect(this, &Optimisation::AreaChanged, this, &Optimisation::drawContour);

    setOptimisationParameters();

    ChangeArea();
    ChangeParamLabel();

    ui->customPlot->addGraph();
    ui->customPlot->setInteractions(QCP::iSelectPlottables);
    connect(ui->customPlot, &QCustomPlot::mousePress, this, &Optimisation::pointSelected);
    connect(ui->customPlot, &QCustomPlot::mouseMove, this, &Optimisation::calculateFunction);
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

void Optimisation::calculateFunction(QMouseEvent* event){
    double x = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    double y = ui->customPlot->yAxis->pixelToCoord(event->pos().y());

    std::ostringstream out;
    out << "f = " << func->operator()({x, y});
    ui->valueLabel->setText(QString::fromStdString(out.str()));
}

QVector<QColor> hsvGradientPalette(int count) {
    QVector<QColor> colors;
    for (int i = 0; i < count; ++i) {
        float hue = 240 * (1 - i / float(count - 1));  // 240°(blue) to 0°(red)
        colors.append(QColor::fromHsv(hue, 255, 255));
    }
    return colors;
}

void Optimisation::drawContour() {
    QImage image(N_CONTOUR, N_CONTOUR, QImage::Format_ARGB32);
    std::vector<std::vector<double>> values;
    double min_value = std::numeric_limits<double>::max();
    double max_value = std::numeric_limits<double>::min();
    values.resize(N_CONTOUR);
    double hx = (param.rtx - param.lbx) / N_CONTOUR, hy = (param.rty - param.lby) / N_CONTOUR;
    for (int i = 0; i < N_CONTOUR; ++i) {
        values[i].resize(N_CONTOUR);
        for (int j = 0; j < N_CONTOUR; ++j) {
            values[i][j] = (*func)({param.lbx + i * hx, param.rty - hy * j});
            min_value = std::min(min_value, values[i][j]);
            max_value = std::max(max_value, values[i][j]);
        }
    }
    auto palette = hsvGradientPalette(GRADIENT_LEVELS);
    for (int i = 0; i < N_CONTOUR; ++i)
        for (int j = 0; j < N_CONTOUR; ++j)
            image.setPixelColor(i, j, palette[std::min<int>(floor((values[i][j] - min_value) / (max_value - min_value) * GRADIENT_LEVELS), GRADIENT_LEVELS - 1)]);
    ui->customPlot->setBackground(QPixmap::fromImage(image), true);
    ui->customPlot->replot();
}

void Optimisation::on_SetMethodButton_clicked()
{
    Dialog_SetMethod dialog(param, this);
    connect(&dialog, &Dialog_SetMethod::optimisationParametersChanged, this, &Optimisation::setOptimisationParameters);
    connect(&dialog, &Dialog_SetMethod::MethodChanged, this, &Optimisation::ChangeParamLabel);
    connect(&dialog, &Dialog_SetMethod::FunctionChanged, this, &Optimisation::ChangeParamLabel);
    connect(&dialog, &Dialog_SetMethod::FunctionChanged, this, &Optimisation::drawContour);

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
