#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scatterplot3d.h"
#include "scatterplot2d.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scatter2d = new ScatterPlot2D(this);
    scatter3d = new ScatterPlot3D(this);
    ui->radioButton2D->setChecked(true);
    set2DVisualization();
    connect(ui->dataWidget, &DatasetManager::dataLoaded, scatter2d, &ScatterPlot2D::updateData);
    connect(ui->dataWidget, &DatasetManager::dataLoaded, scatter3d, &ScatterPlot3D::updateData);
    connect(ui->radioButton2D, &QRadioButton::toggled, this, &MainWindow::set2DVisualization);
    connect(ui->radioButton3D, &QRadioButton::toggled, this, &MainWindow::set3DVisualization);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set2DVisualization()
{
    if(ui->visualizationContainer->count() == 0){
        ui->visualizationContainer->addWidget(scatter2d);
    }else{
        ui->visualizationContainer->replaceWidget(scatter3d, scatter2d);
    }
    scatter2d->setVisible(true);
    if(scatter3d) scatter3d->setVisible(false);
    currentVisualization = scatter2d;
}

void MainWindow::set3DVisualization()
{
    if(ui->visualizationContainer->count() == 0){
        ui->visualizationContainer->addWidget(scatter3d);
    }else{
        ui->visualizationContainer->replaceWidget(scatter2d, scatter3d);
    }
    scatter2d->setVisible(false);
    scatter3d->setVisible(true);
    currentVisualization = scatter3d;
}

