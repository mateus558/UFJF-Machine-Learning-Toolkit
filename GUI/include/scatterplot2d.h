#ifndef SCATTERPLOT2D_H
#define SCATTERPLOT2D_H

#include "visualizationbase.h"
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QVector>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
    class ScatterPlot2D;
}

class ScatterPlot2D : public VisualizationBase
{
    Q_OBJECT
public:
    explicit ScatterPlot2D(QWidget *parent = nullptr);
    ~ScatterPlot2D();
private Q_SLOT:
    void printToPdf();
    void changeMarkerSize(const QString&);
    void titleChanged(const QString&);
    void xTitleChanged(const QString&);
    void yTitleChanged(const QString&);
    void changeXmax(const QString&);
    void changeXmin(const QString&);
    void changeYmax(const QString&);
    void changeYmin(const QString&);

private:
    void initGraph() override;
    void addControls() override;
    void populate() override;

private:
    size_t m_markerSize;
    float x_min = 0, x_max = 0, y_min = 0, y_max = 0;
    bool populated;
    QScatterSeries *seriesPos;
    QScatterSeries *seriesNeg;
    QScatterSeries *seriesUnk;
    QChartView *chart;
    Ui::ScatterPlot2D *ui;
};

#endif // SCATTERPLOT2D_H
