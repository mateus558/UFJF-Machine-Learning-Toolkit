#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization/q3dscatter.h>

class VisualizationBase;
class ScatterPlot2D;
class ScatterPlot3D;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private Q_SLOTS:
    void set2DVisualization();
    void set3DVisualization();

private:
    Ui::MainWindow *ui;
    VisualizationBase *currentVisualization;
    ScatterPlot2D *scatter2d;
    ScatterPlot3D *scatter3d;
    QtDataVisualization::Q3DScatter *graph;
};
#endif // MAINWINDOW_H
