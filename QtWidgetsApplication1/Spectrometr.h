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

	const std::vector<double> getLastWavelengths(); 
	const std::vector<double> getLastSpectrum();
	const unsigned long getIntegrationTime();
	const unsigned long getMaxIntegrationTime();
	const unsigned long getMinIntegrationTime();
	const unsigned long detectIntegrationTime();
	const bool isReady();
	std::vector<double> readWaveLengths();
	std::vector<double> readDarkSpectrum();
	std::vector<double> readCorrectedSpectrum();
	std::size_t getIndexOfWavelenght(double wavelength);
	void setIntegrationTime(const unsigned long ms); // ??
	void setAverageFactor(const unsigned int average); //?
	
private:
	std::vector<double> m_wavelengths;
	std::vector<double> m_darkSpectrum;
	std::vector<double> m_correctedSpectrum;
	std::vector<long> m_deviceIds;
	unsigned long m_integrationTimeMicroseconds;
	int m_errorCode;
	int m_pixelCount;
	unsigned int m_averageFactor;
	bool m_init; // if 0, return
	// Functions
	int init();
			
};

