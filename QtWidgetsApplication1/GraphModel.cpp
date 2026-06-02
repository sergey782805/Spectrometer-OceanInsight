#include "GraphModel.h"
#include <api/OceanDirectAPI.h>
#include <algorithm>
#include <qdebug.h>

GraphModel::GraphModel() : m_maxX {0.0}, m_maxY{1.0}
{
}
GraphModel::~GraphModel()
{
}
double GraphModel::getMaxX() const
{
	return m_maxX;
}
double GraphModel:: getMaxY() const
{
	return m_maxY;
}
Q_INVOKABLE QList<QPointF> GraphModel::getData() const
{
	return m_series;
}
void GraphModel::setMaxX(double x)
{
	m_maxX = x;
	emit maxXChanged(x);
}
void GraphModel::setMaxY(double y)
{
	m_maxY = y;
	emit maxYChanged(y);
}
void GraphModel::setData(QList<QPointF> data)
{
	m_series.clear();

	m_series = data;
	emit dataChanged(data);
}





