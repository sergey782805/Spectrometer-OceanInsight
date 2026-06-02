#include "Spectrometer.h"


Spectrometer::Spectrometer() :
	m_averageFactor{ 1 }, m_integrationTimeMicroseconds{ 400000 }, m_pixelCount{ 0 }
{
	init(); // searching spectrometr during program start
	m_darkSpectrum.resize(m_pixelCount);

}
Spectrometer::~Spectrometer()
{
	m_wavelengths.clear();
	if (isReady())
	{
		odapi_close_device(m_deviceIds[0], &m_errorCode);
		odapi_shutdown();
	}
}
//GET functions PUBLIC


bool Spectrometer::isReady() const
{
	return m_init;
}
std::vector<double> Spectrometer::getLastWavelengths() const
{
	return m_wavelengths;
}
std::vector<double> Spectrometer::getLastSpectrum() const
{
	return m_correctedSpectrum;
}
unsigned long Spectrometer::getIntegrationTime()
{
	if (isReady())
	{
		return odapi_get_integration_time_micros(m_deviceIds[0], &m_errorCode);
	}
	return 0;
}
unsigned long Spectrometer::getMaxIntegrationTime()
{
	if (isReady())
	{
		return odapi_get_maximum_integration_time_micros(m_deviceIds[0], &m_errorCode);
	}
	return 0;
}
unsigned long Spectrometer::getMinIntegrationTime()
{
	if (isReady())
	{
		if (m_averageFactor > 1)
		{
			return odapi_get_minimum_averaging_integration_time_micros(m_deviceIds[0], &m_errorCode);
		}
		return odapi_get_minimum_integration_time_micros(m_deviceIds[0], &m_errorCode);
	}
	return 0;
}
std::vector<double> Spectrometer::getBiasDarkSpectrum() const
{
	return m_biasDarkSpectrum;
}
std::vector<double> Spectrometer::getPureDarkSpectrum() const
{
	return m_pureDarkSpectrum;
}
std::vector<double> Spectrometer::getDarkSpectrum() const
{
	if(isReady())
		return m_darkSpectrum;
	return {};
}
unsigned long Spectrometer::detectIntegrationTime()
{
	if (!isReady())
	{
		return 0;
	}
	const unsigned long minTime{ getMinIntegrationTime() };
	const unsigned long maxTime{ getMaxIntegrationTime() };
	const unsigned long currIntegrationTime{ getIntegrationTime() };
	const double spectrumMaxIntensity{ *std::max_element(m_correctedSpectrum.begin(), m_correctedSpectrum.end()) };
	constexpr double targetMaxIntensity{ 65535.0 * 0.9 };
	constexpr double targetMinIntensity{ targetMaxIntensity * 0.95 };
	constexpr double targetAverage{ (targetMaxIntensity + targetMinIntensity) / 2.0 };

	unsigned long newIntegrationTime{ currIntegrationTime };
	if (spectrumMaxIntensity > targetMaxIntensity || spectrumMaxIntensity < targetMinIntensity)
	{
		newIntegrationTime = static_cast<unsigned long>(currIntegrationTime * (targetAverage / spectrumMaxIntensity));
	}
	return static_cast<unsigned long>(std::clamp(newIntegrationTime, minTime, maxTime));
}
std::size_t Spectrometer::getIndexOfWavelenght(double wavelength)
{
	double tempWL; // not nedeed. Hold value of waveLength at index
	std::size_t index{0};
	if (isReady())
	{
		index = odapi_get_index_at_wavelength(m_deviceIds[0], &m_errorCode, &tempWL, wavelength);
	}
	return index;
}
// SET FUNCTIONS PUBLIC
void Spectrometer::setIntegrationTime(const unsigned long ms)
{
	if (isReady())
	{
		m_integrationTimeMicroseconds = ms;
		odapi_set_integration_time_micros(m_deviceIds[0], &m_errorCode, m_integrationTimeMicroseconds);
	}
}
void Spectrometer::setAverageFactor(const unsigned int average)
{
	if (isReady())
	{
		m_averageFactor = average;
		odapi_set_scans_to_average(m_deviceIds[0], &m_errorCode, m_averageFactor);
	}
}
void Spectrometer::setDarkSpectrum(const std::vector<double>& darkSpectrum)
{
	m_darkSpectrum = darkSpectrum;
}
//PRIVATE FUNCTIONS
int Spectrometer::init()
{
	int deviceCount{ odapi_probe_devices() };

	if (deviceCount < 1)
	{
		m_init = false;
		return -1;
	}
	int deviceIdCount {odapi_get_number_of_device_ids()};
	m_deviceIds.resize(deviceIdCount);
	odapi_get_device_ids(m_deviceIds.data(), deviceIdCount);
	odapi_open_device(m_deviceIds[0], &m_errorCode);
	if (m_errorCode > 0 && m_errorCode < 10001) {
		m_init = false;
		return -1;
	}
	m_pixelCount = odapi_get_formatted_spectrum_length(m_deviceIds[0], &m_errorCode);
	if (m_errorCode > 0 && m_errorCode < 10001) {
		m_init = false;
		return -1;
	}
	odapi_apply_electric_dark_correction_usage(m_deviceIds[0], &m_errorCode, 0);
	odapi_apply_nonlinearity_correct_usage(m_deviceIds[0], &m_errorCode, 0);

	odapi_set_integration_time_micros(m_deviceIds[0], &m_errorCode, m_integrationTimeMicroseconds);
	odapi_set_scans_to_average(m_deviceIds[0], &m_errorCode, m_averageFactor);

	m_init = true;
	return 1;
}
std::vector<double> Spectrometer::readWaveLengths()
{
	if (!isReady())
	{
		return {};
	}
	m_wavelengths.clear();
	m_wavelengths.resize(m_pixelCount);
	odapi_get_wavelengths(m_deviceIds[0], &m_errorCode, m_wavelengths.data(), m_pixelCount);
	return m_wavelengths;
}
std::vector<double> Spectrometer::readBiasDarkSpectrum()
{
	if (!isReady())
	{
		return {};
	}

	const unsigned long oldIntegrationTime{ m_integrationTimeMicroseconds };
	setIntegrationTime( getMinIntegrationTime());
	m_biasDarkSpectrum.resize(m_pixelCount);
	odapi_get_formatted_spectrum(m_deviceIds[0], &m_errorCode, m_biasDarkSpectrum.data(), m_pixelCount);
	setIntegrationTime (oldIntegrationTime);
	calcPureDark();
	return m_biasDarkSpectrum;
}
std::vector<double> Spectrometer::readCalibDarkSpectrum() 
{
	if (!isReady())
	{
		return {};
	}

	const unsigned long oldIntegrationTime{ m_integrationTimeMicroseconds };
	setIntegrationTime(1000000);
	m_calibDarkSpectrum.resize(m_pixelCount);
	odapi_get_formatted_spectrum(m_deviceIds[0], &m_errorCode, m_calibDarkSpectrum.data(), m_pixelCount);
	setIntegrationTime( oldIntegrationTime);
	calcPureDark();
	return m_calibDarkSpectrum;
}
std::vector<double> Spectrometer::calcPureDark()
{
	if (m_calibDarkSpectrum.empty() || m_biasDarkSpectrum.empty())
	{
		return {};
	}

	const double t_min = static_cast<double>(getMinIntegrationTime());
	const double t_calib = 1000000.0; 
	const double timeDelta = t_calib - t_min;

	if (timeDelta <= 0.0) 
		return {};

	m_pureDarkSpectrum.resize(m_pixelCount);
	for (std::size_t i = 0; i < m_pixelCount; ++i)
	{
		double totalDarkNoise = m_calibDarkSpectrum[i] - m_biasDarkSpectrum[i];
		m_pureDarkSpectrum[i] = totalDarkNoise / timeDelta;
	}
	return m_pureDarkSpectrum;
}
std::vector<double> Spectrometer::readCorrectedSpectrum()
{
	if (!isReady())
	{
		return {};
	}
	
	m_correctedSpectrum.resize(m_pixelCount);
	if (m_darkSpectrum.empty())
	{
		m_darkSpectrum.assign(m_pixelCount, 0.0);
	}

	odapi_get_nonlinearity_corrected_spectrum1(
		m_deviceIds[0],
		&m_errorCode,
		m_darkSpectrum.data(),
		static_cast<int>(m_darkSpectrum.size()), 
		m_correctedSpectrum.data(), 
		static_cast<int>(m_correctedSpectrum.size())
	);
	
	return m_correctedSpectrum;
}