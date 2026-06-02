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

    bool isReady()const;

    unsigned long getIntegrationTime();
    unsigned long getMinIntegrationTime();
    unsigned long getMaxIntegrationTime();
    void setIntegrationTime(unsigned long ms);

    void setAverageFactor(unsigned int average);

    std::vector<double> getLastWavelengths() const;
    std::vector<double> getLastSpectrum()const;
    std::size_t getIndexOfWavelenght(double wavelength);

    std::vector<double> getDarkSpectrum() const;
    std::vector<double> getBiasDarkSpectrum() const;
    std::vector<double> getPureDarkSpectrum() const;
    void setDarkSpectrum(const std::vector<double>& darkSpectrum);

    unsigned long detectIntegrationTime();
    std::vector<double> readWaveLengths();
    std::vector<double> readBiasDarkSpectrum();
    std::vector<double> readCalibDarkSpectrum();
    std::vector<double> readCorrectedSpectrum();
    std::vector<double> calcPureDark();
	
private:
	std::vector<double> m_wavelengths;
	std::vector<double> m_biasDarkSpectrum;
	std::vector<double> m_calibDarkSpectrum;
	std::vector<double> m_pureDarkSpectrum;
	std::vector<double> m_darkSpectrum; //corrected final dark spectrum
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

