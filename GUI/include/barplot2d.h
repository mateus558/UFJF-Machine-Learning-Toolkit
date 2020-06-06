#ifndef BARPLOT2D_H
#define BARPLOT2D_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QScatterSeries>

#include "visualizationbase.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class BarPlot2D;
}

class BarPlot2D : public VisualizationBase
{
    Q_OBJECT

public:
    explicit BarPlot2D(QWidget *parent = nullptr);
    ~BarPlot2D();

private:
    void initGraph() override;
    void addControls() override;
    void populate() override;

private:
    Ui::BarPlot2D *ui;
    QBarSet *posSet;
    QBarSet *negSet;
    QBarSet *unkSet;
    QBarSeries *barSeries;
    QChartView *chartView;
    QChart *chart;
};

#endif // BARPLOT2D_H
