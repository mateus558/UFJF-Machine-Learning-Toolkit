#include "scatterplot3d.h"
#include "ui_scatterplot3d.h"

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QFontComboBox>
#include <QLineEdit>
#include <QString>
#include <QtSvg>

using namespace QtDataVisualization;

ScatterPlot3D::ScatterPlot3D(QWidget *parent) :
    VisualizationBase(parent),
    ui(new Ui::ScatterPlot3D),
    graph(new Q3DScatter)
{
    ui->setupUi(this);
    container = QWidget::createWindowContainer(graph);
    ui->horizontalLayout->addWidget(container, 1);

    addControls();
    initGraph();
}

void ScatterPlot3D::changePresetCamera()
{
    static int preset = Q3DCamera::CameraPresetFrontLow;

    graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}

void ScatterPlot3D::changeLabelStyle()
{
    graph->activeTheme()->setLabelBackgroundEnabled(!graph->activeTheme()->isLabelBackgroundEnabled());
}

ScatterPlot3D::~ScatterPlot3D()
{
    delete ui;
}

void ScatterPlot3D::changeDotSize(const QString &value)
{
    m_itemSize = value.toFloat();
    if(m_itemSize <= 0.0) return;
    for(int i = 0; i < graph->seriesList().size(); i++){
        graph->seriesList().at(i)->setItemSize(m_itemSize);
    }
}

void ScatterPlot3D::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        m_style = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        for(int i = 0; i < graph->seriesList().size(); i++){
            graph->seriesList().at(i)->setMesh(m_style);
        }
    }
}

void ScatterPlot3D::changeTheme(int theme)
{
    Q3DTheme *currentTheme = graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
}

void ScatterPlot3D::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    graph->setShadowQuality(sq);
}

void ScatterPlot3D::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality shadowQuality)
{
    int quality = int(shadowQuality);
    emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
}

void ScatterPlot3D::changeFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setPointSizeF(m_fontSize);
    graph->activeTheme()->setFont(newFont);
}

void ScatterPlot3D::changeFontSize(int fontsize)
{
    m_fontSize = fontsize;
    QFont font = graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    graph->activeTheme()->setFont(font);
}

void ScatterPlot3D::setBackgroundEnabled(int enabled)
{
    graph->activeTheme()->setBackgroundEnabled((bool)enabled);
}

void ScatterPlot3D::setGridEnabled(int enabled)
{
    graph->activeTheme()->setGridEnabled((bool)enabled);
}

void ScatterPlot3D::setSmoothDots(int smooth)
{
    m_smooth = bool(smooth);
    for(int i = 0; i < graph->seriesList().size(); i++){
        graph->seriesList().at(i)->setMeshSmooth(m_smooth);
    }
}

void ScatterPlot3D::printToImage()
{
    QImage image = graph->renderToImage(64);
    image.save("output.png", 0, 100);
}

QScatter3DSeries *ScatterPlot3D::seriesFactory(){
    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);

    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    series->setItemSize(m_itemSize);

    return series;
}

void ScatterPlot3D::initGraph()
{
    m_itemSize = 0.1;
    graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    QFont font = graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    graph->setTitle(QString("Teste gráfico"));
    graph->activeTheme()->setFont(font);
    graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
    graph->addSeries(seriesFactory());
    graph->axisX()->setTitle("X");
    graph->axisX()->setTitleVisible(true);
    graph->axisY()->setTitle("Y");
    graph->axisY()->setTitleVisible(true);
    graph->axisZ()->setTitle("Z");
    graph->axisZ()->setTitleVisible(true);
}

void ScatterPlot3D::addControls()
{

    QComboBox *themeList = new QComboBox(this);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));
    themeList->setCurrentIndex(6);

    QPushButton *labelButton = new QPushButton(this);
    labelButton->setText(QStringLiteral("Change label style"));

    QCheckBox *smoothCheckBox = new QCheckBox(this);
    smoothCheckBox->setText(QStringLiteral("Smooth dots"));
    smoothCheckBox->setChecked(true);

    QComboBox *itemStyleList = new QComboBox(this);
    itemStyleList->addItem(QStringLiteral("Sphere"), int(QAbstract3DSeries::MeshSphere));
    itemStyleList->addItem(QStringLiteral("Cube"), int(QAbstract3DSeries::MeshCube));
    itemStyleList->addItem(QStringLiteral("Minimal"), int(QAbstract3DSeries::MeshMinimal));
    itemStyleList->addItem(QStringLiteral("Point"), int(QAbstract3DSeries::MeshPoint));
    itemStyleList->setCurrentIndex(0);

    QPushButton *cameraButton = new QPushButton(this);
    cameraButton->setText(QStringLiteral("Change camera preset"));

    QCheckBox *backgroundCheckBox = new QCheckBox(this);
    backgroundCheckBox->setText(QStringLiteral("Show background"));
    backgroundCheckBox->setChecked(true);

    QCheckBox *gridCheckBox = new QCheckBox(this);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);

    QComboBox *shadowQuality = new QComboBox(this);
    shadowQuality->addItem(QStringLiteral("None"));
    shadowQuality->addItem(QStringLiteral("Low"));
    shadowQuality->addItem(QStringLiteral("Medium"));
    shadowQuality->addItem(QStringLiteral("High"));
    shadowQuality->addItem(QStringLiteral("Low Soft"));
    shadowQuality->addItem(QStringLiteral("Medium Soft"));
    shadowQuality->addItem(QStringLiteral("High Soft"));
    shadowQuality->setCurrentIndex(4);

    QFontComboBox *fontList = new QFontComboBox(this);
    fontList->setCurrentFont(QFont("Arial"));

    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setText(QString::number(m_itemSize));
    lineEdit->setValidator(new QDoubleValidator(0, 1, 2, this));

    QPushButton *printButton = new QPushButton(this);
    printButton->setText("Print to Image");

    QHBoxLayout *feats = new QHBoxLayout(this);
    feats->setAlignment(Qt::AlignRight);
    feats->addWidget(new QLabel(QStringLiteral("X:")));
    QLineEdit *axisEdit_x = new QLineEdit(this);
    axisEdit_x->setMaximumWidth(30);
    axisEdit_x->setText(QString::number(axis_x));
    axisEdit_x->setValidator(new QIntValidator(0,1000,this));
    feats->addWidget(axisEdit_x);

    feats->addWidget(new QLabel(QStringLiteral("Y:")));
    QLineEdit *axisEdit_y = new QLineEdit(this);
    axisEdit_y->setMaximumWidth(30);
    axisEdit_y->setText(QString::number(axis_y));
    axisEdit_y->setValidator(new QIntValidator(0,1000,this));
    feats->addWidget(axisEdit_y);

    feats->addWidget(new QLabel(QStringLiteral("Z:")));
    QLineEdit *axisEdit_z = new QLineEdit(this);
    axisEdit_z->setMaximumWidth(30);
    axisEdit_z->setText(QString::number(axis_z));
    axisEdit_z->setValidator(new QIntValidator(0,1000,this));
    feats->addWidget(axisEdit_z);

    ui->verticalLayout->addWidget(labelButton, 0, Qt::AlignTop);
    ui->verticalLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    ui->verticalLayout->addWidget(printButton, 0, Qt::AlignTop);
    ui->verticalLayout->addWidget(backgroundCheckBox);
    ui->verticalLayout->addWidget(gridCheckBox);
    ui->verticalLayout->addWidget(smoothCheckBox, 0, Qt::AlignTop);
    ui->verticalLayout->addWidget(new QLabel(QStringLiteral("Change dot style")));
    ui->verticalLayout->addWidget(itemStyleList);
    ui->verticalLayout->addWidget(new QLabel(QStringLiteral("Change dot size")));
    ui->verticalLayout->addWidget(lineEdit);
    ui->verticalLayout->addWidget(new QLabel(QStringLiteral("Change theme")));
    ui->verticalLayout->addWidget(themeList);
    ui->verticalLayout->addWidget(new QLabel(QStringLiteral("Adjust shadow quality")));
    ui->verticalLayout->addWidget(shadowQuality);
    ui->verticalLayout->addWidget(new QLabel(QStringLiteral("Change font")));
    ui->verticalLayout->addWidget(fontList, 1, Qt::AlignTop);
    ui->verticalLayout->addWidget(new QLabel(QStringLiteral("Change features on axis")));
    ui->verticalLayout->addLayout(feats);

    QObject::connect(printButton, &QPushButton::clicked, this, &ScatterPlot3D::printToImage);
    QObject::connect(cameraButton, &QPushButton::clicked, this, &ScatterPlot3D::changePresetCamera);
    QObject::connect(labelButton, &QPushButton::clicked, this, &ScatterPlot3D::changeLabelStyle);
    QObject::connect(backgroundCheckBox, &QCheckBox::stateChanged, this, &ScatterPlot3D::setBackgroundEnabled);
    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, this, &ScatterPlot3D::setGridEnabled);
    QObject::connect(smoothCheckBox, &QCheckBox::stateChanged, this, &ScatterPlot3D::setSmoothDots);
    QObject::connect(lineEdit, &QLineEdit::textChanged, this, &ScatterPlot3D::changeDotSize);
    QObject::connect(this, &ScatterPlot3D::backgroundEnabledChanged, backgroundCheckBox, &QCheckBox::setChecked);
    QObject::connect(this, &ScatterPlot3D::gridEnabledChanged, gridCheckBox, &QCheckBox::setChecked);
    QObject::connect(itemStyleList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeStyle(int)));
    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeTheme(int)));
    QObject::connect(shadowQuality, SIGNAL(currentIndexChanged(int)), this, SLOT(changeShadowQuality(int)));
    QObject::connect(this, &ScatterPlot3D::shadowQualityChanged, shadowQuality, &QComboBox::setCurrentIndex);
    QObject::connect(graph, &Q3DScatter::shadowQualityChanged, this, &ScatterPlot3D::shadowQualityUpdatedByVisual);
    QObject::connect(fontList, &QFontComboBox::currentFontChanged, this, &ScatterPlot3D::changeFont);
    QObject::connect(this, &ScatterPlot3D::fontChanged, fontList, &QFontComboBox::setCurrentFont);
    QObject::connect(axisEdit_x, &QLineEdit::textChanged, this, &ScatterPlot3D::xAxisChanged);
    QObject::connect(axisEdit_y, &QLineEdit::textChanged, this, &ScatterPlot3D::yAxisChanged);
    QObject::connect(axisEdit_z, &QLineEdit::textChanged, this, &ScatterPlot3D::zAxisChanged);
}

void ScatterPlot3D::populate()
{
    size_t i, size = m_data->getSize(), sizePos = m_data->getNumberPositivePoints(),
           sizeNeg = m_data->getNumberNegativePoints();

    QScatterDataArray *dataPosArray = new QScatterDataArray;;
    QScatterDataItem *ptrToDataPosArray;
    if(sizeNeg > 0){
        dataPosArray->resize(sizePos);
        ptrToDataPosArray = &dataPosArray->first();
        graph->addSeries(seriesFactory());
    }

    QScatterDataArray *dataNegArray = new QScatterDataArray;
    QScatterDataItem *ptrToDataNegArray;
    if(sizeNeg > 0){
        dataNegArray->resize(sizeNeg);
        ptrToDataNegArray = &dataNegArray->first();
        graph->addSeries(seriesFactory());
    }

    size_t sizeUnk = size - (sizeNeg + sizePos);
    QScatterDataArray *dataUnkArray = new QScatterDataArray;
    QScatterDataItem *ptrToDataUnkArray;
    if(sizeUnk > 0){
        dataUnkArray->resize(size - (sizeNeg + sizePos));
        ptrToDataUnkArray = &dataUnkArray->first();
        graph->addSeries(seriesFactory());
    }

    for(i = 0; i < size; i++){
        float x = (*(*m_data)[i])[axis_x];
        float y = (*(*m_data)[i])[axis_y];
        float z = (*(*m_data)[i])[axis_z];
        int class_y = (*m_data)[i]->y;
        QVector3D sample(x, y, z);

        if(class_y == 1){
            ptrToDataPosArray->setPosition(sample);
            ptrToDataPosArray++;
        }else if(class_y == -1){
            ptrToDataNegArray->setPosition(sample);
            ptrToDataNegArray++;
        }else{
            ptrToDataUnkArray->setPosition(sample);
            ptrToDataUnkArray++;
        }
    }
    graph->seriesList().at(0)->dataProxy()->resetArray(dataPosArray);
    graph->seriesList().at(0)->setBaseColor(Qt::blue);
    graph->seriesList().at(1)->dataProxy()->resetArray(dataNegArray);
    graph->seriesList().at(1)->setBaseColor(Qt::red);
    graph->seriesList().at(2)->dataProxy()->resetArray(dataUnkArray);
    graph->seriesList().at(2)->setBaseColor(Qt::white);
}
