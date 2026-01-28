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
const double Spectrometr::PPFD(const bool extendedPAR)
{
	
	const double h = 6.62607015e-34;
	const double c = 3e8;
	const double Na = 6.022e23;
	//or use 119.7
	
	int minNM;
	int maxNM;
	if (extendedPAR)
	{
		minNM = 350;
		maxNM = 800;
		//350 - 800 nm
	}
	else
	{
		minNM = 400;
		maxNM = 700;
		//400 - 700 nm
	};

	double tempWL; // not nedeed. Hold value of waveLength at index
	int lo{ 0 }; // index at wavelength array with 300 or 400 nm
	int hi{ 0 }; // index at wavelength array with 700 or 800 nm
	if (isReady())
	{
		lo = odapi_get_index_at_wavelength(m_deviceIds[0], &m_errorCode, &tempWL, minNM);
		hi = odapi_get_index_at_wavelength(m_deviceIds[0], &m_errorCode, &tempWL, maxNM);
	}
	
	double PPFD = 0.0;

	//DEBUG
	m_wavelengths.assign( { 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410});
	m_wavelengthCount = m_wavelengths.size();

	m_spectrum.assign( {0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05});

	hi = m_wavelengthCount;
	//from 350 nm to 800 nm
	//
	//check for 0 value
	//double deltaLambda = 0.0;
	for (int i{ lo }; i < hi; ++i)
	{
		
		double wl_m = m_wavelengths.at(i) * 1e-9; // wavelength in meters
	
		double E = m_spectrum.at(i); // spectral irradiance in W/m^2/nm

		if(E > 0.0)
			PPFD += (E * wl_m) / (h * c) * 1e6 / Na;

		
	}
	
	
	return PPFD;
}
// SET FUNCTIONS PUBLIC

void Spectrometr::setIntegrationTime(const unsigned long ms)
{
	if (isReady())
	{
		m_integrationTimeMicroseconds = ms;
		//odapi_set_integration_time_micros(m_deviceIds[0], &m_errorCode, m_integrationTimeMicroseconds);
	}
}
void Spectrometr::setAverageFactor(const unsigned int average)
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
		odapi_set_integration_time_micros(m_deviceIds[0], &m_errorCode, m_integrationTimeMicroseconds);
		//odapi_set_boxcar_width(m_deviceIds[0], &m_errorCode, m_averageFactor);
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
	// 
	std::vector<double> normalizedSpectrum = m_spectrum; // not rewriting spectr in absolute values
	
	//PPFD colculater WRONG because m_spectrum is normalized BEFORE PPFD calculatrion, which is wrong
	// add filter to negative spectrum values?
	for (int i{ 0 }; i < m_pixelCount; ++i)
	{
		// replace m_spectrum.at(i) with normalizedSpectrum.at(i)
		if (m_spectrum.at(i) <= m_maxIntensity && m_maxIntensity > 0)
		{
			normalizedSpectrum.at(i) /= m_maxIntensity;
			//below is zero-filtering
			/*if (m_spectrum.at(i) > 0)
			{
				normalizedSpectrum.at(i) /= m_maxIntensity;
			}
			else
			{
				normalizedSpectrum.at(i) = 0.0;
			}*/
			
		}

		point.setX(m_wavelengths.at(i));
		point.setY(normalizedSpectrum.at(i));
		m_convertedSeries.append(point);

	}
}
