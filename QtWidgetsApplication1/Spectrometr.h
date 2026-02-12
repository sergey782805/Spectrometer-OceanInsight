#pragma once
#include <api/OceanDirectAPI.h>
#include <qobject.h>
#include <qpoint.h>
#include <vector>
#include <memory>
#include <math.h>
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
	std::vector<double> readWaveLengths();
	std::vector<double> readSpectrum();
	std::vector<double> readDarkSpectrum();
	std::vector<double> readCorrectedSpectrum();

	std::size_t getIndexOfWavelenght(double wavelength);

	void setIntegrationTime(const unsigned long ms); // ??
	void setAverageFactor(const unsigned int average); //?
	
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
	std::vector<double> m_spectrum; // not used?
	int m_intensityCount;
	std::vector<double> m_darkSpectrum;


	unsigned long m_integrationTimeMicroseconds; // ??
	double m_maxIntensity; // not used

	// Functions
	int init();
	
	
	
				
};

