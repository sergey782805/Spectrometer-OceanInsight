#pragma once
#include <api/OceanDirectAPI.h>
#include <qobject.h>
#include <qpoint.h>
#include <vector>
#include <memory>
class Spectrometr : QObject
{

	Q_OBJECT

public:
	Spectrometr();
	~Spectrometr();

	const int getdeviceCount();
	const int getDeviceIdCount();
	const char getDeviceName(); // not returning proper value!
	const int getPixelCount();
	const bool isReady();
	QList<QPointF> getNewSpectrum();

	void setIntegrationTime(unsigned long ms);
	void setAverageFactor(unsigned int average);
	
private:
	int m_errorCode;
	int m_deviceCount;
	int m_deviceIdCount;
	std::vector<long> m_deviceIds;
	int m_retrievedIdCount;
	char m_deviceName[32];
	int m_pixelCount;

	bool m_init; // if 0, return

	unsigned int m_averageFactor;
	
	std::vector<double> m_wavelengths;
	int m_wavelengthCount;
	std::vector<double> m_spectrum;
	int m_intensityCount;

	unsigned long m_integrationTimeMicroseconds;
	double m_maxIntensity;

	QList<QPointF> m_convertedSeries;
	// Functions
	int init();
	void readWaveLengths();
	void readSpectrum();
	void vectorToQlist();
};

