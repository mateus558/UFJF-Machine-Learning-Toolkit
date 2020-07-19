#ifndef DATASETMANAGER_H
#define DATASETMANAGER_H

#include <QWidget>
#include <QLabel>
#include "core/Data.hpp"
#include "barplot2d.h"

namespace Ui {
class DatasetManager;
}

class DatasetManager : public QWidget
{
    Q_OBJECT

public:
    explicit DatasetManager(QWidget *parent = nullptr);
    ~DatasetManager();

private slots:
    void on_openDatasetPushButton_clicked();

signals:
    void dataLoaded(Data<float> *new_data);

private:
    void buildGUI();

private:
    Ui::DatasetManager *ui;
    Data<float> *dataset;

    QString dataPath, datasetFileName;
    QLabel *nameLabel, *nameLabelValue;
    QLabel *sizeLabel, *sizeLabelValue;
    QLabel *dimLabel, *dimLabelValue;
    QLabel *posSizeLabel, *posSizeLabelValue;
    QLabel *negSizeLabel, *negSizeLabelValue;
    BarPlot2D *barChart;
};

#endif // DATASETMANAGER_H
