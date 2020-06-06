#include "visualizationbase.h"

VisualizationBase::VisualizationBase(QWidget *parent) :
    QWidget(parent),
    axis_x(0), axis_y(1), axis_z(2),
    m_fontSize(2),
    m_itemSize(0.4),
    m_data(nullptr),
    m_smooth(true)
{
}

VisualizationBase::~VisualizationBase()
{
}

void VisualizationBase::updateData(Data<float> *new_data)
{
    setData(new_data);
}

void VisualizationBase::setData(Data<float> *data)
{
    m_data = data;
    populate();
}

Data<float> *VisualizationBase::data() const
{
    return m_data;
}

void VisualizationBase::xAxisChanged(const QString &value)
{
    size_t temp = value.toInt();

    if(temp == axis_x) return;
    else if(temp > m_data->getDim()) return ;
    else axis_x = temp;

    populate();
}

void VisualizationBase::yAxisChanged(const QString &value)
{
    size_t temp = value.toInt();

    if(temp == axis_y) return;
    else if(temp > m_data->getDim()) return ;
    else axis_y = temp;

    populate();
}

void VisualizationBase::zAxisChanged(const QString &value)
{
    size_t temp = value.toInt();

    if(temp == axis_z) return;
    else if(temp > m_data->getDim()) return ;
    else axis_z = temp;

    populate();
}
