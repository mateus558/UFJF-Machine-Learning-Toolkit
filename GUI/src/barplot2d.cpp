#include "barplot2d.h"
#include "ui_barplot2d.h"

#include <QValueAxis>
#include <QGraphicsLayout>
#include <QBarCategoryAxis>

BarPlot2D::BarPlot2D(QWidget *parent) :
    VisualizationBase(parent),
    ui(new Ui::BarPlot2D),
    posSet(nullptr),
    negSet(nullptr),
    unkSet(nullptr),
    chartView(nullptr)
{
    ui->setupUi(this);

    initGraph();
}

BarPlot2D::~BarPlot2D()
{
    delete ui;
}

void BarPlot2D::initGraph()
{
    chart = new QChart();
    barSeries = new QBarSeries;
    chart->addSeries(barSeries);
    //chart->setTitle("Simple barchart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->layout()->setContentsMargins(0,0,0,0);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

}

void BarPlot2D::addControls()
{

}

void BarPlot2D::populate()
{
    size_t size = m_data->getSize();
    size_t pos_size = m_data->getNumberPositivePoints(), neg_size = m_data->getNumberNegativePoints();
    size_t unk_size = size - (pos_size + neg_size);
    if(!posSet && pos_size > 0){
        posSet = new QBarSet("Positive");
        *posSet << pos_size;
        barSeries->append(posSet);
    }
    if(!negSet && neg_size > 0){
        negSet = new QBarSet("Negative");
        *negSet << neg_size;
        barSeries->append(negSet);
    }
    if(!unkSet && unk_size > 0){
        unkSet = new QBarSet("Unknown");
        *unkSet << unk_size;
        barSeries->append(unkSet);
    }
    QStringList categories;
    categories << "Samples ratio";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, size);
    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    if(!chartView){
        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        ui->horizontalLayout->addWidget(chartView, 1);
    }
}
