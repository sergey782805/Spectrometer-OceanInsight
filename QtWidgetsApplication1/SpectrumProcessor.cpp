#include "SpectrumProcessor.h"

SpectrumProcessor::SpectrumProcessor()
{
}
SpectrumProcessor::~SpectrumProcessor()
{
}

QList<QPointF> SpectrumProcessor::toQList(const std::vector<double>& wavelengths, const std::vector<double>& spectrum)
{
	if (wavelengths.size() != spectrum.size() || wavelengths.empty() || spectrum.empty())
		return {};

	QList<QPointF> list;
	std::size_t size{ wavelengths.size() };
	list.reserve(static_cast<qsizetype> (size));

	for (std::size_t i{ 0 }; i < size; ++i)
	{
		list.append(QPointF{ wavelengths[i], spectrum[i] });
	}
	return list;
}
std::vector <double>SpectrumProcessor::savitzkyGolayFilter(const std::vector<double>& spectrum, const int windowSize)
{
	std::vector<double> coefficients{};
	double normalization{};
	switch (windowSize)
	{
	case 9:
		coefficients = { -21.0, 14.0, 39.0, 54.0, 59.0, 54.0, 39.0, 14.0, -21.0 };
		normalization = 231.0;
		break;
	case 11:
		coefficients = { -36, 9, 44, 69, 84, 89, 84, 69, 44, 9, -36 };
		normalization = 429.0;
		break;
	case 13:
		coefficients = { -11, 0, 9, 16, 21, 24, 25, 24, 21, 16, 9, 0, -11 };
		normalization = 143.0;
		break;
	case 15:
		coefficients = { -78, -13, 42, 87, 122, 147, 162, 167, 162, 147, 122, 87, 42, -13, -78 };
		normalization = 1105.0;
		break;
	case 17:
		coefficients = { -21, -6, 7, 18, 27, 34, 39, 42, 43, 42, 39, 34, 27, 18, 7, -6, -21 };
		normalization = 323.0;
		break;
	case 19:
		coefficients = { -136, -51, 24, 89, 144, 189, 224, 249, 264, 269, 264, 249, 224, 189, 144, 89, 24, -51, -136 };
		normalization = 2261.0;
		break;
	case 21:
		coefficients = { -171, -76, 9, 84, 149, 204, 249, 284, 309, 324, 329, 324, 309, 284, 249, 204, 149, 84, 9, -76, -171 };
		normalization = 3059.0;
		break;
	case 23:
		coefficients = { -42, -21, -2, 15, 30, 43, 54, 63, 70, 75, 78, 79, 78, 75, 70, 63, 54, 43, 30, 15, -2, -21, -42 };
		normalization = 805.0;
		break;
	case 25: 
		coefficients = { -253, -138, -33, 62, 147, 222, 287, 342, 387, 422, 447, 462, 467, 462, 447, 422, 387, 342, 287, 222, 147, 62, -33, -138, -253 };
		normalization = 5175.0;
		break;
	default:
		break;
	}

	if (spectrum.size() < coefficients.size())
	{
		return {};
	}
	std::vector<double> filteredSpectrum(spectrum.size());

	std::size_t size{ spectrum.size() };
	const int  max{ static_cast<int>((coefficients.size() - 1) / 2) }; 
	const int  min{ -max }; 
	for (std::size_t i{ 0 }; i < size; ++i)
	{
		double newY{0};
		for (int k{ min }; k <= max; ++k)
		{
			double C{ coefficients[k + max] / normalization };
			double Y{ 0 };

			if (i + k < 0)
			{
				Y = spectrum[i + std::abs(k)];
			}
			else if (i + k >= size)
			{
				Y = spectrum[i - k];
			}
			else 
			{
				Y = spectrum[i + k];
			}
			newY += C * Y;
		}
		filteredSpectrum[i] = newY;
	}
	return filteredSpectrum;
}
std::vector<double> SpectrumProcessor::adjustDarkSpectrum(const std::vector<double>& biasDarkSpectrum, const std::vector<double>& pureDarkSpectrum,const unsigned long minTime, const unsigned long targetTime)
{
	if (biasDarkSpectrum.empty() || pureDarkSpectrum.empty())
	{
		return biasDarkSpectrum;
	}

	std::vector<double> adjustedDarkSpectrum(biasDarkSpectrum.size());
	double currentNoiseTime = 0.0;
	if (targetTime > minTime)
	{
		currentNoiseTime = static_cast<double>(targetTime - minTime);
	}
	for (std::size_t i = 0; i < biasDarkSpectrum.size(); ++i)
	{
		adjustedDarkSpectrum[i] = biasDarkSpectrum[i] + (pureDarkSpectrum[i] * currentNoiseTime);
	}
	return adjustedDarkSpectrum;
}
std::vector<double> SpectrumProcessor::toRelative(const std::vector<double>& spectrum)
{
	if (spectrum.empty())
		return {};
	std::vector<double> relativeSpectrum;
	relativeSpectrum.reserve(spectrum.size());
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
std::vector<double> SpectrumProcessor::calibrate(const std::vector<double>& waveLengts, const std::vector<double>& spectrum)
{
	if (m_calibrationWavelengts.empty() || m_calibrationCoeff.empty() 
		|| waveLengts.empty() || spectrum.empty()
		|| m_calibrationCoeff.size() != m_calibrationWavelengts.size())
		return {};

	auto start{ std::lower_bound(waveLengts.begin(), waveLengts.end(), m_calibrationWavelengts.front()) };
	if (start == waveLengts.begin() || start == waveLengts.end())
		return {};
	const std::size_t startIndex{ static_cast<size_t>(std::distance(waveLengts.begin(), start)) - 1};
	const std::size_t endIndex{ startIndex + m_calibrationWavelengts.size() };

	if (startIndex == 0 || endIndex >= waveLengts.size())
		return {};
	
	if (startIndex > endIndex)
		return {};
	std::vector<double> calibratedSpectrum;
	double dLambda{ 1 };
	calibratedSpectrum.reserve(endIndex - startIndex);
	for(std::size_t i{1}, k{0}; i < m_calibrationCoeff.size(); ++i, ++k)
	{
		dLambda = m_calibrationWavelengts[i] - m_calibrationWavelengts[i - 1];
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
