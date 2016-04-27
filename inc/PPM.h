#pragma once

#include <stdio.h>
#include <math.h>
#include "ErrorDef.h"

class CPeakMeter
{
public:
	CPeakMeter();
	virtual ~CPeakMeter();

	Error_t init(float fSampleRate, int iNumChannels, int iBlockLength);
    void calculatePeak(float **pp_fAudioBlock, float fAlphaAT, float fAlphaRT);
	float getPeak(int iChannelNum);
	float getMaxAmplitude(int iChannelNum);
	float convertFloatTodB(float fAmplitude);
	void reset();

private:
	float   **m_PPMOutput;
	float   *m_fMaxAmplitude;
	float   *m_fLastPeak;
	int     m_iNumChannels;
	int     m_iBlockLength;
	float   m_fSampleRate;
};