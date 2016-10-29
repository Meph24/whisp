#include "PerformanceMeter.h"
#include <ctgmath>

PerformanceMeter::PerformanceMeter(int stepCount)
{
	n = stepCount;
	time = new int[stepCount * 6];
	maxTolerated = time + stepCount;
	maxMeasured = time + stepCount * 2;
	minMeasured = time + stepCount * 3;
	avg = (float *)(time + stepCount * 4);//dirty: blame HL65536
	spikes = (float *)(time + stepCount * 5);
	names = new std::string[stepCount];
	clear();
	for (int i = 0; i < n; i++)
		maxTolerated[i] = 99999999;
}

PerformanceMeter::~PerformanceMeter()
{
	delete[] time;
	delete[] names;
}

void PerformanceMeter::clearMax()
{
	for (int i = 0; i < n; i++)
		maxMeasured[i] = -1;
}
void PerformanceMeter::clearMin()
{
	for (int i = 0; i < n; i++)
		minMeasured[i] = ~(1 << 31);
}
void PerformanceMeter::clear()
{
	for (int i = 0; i < n; i++)
	{
		time[i] = 0;
		maxMeasured[i] = -1;
		minMeasured[i] = ~(1 << 31);
		avg[i] = 0;
		spikes[i] = 0;
	}
	roundtriptime = 0;
}

//works for a maximum time <100s
std::string PerformanceMeter::getInfo(int stepID,int infoFlags)
{
	char format[9];
	format[5] = '.';
	std::string ret = names[stepID] + ':';
	int num;
	if (infoFlags&FLAG_ALL_TIME_MIN)
	{
		infoFlags -= FLAG_ALL_TIME_MIN;
		num = minMeasured[stepID];
		format[8] = '0' + (num % 10);
		num /= 10;
		format[7] = '0' + (num % 10);
		num /= 10;
		format[6] = '0' + (num % 10);
		num /= 10;
		format[4] = '0' + (num % 10);
		num /= 10;
		format[3] = '0' + (num % 10);
		num /= 10;
		format[2] = '0' + (num % 10);
		num /= 10;
		format[1] = '0' + (num % 10);
		num /= 10;
		format[0] = '0' + (num % 10);
		for (int i = 0; i < 4; i++)
		{
			if (format[i] == '0') format[i] = ' ';
			else break;
		}
		std::string avgString(format, 9);
		if (infoFlags) ret = ret + avgString + "ms min; ";
		else ret = ret + avgString + "ms min";
	}
	if (infoFlags&FLAG_NOW)
	{
		infoFlags -= FLAG_NOW;
		num = time[stepID];
		format[8] = '0' + (num % 10);
		num /= 10;
		format[7] = '0' + (num % 10);
		num /= 10;
		format[6] = '0' + (num % 10);
		num /= 10;
		format[4] = '0' + (num % 10);
		num /= 10;
		format[3] = '0' + (num % 10);
		num /= 10;
		format[2] = '0' + (num % 10);
		num /= 10;
		format[1] = '0' + (num % 10);
		num /= 10;
		format[0] = '0' + (num % 10);
		for (int i = 0; i < 4; i++)
		{
			if (format[i] == '0') format[i] = ' ';
			else break;
		}
		std::string avgString(format, 9);
		if (infoFlags) ret = ret + avgString + "ms now; ";
		else ret = ret + avgString + "ms now";
	}
	if (infoFlags&FLAG_AVG)
	{
		infoFlags -= FLAG_AVG;
		num = (int)(avg[stepID] + 0.5f);
		format[8] = '0' + (num % 10);
		num /= 10;
		format[7] = '0' + (num % 10);
		num /= 10;
		format[6] = '0' + (num % 10);
		num /= 10;
		format[4] = '0' + (num % 10);
		num /= 10;
		format[3] = '0' + (num % 10);
		num /= 10;
		format[2] = '0' + (num % 10);
		num /= 10;
		format[1] = '0' + (num % 10);
		num /= 10;
		format[0] = '0' + (num % 10);
		for (int i = 0; i < 4; i++)
		{
			if (format[i] == '0') format[i] = ' ';
			else break;
		}
		std::string avgString(format, 9);
		if (infoFlags) ret = ret + avgString + "ms avg; ";
		else ret = ret + avgString + "ms avg";
	}
	if (infoFlags&FLAG_SPIKES)
	{
		num = (int)(spikes[stepID]+0.5f);
		format[8] = '0' + (num % 10);
		num /= 10;
		format[7] = '0' + (num % 10);
		num /= 10;
		format[6] = '0' + (num % 10);
		num /= 10;
		format[4] = '0' + (num % 10);
		num /= 10;
		format[3] = '0' + (num % 10);
		num /= 10;
		format[2] = '0' + (num % 10);
		num /= 10;
		format[1] = '0' + (num % 10);
		num /= 10;
		format[0] = '0' + (num % 10);
		for (int i = 0; i < 4; i++)
		{
			if (format[i] == '0') format[i] = ' ';
			else break;
		}
		std::string spikesString(format, 9);
		if (infoFlags&FLAG_ALL_TIME_MAX) ret = ret + spikesString + "ms spikes; ";
		else ret = ret + spikesString + "ms spikes";
	}
	if (infoFlags&FLAG_ALL_TIME_MAX)
	{
		num = maxMeasured[stepID];
		format[8] = '0' + (num % 10);
		num /= 10;
		format[7] = '0' + (num % 10);
		num /= 10;
		format[6] = '0' + (num % 10);
		num /= 10;
		format[4] = '0' + (num % 10);
		num /= 10;
		format[3] = '0' + (num % 10);
		num /= 10;
		format[2] = '0' + (num % 10);
		num /= 10;
		format[1] = '0' + (num % 10);
		num /= 10;
		format[0] = '0' + (num % 10);
		for (int i = 0; i < 4; i++)
		{
			if (format[i] == '0') format[i] = ' ';
			else break;
		}
		std::string maxString(format, 9);
		ret = ret + maxString + "ms all time max";
	}
	return ret;
}

bool PerformanceMeter::registerTime(int stepID) //4K/s; 500K budget
{
	int t = clock.restart().asMicroseconds();//TODO replace: 2k cycles
	roundtriptime += t - time[stepID];
	time[stepID] = t;
	avg[stepID] = avg[stepID] * (1 - avgWeight) + t * avgWeight;
	float spike = spikes[stepID];
	float spklwd;
	if (useFastApproximation)
	{
		spklwd =  spike - spike * roundtriptime * spikeMultiplier;//TODO round trip time
	}
	else
	{
		float f = roundtriptime*spikeHalfLifeInv;
		spklwd = spike * pow<float>(0.5f, f);
	}
	spikes[stepID] = spklwd;
	if (spike<t) spikes[stepID] = t;
	if (t > maxMeasured[stepID]) maxMeasured[stepID] = t;
	if (t < minMeasured[stepID]) minMeasured[stepID] = t;
	bool tooLong = t > maxTolerated[stepID];
	if (tooLong)
	{
		exceededMax(stepID);
		return true;
	}
	return false;
}

void PerformanceMeter::setSpikeHalfLifeTime(float seconds)
{
	spikeHalfLifeInv = 0.000001f / seconds;
	spikeMultiplier = 0.0000005f / seconds;
}

void PerformanceMeter::reset()
{
	clock.restart();//TODO replace
}

void PerformanceMeter::exceededMax(int stepID)
{
	//TODO insert stuff
}

void PerformanceMeter::setName(std::string name, int  stepID)
{
	names[stepID] = name;
}

int PerformanceMeter::getMaxMeasured(int stepID)
{
	return maxMeasured[stepID];
}

int PerformanceMeter::getMinMeasured(int stepID)
{
	return minMeasured[stepID];
}

int PerformanceMeter::getTime(int stepID)
{
	return time[stepID];
}

float PerformanceMeter::getAVG(int stepID)
{
	return avg[stepID];
}

float PerformanceMeter::getSpikes(int stepID)
{
	return spikes[stepID];
}

void PerformanceMeter::setMaxTolerated(int stepID, int us)
{
	maxTolerated[stepID] = us;
}
