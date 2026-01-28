#pragma once
#include <qobject.h>
#include <vector>
#include <qlist.h>
#include <qpoint.h>
#include <String>
#include <QtQmlIntegration/qqmlintegration.h>

class GraphModel : public QObject
{

	Q_OBJECT
	Q_PROPERTY(double getMaxX READ getMaxX WRITE setMaxX NOTIFY maxXChanged)
	Q_PROPERTY(double getMaxY READ getMaxY WRITE setMaxY NOTIFY maxYChanged)
	Q_PROPERTY(QList<QPointF> getData READ getData WRITE setData NOTIFY dataChanged)

	
public:
	GraphModel();
	~GraphModel();

	double getMaxX()
	{
		return m_maxX;
	}
	double getMaxY()
	{
		return m_maxY;
	}
	
	Q_INVOKABLE QList<QPointF> getData()
	{
		return m_series;
	}

public slots:
	void setMaxX(double x);
	void setMaxY(double y);
	void setData(QList<QPointF> data);
	
	//void updateSeries();

signals:
	void maxXChanged(double newMaxX);
	void maxYChanged(double newMaxY);
	void dataChanged(QList<QPointF> data);

private:

	double m_maxX;
	double m_maxY;
	
	// Try QList<QVariant>
	QList<QPointF> m_series;
	
	
	//spectrometr parameters
	
	
};

