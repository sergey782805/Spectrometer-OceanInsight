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

	std::vector<double> toRelative(const std::vector<double> spectrum);

	double PARsum(const std::vector<double>& calibratedSpectrum, const double lo, const double hi);
	std::vector<double> calibrate(const std::vector<double>& waveLengts, const std::vector<double>& spectrum );

	void setCalibrationWavelengts(const std::vector<double>& calibrationWavelengts);
	void setCalibrationCoeff(const std::vector<double>& calibrationCoeff);
	std::vector<double> getCalibrationWavelengts();
	std::vector<double> getCalibrationCoeff();
private:

	std::vector<double> m_wavelengths; // should I use this to store last read and last opened from file spectrum?
	std::vector<double> m_spectrum; // when I'm saving spectrum, get data from here?

	std::vector<double> m_calibrationWavelengts;
	std::vector<double> m_calibrationCoeff;

};

