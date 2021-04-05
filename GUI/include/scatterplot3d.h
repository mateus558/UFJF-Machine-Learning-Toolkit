#ifndef SCATTERPLOT3D_H
#define SCATTERPLOT3D_H

#include <QWidget>
#include <QtDataVisualization/Q3DScatter>

#include "visualizationbase.h"
#include "core/Data.hpp"

namespace Ui {
class ScatterPlot3D;
}

class ScatterPlot3D : public VisualizationBase
{
    Q_OBJECT

public:
    explicit ScatterPlot3D(QWidget *parent = nullptr);
    ~ScatterPlot3D();

public Q_SLOTS:
    void changeDotSize(const QString &);
    void changeStyle(int style);
    void changePresetCamera();
    void changeLabelStyle();
    void changeTheme(int theme);
    void changeShadowQuality(int quality);
    void shadowQualityUpdatedByVisual(QtDataVisualization::QAbstract3DGraph::ShadowQuality shadowQuality);
    void changeFont(const QFont &font);
    void changeFontSize(int fontsize);
    void setBackgroundEnabled(int enabled);
    void setGridEnabled(int enabled);
    void setSmoothDots(int smooth);
    void printToImage();

Q_SIGNALS:
    void backgroundEnabledChanged(bool enabled);
    void gridEnabledChanged(bool enabled);
    void shadowQualityChanged(int quality);
    void fontChanged(QFont font);

private:
    void initGraph() override;
    void addControls() override;
    void populate() override;
    QtDataVisualization::QScatter3DSeries *seriesFactory();

private:
    QWidget *container;
    QtDataVisualization::Q3DScatter *graph;
    Ui::ScatterPlot3D *ui;
};

#endif // SCATTERPLOT3D_H
