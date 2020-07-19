#include "scatterplot2d.h"
#include "ui_scatterplot2d.h"

#include<QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QPdfWriter>
#include <QPushButton>
#include <QGraphicsLayout>


ScatterPlot2D::ScatterPlot2D(QWidget *parent) :
    VisualizationBase(parent),
    ui(new Ui::ScatterPlot2D),
    seriesPos(nullptr),
    seriesNeg(nullptr),
    seriesUnk(nullptr),
    m_markerSize(10),
    populated(false),
    chart(new QChartView(new QChart()))
{
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(chart, 1);
    addControls();
    initGraph();
}

ScatterPlot2D::~ScatterPlot2D()
{
    delete ui;
}

void ScatterPlot2D::printToPdf()
{
    QPdfWriter writer("out.pdf");
    writer.setPageSize(QPagedPaintDevice::A4);
    writer.setResolution(10000000);
    QPainter painter(&writer);
    QPixmap pix = chart->grab();
    int h_old = chart->height();
    int w_old = chart->width();
    int h = painter.window().height()*0.4;
    int w = h * 1.3;
    int x = (painter.window().width() / 2) - (w/2);
    int y = (painter.window().height() / 2) - (h/2);
    chart->resize(w, h);
    painter.drawPixmap(x, y, w, h, pix);
    chart->resize(w_old, h_old);
}

void ScatterPlot2D::changeMarkerSize(const QString &value)
{
    m_markerSize = value.toInt();
    seriesPos->setMarkerSize(m_markerSize);
    seriesNeg->setMarkerSize(m_markerSize);
    seriesUnk->setMarkerSize(m_markerSize);
}

void ScatterPlot2D::titleChanged(const QString &title)
{
    chart->chart()->setTitle(title);
}

void ScatterPlot2D::xTitleChanged(const QString &title)
{
    if(!populated) return;
    chart->chart()->axisX()->setTitleText(title);
}

void ScatterPlot2D::yTitleChanged(const QString &title)
{
    if(!populated) return ;
    chart->chart()->axisY()->setTitleText(title);
}

void ScatterPlot2D::changeXmax(const QString &value)
{
    x_max = value.toFloat();
    if(x_max == 0) return;
    chart->chart()->axisX()->setRange(x_min, x_max);
}

void ScatterPlot2D::changeXmin(const QString &value)
{
    x_min = value.toFloat();
    if(x_min == 0) return;
    chart->chart()->axisX()->setRange(x_min, x_max);
}

void ScatterPlot2D::changeYmax(const QString &value)
{
    y_max = value.toFloat();
    if(y_max == 0) return;
    chart->chart()->axisY()->setRange(y_min, y_max);
}

void ScatterPlot2D::changeYmin(const QString &value)
{
    y_min = value.toFloat();
    if(y_min == 0) return;
    chart->chart()->axisY()->setRange(y_min, y_max);
}

void ScatterPlot2D::initGraph()
{
    chart->setRenderHint(QPainter::Antialiasing);
    chart->chart()->layout()->setContentsMargins(0,0,0,0);
    chart->chart()->setTitle("Simple scatterchart example");
    chart->chart()->createDefaultAxes();
    chart->chart()->setDropShadowEnabled(true);
    chart->chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
}

void ScatterPlot2D::addControls()
{
    QHBoxLayout *feats = new QHBoxLayout(this);
    feats->setAlignment(Qt::AlignRight);
    feats->addWidget(new QLabel(QStringLiteral("X:")));
    QLineEdit *axisEdit_x = new QLineEdit(this);
    axisEdit_x->setMaximumWidth(30);
    axisEdit_x->setText(QString::number(axis_x));
    axisEdit_x->setValidator(new QIntValidator(0,10000,this));
    feats->addWidget(axisEdit_x);

    feats->addWidget(new QLabel(QStringLiteral("Y:")));
    QLineEdit *axisEdit_y = new QLineEdit(this);
    axisEdit_y->setMaximumWidth(30);
    axisEdit_y->setText(QString::number(axis_y));
    axisEdit_y->setValidator(new QIntValidator(0,10000,this));
    feats->addWidget(axisEdit_y);

    QVBoxLayout *vertF = new QVBoxLayout(this);
    vertF->setAlignment(Qt::AlignBottom);
    vertF->addWidget(new QLabel(QStringLiteral("Change axis feats")));
    vertF->addLayout(feats);

    QLineEdit *markerSize = new QLineEdit(this);
    markerSize->setMaximumWidth(30);
    markerSize->setText(QString::number(m_markerSize));
    markerSize->setValidator(new QIntValidator(0,50,this));

    QHBoxLayout *markerSizeLay = new QHBoxLayout(this);
    markerSizeLay->setAlignment(Qt::AlignTop);
    markerSizeLay->addWidget(new QLabel(QStringLiteral("Marker size:")));
    markerSizeLay->addWidget(markerSize);

    QLineEdit *titleEdit = new QLineEdit(this);
    titleEdit->setMaximumWidth(80);
    titleEdit->setText(chart->chart()->title());

    QHBoxLayout *titleEditLay = new QHBoxLayout(this);
    titleEditLay->setAlignment(Qt::AlignTop);
    titleEditLay->addWidget(new QLabel(QStringLiteral("Title:")));
    titleEditLay->addWidget(titleEdit);

    QLineEdit *xTitleEdit = new QLineEdit(this);
    xTitleEdit->setMaximumWidth(80);
    xTitleEdit->setText("X");

    QHBoxLayout *xTitleEditLay = new QHBoxLayout(this);
    xTitleEditLay->setAlignment(Qt::AlignTop);
    xTitleEditLay->addWidget(new QLabel(QStringLiteral("X Title:")));
    xTitleEditLay->addWidget(xTitleEdit);

    QLineEdit *yTitleEdit = new QLineEdit(this);
    yTitleEdit->setMaximumWidth(80);
    yTitleEdit->setText("Y");

    QHBoxLayout *yTitleEditLay = new QHBoxLayout(this);
    yTitleEditLay->setAlignment(Qt::AlignTop);
    yTitleEditLay->addWidget(new QLabel(QStringLiteral("Y Title:")));
    yTitleEditLay->addWidget(yTitleEdit);

    QLineEdit *minXedit = new QLineEdit(this);
    minXedit->setMaximumWidth(30);
    minXedit->setValidator(new QDoubleValidator(-1000000,1000000,2,this));
    minXedit->setText(QString::number(x_min));

    QLineEdit *maxXedit = new QLineEdit(this);
    maxXedit->setMaximumWidth(30);
    maxXedit->setValidator(new QDoubleValidator(-1000000,1000000,2,this));
    maxXedit->setText(QString::number(x_max));

    QHBoxLayout *xLimitsLay = new QHBoxLayout(this);
    xLimitsLay->setAlignment(Qt::AlignTop);
    xLimitsLay->addWidget(new QLabel(QStringLiteral("min:")));
    xLimitsLay->addWidget(minXedit);
    xLimitsLay->addWidget(new QLabel(QStringLiteral("max:")));
    xLimitsLay->addWidget(maxXedit);

    QLineEdit *minYedit = new QLineEdit(this);
    minYedit->setMaximumWidth(30);
    minYedit->setValidator(new QDoubleValidator(-1000000,1000000,2,this));
    minYedit->setText(QString::number(y_min));

    QLineEdit *maxYedit = new QLineEdit(this);
    maxYedit->setMaximumWidth(30);
    maxYedit->setValidator(new QDoubleValidator(-1000000,1000000,2,this));
    maxYedit->setText(QString::number(y_max));

    QHBoxLayout *yLimitsLay = new QHBoxLayout(this);
    yLimitsLay->setAlignment(Qt::AlignTop);
    yLimitsLay->addWidget(new QLabel(QStringLiteral("min:")));
    yLimitsLay->addWidget(minYedit);
    yLimitsLay->addWidget(new QLabel(QStringLiteral("max:")));
    yLimitsLay->addWidget(maxYedit);

    QPushButton *pdfButton = new QPushButton(this);
    pdfButton->setText("Print to PDF");

    ui->verticalLayout->setAlignment(Qt::AlignTop);
    ui->verticalLayout->addWidget(pdfButton);
    ui->verticalLayout->addLayout(titleEditLay);
    ui->verticalLayout->addLayout(xTitleEditLay);
    ui->verticalLayout->addLayout(yTitleEditLay);
    ui->verticalLayout->addLayout(markerSizeLay);
    ui->verticalLayout->addLayout(vertF);
    ui->verticalLayout->addWidget(new QLabel(QStringLiteral("Axis X limits")));
    ui->verticalLayout->addLayout(xLimitsLay);
    ui->verticalLayout->addWidget(new QLabel(QStringLiteral("Axis Y limits")));
    ui->verticalLayout->addLayout(yLimitsLay);

    connect(pdfButton, &QPushButton::clicked, this, &ScatterPlot2D::printToPdf);
    connect(axisEdit_x, &QLineEdit::textChanged, this, &ScatterPlot2D::xAxisChanged);
    connect(axisEdit_y, &QLineEdit::textChanged, this, &ScatterPlot2D::yAxisChanged);
    connect(markerSize, &QLineEdit::textChanged, this, &ScatterPlot2D::changeMarkerSize);
    connect(titleEdit, &QLineEdit::textChanged, this, &ScatterPlot2D::titleChanged);
    connect(xTitleEdit, &QLineEdit::textChanged, this, &ScatterPlot2D::xTitleChanged);
    connect(yTitleEdit, &QLineEdit::textChanged, this, &ScatterPlot2D::yTitleChanged);
    connect(maxYedit, &QLineEdit::textChanged, this, &ScatterPlot2D::changeYmax);
    connect(minYedit, &QLineEdit::textChanged, this, &ScatterPlot2D::changeYmin);
    connect(maxXedit, &QLineEdit::textChanged, this, &ScatterPlot2D::changeXmax);
    connect(minXedit, &QLineEdit::textChanged, this, &ScatterPlot2D::changeXmin);
}

void ScatterPlot2D::populate()
{
    size_t i, size = m_data->getSize();
    float min_x = 100000000, max_x = -100000000, min_y = 100000000, max_y = -100000000;

    if(!seriesPos){
        seriesPos = new QScatterSeries;
        seriesPos->setName("Positive");
        seriesPos->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        seriesPos->setMarkerSize(m_markerSize);
        seriesPos->setColor(QColor(Qt::blue));
        chart->chart()->addSeries(seriesPos);
    }else{
        seriesPos->clear();
    }
    if(!seriesNeg){
        seriesNeg = new QScatterSeries;
        seriesNeg->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        seriesNeg->setMarkerSize(m_markerSize);
        seriesNeg->setName("Negative");
        seriesNeg->setColor(QColor(Qt::red));
        chart->chart()->addSeries(seriesNeg);
    }else{
        seriesNeg->clear();
    }
    if(!seriesUnk){
        seriesUnk = new QScatterSeries;
        seriesUnk->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        seriesUnk->setMarkerSize(m_markerSize);
        seriesUnk->setName("Unknown");
        chart->chart()->addSeries(seriesUnk);
    }else{
        seriesUnk->clear();
    }

    for(i = 0; i < size; i++){
        float x = (*(*m_data)[i])[axis_x];
        float y = (*(*m_data)[i])[axis_y];
        if(x > max_x){
            max_x = x;
        }else if(x < min_x){
            min_x = x;
        }
        if(y > max_y){
            max_y = y;
        }else if(y < min_y){
            min_y = y;
        }
        if((*m_data)[i]->y == 1){
            *seriesPos << QPointF(x, y);
        }else if((*m_data)[i]->y == -1){
            *seriesNeg << QPointF(x, y);
        }else{
            *seriesUnk << QPointF(x, y);
        }
    }

    x_min = min_x; x_max = max_x;
    y_min = min_y; y_max = max_y;
    chart->chart()->createDefaultAxes();
    chart->chart()->axes(Qt::Horizontal).first()->setRange(min_x, max_x);
    chart->chart()->axes(Qt::Vertical).first()->setRange(min_y, max_y);
    populated = true;
}



