
#include "PPM.h"
#include "ErrorDef.h"

CPeakMeter::CPeakMeter() 
{
}

CPeakMeter::~CPeakMeter() 
{
	reset();
}


Error_t CPeakMeter::init(float fSampleRate, int iNumChannels, int iBlockLength)
{
	m_iNumChannels = iNumChannels;
	m_fSampleRate = fSampleRate;
	m_iBlockLength = iBlockLength;
	m_fLastPeak = new float[iNumChannels];

	m_fMaxAmplitude = new float[iNumChannels];

	// Initialize PPM Output accoring to the number of channels
	m_PPMOutput = new float*[iNumChannels];

	// Assign default values and allocate memory
	for (int i = 0; i<iNumChannels; i++) 
	{
		m_fMaxAmplitude[i] = 0.0;
		m_fLastPeak[i] = 0.0;
		m_PPMOutput[i] = new float[m_iBlockLength];
	}

	return kNoError;
}

void CPeakMeter::reset() 
{
	for (int i = 0; i < m_iNumChannels; i++) 
	{
		delete[] m_PPMOutput[i];
	}
	delete[] m_fLastPeak;
	delete[] m_fMaxAmplitude;
	delete[] m_PPMOutput;
}

float CPeakMeter::getPeak(int iChannelNum) 
{

	float peak = m_fLastPeak[iChannelNum];
	 
	for (int i = 0; i < m_iBlockLength; i++) 
	{
		if (m_PPMOutput[iChannelNum][i] > peak) 
		{
			peak = m_PPMOutput[iChannelNum][i];
		}
	}

	// Set limit to -100dB
	if (peak < 1e-5) 
	{
		peak = 1e-5;
	}

	return peak;
}

float CPeakMeter::getMaxAmplitude(int iChannelNum) 
{
	return m_fMaxAmplitude[iChannelNum];
}

float CPeakMeter::convertFloatTodB(float fAmplitude) 
{
	float fAmplitudeInDB = 20 * log10(fAmplitude);
	if (fAmplitudeInDB > 6)
		fAmplitudeInDB = 6;
	if (fAmplitudeInDB < -88)
		fAmplitudeInDB = -88;
	return fAmplitudeInDB;
}

void CPeakMeter::calculatePeak(float **pp_fAudioBlock, float fAlphaAT, float fAlphaRT) 
{

	for (int iChannelNum = 0; iChannelNum<m_iNumChannels; iChannelNum++) 
	{
		float fLocalPeak = 0.0;

		for (int i = 0; i < m_iBlockLength; i++) 
		{

			if (m_fLastPeak[iChannelNum] > fabsf(pp_fAudioBlock[iChannelNum][i])) 
			{
				// Release State
				m_PPMOutput[iChannelNum][i] = (1 - fAlphaRT) * m_fLastPeak[iChannelNum];
			}
			else 
			{
				// Attack State
				m_PPMOutput[iChannelNum][i] = fAlphaAT * fabsf(pp_fAudioBlock[iChannelNum][i]) + (1 - fAlphaAT) * m_fLastPeak[iChannelNum];
			}
			if (m_PPMOutput[iChannelNum][i] > fLocalPeak) 
			{
				fLocalPeak = m_PPMOutput[iChannelNum][i];
			}
		}
		m_fLastPeak[iChannelNum] = fLocalPeak;
	}

}

