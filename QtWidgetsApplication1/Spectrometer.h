#pragma once
#include <api/OceanDirectAPI.h>
#include <qobject.h>
#include <qpoint.h>
#include <vector>
#include <memory>
#include <math.h>
class Spectrometer : QObject
{
	Q_OBJECT

public:
	Spectrometer();
	~Spectrometer();

	const std::vector<double> getLastWavelengths(); 
	const std::vector<double> getLastSpectrum();
	const unsigned long getIntegrationTime();
	const unsigned long getDarkIntegrationTime();
	const unsigned long getMaxIntegrationTime();
	const unsigned long getMinIntegrationTime();
	std::vector<double> getBiasDarkSpectrum();
	std::vector<double> getPureDarkSpectrum();
	std::vector<double> getDarkSpectrum();
	const unsigned long detectIntegrationTime();
	const bool isReady();
	std::vector<double> readWaveLengths();
	std::vector<double> readBiasDarkSpectrum();
	std::vector<double> readCalibDarkSpectrum();
	std::vector<double> calcPureDark();
	std::vector<double> readDarkSpectrum();
	std::vector<double> readCorrectedSpectrum();
	std::size_t getIndexOfWavelenght(double wavelength);
	void setIntegrationTime(const unsigned long ms); // ??
	void setAverageFactor(const unsigned int average); //?
	
	void setDarkSpectrum(const std::vector<double>& darkSpectrum);
	void setDarkIntegrationTime(const unsigned long newTime);
	
private:
	std::vector<double> m_wavelengths;
	std::vector<double> m_biasDarkSpectrum;
	std::vector<double> m_calibDarkSpectrum;
	std::vector<double> m_pureDarkSpectrum;
	std::vector<double> m_darkSpectrum; // pure dark
	std::vector<double> m_correctedSpectrum;
	std::vector<long> m_deviceIds;
	unsigned long m_integrationTimeMicroseconds;
	unsigned long m_darkIntegrationTimeMicroseconds;
	int m_errorCode;
	int m_pixelCount;
	unsigned int m_averageFactor;
	bool m_init; // if 0, return
	// Functions
	int init();
			
};

