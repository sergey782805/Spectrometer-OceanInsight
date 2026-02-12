#include "SpectrumProcessor.h"

SpectrumProcessor::SpectrumProcessor():
	m_wavelengths{0}, m_spectrum{0}
{

}
SpectrumProcessor::~SpectrumProcessor()
{

}


QList<QPointF> SpectrumProcessor::toQList(const std::vector<double>& wavelengths, const std::vector<double>& spectrum)
{
	
	if (wavelengths.size() != spectrum.size())
		return { QPointF{0, 0} };

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
double SpectrumProcessor::PPFD(const std::vector<double>& wavelengths, const std::vector<double>& spectrum, std::size_t lo, std::size_t hi)
{

	if (wavelengths.size() != spectrum.size()
		|| lo >= wavelengths.size() - 1
		|| lo > hi)
	{
		return 0;
	}
	const double h = 6.62607015e-34;
	const double c = 3e8;
	const double Na = 6.022e23;
	//or use 119.7

	
	//FIND A WAY TO determine indexes 
	
	//std::size_t lo{ 0 }; // index at wavelength array with 300 or 400 nm
	//std::size_t hi{ 0 }; // index at wavelength array with 700 or 800 nm
	

	double PPFD = 0.0;

	//DEBUG
	//wavelengths.assign({ 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410 });
	//hi = wavelengths.size();

	//spectrum.assign({ 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05 });

	//from 350 nm to 800 nm
	//
	//check for 0 value
	//double deltaLambda = 0.0;
	for (std::size_t i{ lo }; i <= hi; ++i)
	{

		double wl_m = wavelengths.at(i) * 1e-9; // wavelength in meters

		double E = spectrum.at(i); // spectral irradiance in W/m^2/nm

		if (E > 0.0)
			PPFD += (E * wl_m) / (h * c) * 1e6 / Na;
	}

	return PPFD;
}