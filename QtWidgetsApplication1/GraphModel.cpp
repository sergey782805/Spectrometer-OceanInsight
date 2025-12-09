#include "GraphModel.h"
#include <api/OceanDirectAPI.h>
#include <algorithm>
#include <qdebug.h>

GraphModel::GraphModel() : m_maxX {5.0}, m_maxY{1.0}
{

	//init();

	//setIngertationTime(800000); //2

	//readWaveLengths();
	//readSpectrum();

	//vectorToQlist();

}
GraphModel::~GraphModel()
{
	
	
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





