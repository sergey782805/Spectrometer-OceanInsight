#include "Spectrometr.h"

Spectrometr::Spectrometr(): m_averageFactor {1}, m_integrationTimeMicroseconds{ 400000 }
{
	init(); // searching spectrometr during RUNTIME
	
}
Spectrometr::~Spectrometr()
{
	m_wavelengths.clear();
	m_spectrum.clear();
	if (isReady())
	{
		odapi_close_device(m_deviceIds[0], &m_errorCode);
		odapi_shutdown();
	}
}
//GET functions PUBLIC
const int Spectrometr::getdeviceCount()
{
	return m_deviceCount;
}
const int Spectrometr::getDeviceIdCount()
{
	return m_deviceIdCount;
}
const char Spectrometr::getDeviceName()
{
	return m_deviceName[32];
}
const int Spectrometr::getPixelCount()
{
	return m_pixelCount;
}
const bool Spectrometr::isReady()
{
	return m_init;
}
QList<QPointF> Spectrometr::getNewSpectrum()
{
	readWaveLengths();
	readSpectrum();
	vectorToQlist();
	return m_convertedSeries;
}
// SET FUNCTIONS PUBLIC

void Spectrometr::setIntegrationTime(unsigned long ms)
{
	if (isReady())
	{
		odapi_set_integration_time_micros(m_deviceIds[0], &m_errorCode, m_integrationTimeMicroseconds);
	}
}
void Spectrometr::setAverageFactor(unsigned int average)
{
	m_averageFactor = average;
}

//PRIVATE FUNCTIONS
int Spectrometr::init()
{
	m_deviceCount = odapi_probe_devices();

	if (m_deviceCount < 1)
	{
		m_init = false;
		return -1;
	}
	m_deviceIdCount = odapi_get_number_of_device_ids();
	m_deviceIds.resize(m_deviceIdCount);
	m_retrievedIdCount = odapi_get_device_ids(m_deviceIds.data(), m_deviceIdCount);
	odapi_open_device(m_deviceIds[0], &m_errorCode);
	if (m_errorCode > 0 && m_errorCode < 10001) {
		m_init = false;
		return -1;
	}
	odapi_get_device_name(m_deviceIds[0], &m_errorCode, m_deviceName, 32);
	if (m_errorCode > 0 && m_errorCode < 10001) {
		m_init = false;
		return -1;
	}
	m_pixelCount = odapi_get_formatted_spectrum_length(m_deviceIds[0], &m_errorCode);
	if (m_errorCode > 0 && m_errorCode < 10001) {
		m_init = false;
		return -1;
	}
	//init end. Ready for reading spectr

	m_init = 1;
	return 0;
}
void Spectrometr::readWaveLengths()
{
	if (isReady())
	{
		m_wavelengths.clear();
		m_wavelengths.resize(m_pixelCount);
		m_wavelengthCount = odapi_get_wavelengths(m_deviceIds[0], &m_errorCode, m_wavelengths.data(), m_pixelCount);
		//m_maxX = *std::max_element(m_wavelengths.begin(), m_wavelengths.end());
	}
}
void Spectrometr::readSpectrum()
{
	if (isReady())
	{
		m_spectrum.clear();
		//unsigned int average = 50;//!!
		odapi_set_scans_to_average(m_deviceIds[0], &m_errorCode, m_averageFactor);
		//odapi_get_scans_to_average(m_deviceIds[0], &m_errorCode); USED TO GET AVARAVE VALUE?

		m_spectrum.resize(m_pixelCount);
		m_intensityCount = odapi_get_formatted_spectrum(m_deviceIds[0], &m_errorCode, m_spectrum.data(), m_pixelCount);
		m_maxIntensity = *std::max_element(m_spectrum.begin(), m_spectrum.end());
	}
}
void Spectrometr::vectorToQlist()
{
	m_convertedSeries.clear();
	QPointF point;
	//Y are normalized to 1
	for (int i{ 0 }; i < m_pixelCount; ++i)
	{
		if (m_spectrum.at(i) <= m_maxIntensity && m_maxIntensity > 0)
		{
			m_spectrum.at(i) /= m_maxIntensity;
		}

		point.setX(m_wavelengths.at(i));
		point.setY(m_spectrum.at(i));
		m_convertedSeries.append(point);

	}
}
