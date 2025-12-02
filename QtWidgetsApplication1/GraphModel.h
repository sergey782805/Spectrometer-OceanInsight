#pragma once
#include <qobject.h>
#include <vector>
#include <qlist.h>
#include <qpoint.h>
#include <String>
class GraphModel : public QObject
{

	Q_OBJECT
	Q_PROPERTY(double getMaxX READ getMaxX WRITE setMaxX NOTIFY maxXChanged)
	Q_PROPERTY(double getMaxY READ getMaxY WRITE setMaxY NOTIFY maxYChanged)
	Q_PROPERTY(QList<QPointF> qList READ qList)

		//QML_ELEMENT
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

	int getdeviceCount()
	{
		return m_deviceCount;
	}
	int getDeviceIdCount()
	{
		return m_deviceIdCount;
	}
	char getDeviceName()
	{
		return m_deviceName[32];
	}
	int getPixelCount()
	{
		return m_pixelCount;
	}
	bool isReady()
	{
		return m_init;
	}
	Q_INVOKABLE QList<QPointF> qList()
	{
		return m_series;
	}
	

	void setIngertationTime(unsigned long ms);

public slots:
	void setMaxX(double x);
	void setMaxY(double y);
	
	//void updateSeries();

signals:
	void maxXChanged(double newMaxX);
	void maxYChanged(double newMaxY);


private:

	double m_maxX;
	double m_maxY;
	std::vector<double> m_wavelengths;
	int m_wavelengthCount;
	std::vector<double> m_spectrum;
	int m_intensityCount;
	//qlist<> list
	QList<QPointF> m_series;

	//spectrometr parameters
	int m_errorCode;
	int m_deviceCount;
	int m_deviceIdCount;
	std::vector<long> m_deviceIds;
	int m_retrievedIdCount;
	char m_deviceName[32];
	int m_pixelCount;

	bool m_init; // if 0, return
	unsigned long m_integrationTimeMicroseconds;


	int init();




	void readWaveLengths();
	void readSpectrum();

	void vectorToQlist();
	
};

