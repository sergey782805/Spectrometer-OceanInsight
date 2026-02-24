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

	double PPFD(const std::vector<double>& wavelengths, const std::vector<double>& spectrum, std::size_t lo, std::size_t hi);

private:

	std::vector<double> m_wavelengths; // should I use this to store last read and last opened from file spectrum?
	std::vector<double> m_spectrum; // when I'm saving spectrum, get data from here?


};

