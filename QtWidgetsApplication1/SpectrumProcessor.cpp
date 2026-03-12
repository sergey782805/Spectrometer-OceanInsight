#include "SpectrumProcessor.h"

SpectrumProcessor::SpectrumProcessor():
	m_wavelengths{0.0}, m_spectrum{0.0}
{

}
SpectrumProcessor::~SpectrumProcessor()
{

}


QList<QPointF> SpectrumProcessor::toQList(const std::vector<double>& wavelengths, const std::vector<double>& spectrum)
{
	
	if (wavelengths.size() != spectrum.size() || wavelengths.empty() || spectrum.empty())
		return {};

	QPointF point;
	QList<QPointF> list;
	
	for (std::size_t i{ 0 }; i < wavelengths.size(); ++i)
	{
		point.setX(wavelengths.at(i));
		point.setY(spectrum.at(i));
		list.append(point);
	}

	return list;
}

std::vector<double> SpectrumProcessor::toRelative(const std::vector<double> spectrum)
{
	if (spectrum.empty())
		return {};
	std::vector<double> relativeSpectrum;
	// pass spectrum by reference cause dereference invalid iterator error with *std::max_elemet
	// pass spectrum by value for now.
	double max = *std::max_element(spectrum.begin(), spectrum.end());
	for (const auto& i : spectrum)
	{
		if (i <= max && max > 0.0)
			relativeSpectrum.push_back(i / max);
	}
	return relativeSpectrum;
}
double SpectrumProcessor::PARsum(const std::vector<double>& calibratedSpectrum, const double lo, const double hi)
{
	if (calibratedSpectrum.size() != m_calibrationWavelengts.size()
		|| calibratedSpectrum.empty() || m_calibrationWavelengts.empty()
		|| lo > hi)
		return 0;

	auto itStart{ std::lower_bound(m_calibrationWavelengts.begin(), m_calibrationWavelengts.end(), lo) };
	auto itEnd{ std::lower_bound(m_calibrationWavelengts.begin(), m_calibrationWavelengts.end(), hi) };

	std::size_t startIndex{ static_cast<std::size_t>(std::distance(m_calibrationWavelengts.begin(), itStart)) };
	std::size_t endIndex{ static_cast<std::size_t>(std::distance(m_calibrationWavelengts.begin(), itEnd)) };

	double sum{ 0 };
	for (std::size_t i {startIndex}; i <= endIndex; ++i)
	{
		sum += calibratedSpectrum[i];
	}

	return sum;
}
//add std::vector<double>& calibrateNm, std::vector<double>& calibrateCoeff as fields to the class
std::vector<double> SpectrumProcessor::calibrate(const std::vector<double>& waveLengts, const std::vector<double>& spectrum)
{
	if (m_calibrationWavelengts.empty() || m_calibrationCoeff.empty() 
		|| waveLengts.empty() || spectrum.empty()
		|| m_calibrationCoeff.size() != m_calibrationWavelengts.size())
		return {};

	auto start{ std::lower_bound(waveLengts.begin(), waveLengts.end(), m_calibrationWavelengts.front()) };
	if (start == waveLengts.begin() || start == waveLengts.end())
		return {};
	//std::lower_bound not returning EQUEL value, so -1 here
	const std::size_t startIndex{ static_cast<size_t>(std::distance(waveLengts.begin(), start)) - 1};
	
	const std::size_t endIndex{ startIndex + m_calibrationWavelengts.size() };

	if (startIndex == 0 || endIndex >= waveLengts.size())
		return {};
	

	if (startIndex > endIndex)
		return {};

	//use LowerBound to find nm from calibrationNM in wavelengts with different step.

	//if (clippedSpectrum.size() != calibrateCoeff.size())
	//	return {};
	// average dLambda 0.426922127
	std::vector<double> calibratedSpectrum;
	double dLambda{ 1 };
	calibratedSpectrum.reserve(endIndex - startIndex);
	//for (size_t i{ startIndex }, k{0} ; i < endIndex; ++i, ++k)
	for(std::size_t i{1}, k{0}; i < m_calibrationCoeff.size(); ++i, ++k)
	{
		//calibratedSpectrum.push_back(spectrum[i] * m_calibrationCoeff[k] * 0.426922127);
		dLambda = m_calibrationWavelengts[i] - m_calibrationWavelengts[i - 1]; // wavelengths[i]???
		calibratedSpectrum.push_back(spectrum[startIndex + i - 1] * m_calibrationCoeff[i - 1] * dLambda);
	}
	calibratedSpectrum.push_back(spectrum[endIndex - 1] * m_calibrationCoeff.back() * dLambda);

	return calibratedSpectrum;
}

void SpectrumProcessor::setCalibrationWavelengts(const std::vector<double>& calibrationWavelengts)
{
	m_calibrationWavelengts = calibrationWavelengts;
}
void SpectrumProcessor::setCalibrationCoeff(const std::vector<double>& calibrationCoeff)
{
	m_calibrationCoeff = calibrationCoeff;
}
std::vector<double> SpectrumProcessor::getCalibrationWavelengts()
{
	return m_calibrationWavelengts;
}
std::vector<double> SpectrumProcessor::getCalibrationCoeff()
{
	return m_calibrationCoeff;
}