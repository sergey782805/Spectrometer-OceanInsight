#pragma once
#include <vector>
#include <qobject.h>
#include <qpoint.h>

class SpectrumProcessor
{
public:
	SpectrumProcessor();
	~SpectrumProcessor();
	
	QList<QPointF> toQList(const std::vector<double>& wavelengths, const std::vector<double>& spectrum);
	std::vector <double> savitzkyGolayFilter(const std::vector<double>& spectrum, const int windowSize);
	std::vector<double> toRelative(const std::vector<double>& spectrum);
	std::vector<double> adjustDarkSpectrum(const std::vector<double>& biasDarkSpectrum, const std::vector<double>& darkSpectrum,const unsigned long minTime, const unsigned long targetTime);
	double PARsum(const std::vector<double>& calibratedSpectrum, const double lo, const double hi);
	std::vector<double> calibrate(const std::vector<double>& waveLengts, const std::vector<double>& spectrum );

	void setCalibrationWavelengts(const std::vector<double>& calibrationWavelengts);
	void setCalibrationCoeff(const std::vector<double>& calibrationCoeff);
	void setCalibrationIntegrationTime(const unsigned long calibrationIntegrationTime);
private:
	std::vector<double> m_calibrationWavelengts;
	std::vector<double> m_calibrationCoeff;
	unsigned long m_calibrationIntegrationTime;

};

