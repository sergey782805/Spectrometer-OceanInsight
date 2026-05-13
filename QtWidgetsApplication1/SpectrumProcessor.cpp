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

	//QPointF point;
	QList<QPointF> list;
	std::size_t size{ wavelengths.size() };
	list.reserve(static_cast<qsizetype> (size));

	for (std::size_t i{ 0 }; i < size; ++i)
	{
		//point.setX(wavelengths[i]);
		//point.setY(spectrum[i]);
		//list.append(point);
		list.append(QPointF{ wavelengths[i], spectrum[i] });
	}

	return list;
}

std::vector <double>SpectrumProcessor::savitzkyGolayFilter9(const std::vector<double>& spectrum)
{
	const std::vector<double> coefficients{ -21.0, 14.0, 39.0, 54.0, 59.0, 54.0, 39.0, 14.0, -21.0 };
	//
	if (spectrum.size() < coefficients.size())
	{
		return {};
	}
	const double normalisation{ 231.0 };
	std::vector<double> filteredSpectrum(spectrum.size());

	std::size_t size{ spectrum.size() };
	const int  max{ static_cast<int>((coefficients.size() - 1) / 2) }; 
	const int  min{ -max }; 
	for (std::size_t i{ 0 }; i < size; ++i)
	{
		double newY{0};
		for (int k{ min }; k <= max; ++k)
		{
			double C{ coefficients[k + max] / normalisation };
			double Y{ 0 };

			if (i + k < 0)
			{
				
				//i = 0
				// k = -2
				//y+2 y+1 y y+1 y+2

				Y = spectrum[i + std::abs(k)];
			}
			else if (i + k >= size)
			{
				// [1, 2, 3, 4, 5]
				// 3, 4, 5, 4, 3
				//i = 4;
				// k = 1
				// 
				Y = spectrum[i - k];
			}
			else 
			{
				Y = spectrum[i + k];
			}

			newY += C * Y; //spectrum[i + k];
		}
		filteredSpectrum[i] = newY;

	}

	return filteredSpectrum;
}


std::vector<double> SpectrumProcessor::adjustDarkSpectrum(const std::vector<double>& darkSpectrum,const unsigned long oldTime, const unsigned long newTime)
{
	if (oldTime == 0) 
		return darkSpectrum;

	const double coefficient{ static_cast<double> (newTime) / oldTime };

	std::vector<double> adjustedDarkSpectrum{ darkSpectrum };

	for (auto& value : adjustedDarkSpectrum)
	{
		value *= coefficient;
	}
	return adjustedDarkSpectrum;
}

std::vector<double> SpectrumProcessor::toRelative(const std::vector<double>& spectrum)
{
	if (spectrum.empty())
		return {};
	std::vector<double> relativeSpectrum;
	relativeSpectrum.reserve(spectrum.size());
	// pass spectrum by reference cause dereference invalid iterator error with *std::max_elemet
	// pass spectrum by value for now.
	double max = *std::max_element(spectrum.begin(), spectrum.end());
	if (max == 0.0)
	{
		return {};
	}
	for (const auto& i : spectrum)
	{
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