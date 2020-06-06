#ifndef VISUALIZATIONBASE_H
#define VISUALIZATIONBASE_H

#include <core/Data.hpp>

#include <QWidget>
#include <QtDataVisualization/QAbstract3DSeries>

class VisualizationBase : public QWidget
{
    Q_OBJECT

public:
    explicit VisualizationBase(QWidget *parent = nullptr);
    ~VisualizationBase();

    Data<float> *data() const;

public Q_SLOTS:
    void updateData(Data<float> *new_data);
    void xAxisChanged(const QString &);
    void yAxisChanged(const QString &);
    void zAxisChanged(const QString &);

protected:
    virtual void populate() = 0;
    virtual void initGraph() = 0;
    virtual void addControls() = 0;
    virtual void setData(Data<float> *data);

protected:
    int m_fontSize;
    float m_itemSize;
    bool m_smooth;
    size_t m_markerSize;
    size_t axis_x, axis_y, axis_z;
    Data<float> *m_data;
    QtDataVisualization::QAbstract3DSeries::Mesh m_style;
};

#endif // VISUALIZATIONBASE_H
