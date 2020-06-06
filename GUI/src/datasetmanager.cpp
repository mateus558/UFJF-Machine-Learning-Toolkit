#include "datasetmanager.h"
#include "ui_datasetmanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>

DatasetManager::DatasetManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatasetManager),
    dataset(nullptr),
    barChart(new BarPlot2D(this)),
    nameLabel(new QLabel("Dataset: ")),
    nameLabelValue(new QLabel),
    sizeLabel(new QLabel("Size: ")),
    sizeLabelValue(new QLabel),
    dimLabel(new QLabel("Dimensions: ")),
    dimLabelValue(new QLabel),
    posSizeLabel(new QLabel("Positive samples: ")),
    posSizeLabelValue(new QLabel),
    negSizeLabel(new QLabel("Negative samples: ")),
    negSizeLabelValue(new QLabel)
{
    ui->setupUi(this);
    ui->trainSetRadioButton->setChecked(true);
    ui->verticalLayout_2->addWidget(barChart);
    connect(this, &DatasetManager::dataLoaded, barChart, &VisualizationBase::updateData);
}

DatasetManager::~DatasetManager()
{
    delete ui;
    delete dataset;
}

void DatasetManager::on_openDatasetPushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Choose a dataset file",
                                                    QDir::homePath(),
                                                    tr("Dataset file (*.arff *data *txt *csv);; ARFF (*.arff);; DATA (*.data);; CSV (*.csv);; TXT (*.txt)"));
    if(filename != ""){
        if(ui->trainSetRadioButton->isChecked()){
            qDebug() << "train checked.";
        }else if(ui->testSetRadioButton->isChecked()){
            qDebug() << "test checked.";
        }else{
            QMessageBox::warning(
                this,
                tr("Dataset wasn't opened"),
                tr("You must select if it's a train or test set.") );
            return;
        }
        if(dataset){
            delete dataset;
            dataset = nullptr;
        }
        dataPath = filename;
        dataset = new Data<float>(filename.toLocal8Bit().data());

        if(dataset){
            if(ui->label->isVisible())
                ui->label->setVisible(false);
            buildGUI();
        }
        emit dataLoaded(dataset);
    }
}

void DatasetManager::buildGUI()
{
    ui->info_labels_vert->addWidget(nameLabel);
    ui->info_labels_vert->addWidget(sizeLabel);
    ui->info_labels_vert->addWidget(dimLabel);
    ui->info_labels_vert->addWidget(posSizeLabel);
    ui->info_labels_vert->addWidget(negSizeLabel);

    datasetFileName = *(dataPath.split(QString("/")).end()-1);
    nameLabelValue->setText(datasetFileName.toLocal8Bit().data());
    sizeLabelValue->setText(QString::number(dataset->getSize()));
    dimLabelValue->setText(QString::number(dataset->getDim()));
    posSizeLabelValue->setText(QString::number(dataset->getNumberPositivePoints()));
    negSizeLabelValue->setText(QString::number(dataset->getNumberNegativePoints()));

    ui->info_data_vert->addWidget(nameLabelValue);
    ui->info_data_vert->addWidget(sizeLabelValue);
    ui->info_data_vert->addWidget(dimLabelValue);
    ui->info_data_vert->addWidget(posSizeLabelValue);
    ui->info_data_vert->addWidget(negSizeLabelValue);
}
