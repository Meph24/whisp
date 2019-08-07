#include "PerformanceMeter.h"
#include <ctgmath>

PerformanceMeter::PerformanceMeter(int stepCount,int warmupPeriod):
warmup(warmupPeriod),warmupCounter(0)
{
	stepCount++;
	n = stepCount;
	time = new int[stepCount * 8];
	maxTolerated = time + stepCount;
	maxMeasured = time + stepCount * 2;
	minMeasured = time + stepCount * 3;
	avgRecent = (float *)(time + stepCount * 4);//dirty: blame HL65536
	spikes = (float *)(time + stepCount * 5);
	sum = (float *)(time + stepCount * 6);
	runs = time + stepCount * 7;
	names = new std::string[stepCount];
	clear();
	for (int i = 0; i < n; i++)
		maxTolerated[i] = 99999999;
	roundtripUpdateIndex = stepCount - 1;
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
void PerformanceMeter::clearTotalAVG()
{
	for (int i = 0; i < n; i++)
		sum[i] = 0;
}
float PerformanceMeter::getTotalAVG(int stepID)
{
	return sum[stepID] / runs[stepID];
}
void PerformanceMeter::clear()
{
	for (int i = 0; i < n; i++)
	{
		time[i] = 0;
		maxMeasured[i] = -1;
		minMeasured[i] = ~(1 << 31);
		avgRecent[i] = 0;
		sum[i] = 0;
		runs[i] = 0;
		spikes[i] = 0;
	}
	roundtriptime = 0;
}

//works for a maximum time <100s
std::string PerformanceMeter::getInfo(int stepID,int infoFlags)
{
	if(stepID<0) stepID=n-1;
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
	if (infoFlags&FLAG_RECENTAVG)
	{
		infoFlags -= FLAG_RECENTAVG;
		num = (int)(avgRecent[stepID] + 0.5f);
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
		if (infoFlags) ret = ret + avgString + "ms recent avg; ";
		else ret = ret + avgString + "ms recent avg";
	}
	if (infoFlags&FLAG_TOTALAVG)
	{
		infoFlags -= FLAG_TOTALAVG;
		int numRuns = runs[stepID];
		if (numRuns == 0) num = 0;
		else num = (int)(sum[stepID]/numRuns + 0.5f);
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
		if (infoFlags) ret = ret + avgString + "ms total avg; ";
		else ret = ret + avgString + "ms total avg";
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
	int t;
	if(stepID!=(n-1))
	{
		t= clock.restart().asMicroseconds();//TODO replace: 2k cycles
		roundtriptime += t - time[stepID];
	}
	else t=roundtriptime;
	if(stepID==roundtripUpdateIndex)
	{
		registerTime(n-1);
		if(warmupCounter==warmup)
		{
			//reset certain time data
			for (int i = 0; i < n; i++)
			{
				maxMeasured[i] = -1;
				minMeasured[i] = ~(1 << 31);
				avgRecent[i] = 0;
				sum[i] = 0;
				runs[i] = 0;
				spikes[i] = 0;
			}
		}
		warmupCounter++;

	}
	runs[stepID]++;
	time[stepID] = t;
	avgRecent[stepID] = avgRecent[stepID] * (1 - avgWeight) + t * avgWeight;
	float spike = spikes[stepID];
	float spklwd;
	if (useFastApproximation)
	{
		spklwd =  spike - spike * roundtriptime * spikeMultiplier;
	}
	else
	{
		float f = roundtriptime*spikeHalfLifeInv;
		spklwd = spike * powf(0.5f, f);
	}
	spikes[stepID] = spklwd;
	if (spike<t) spikes[stepID] = t;
	sum[stepID] += t;
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
	clock.restart();//TODO replace, reason: bad performance
}

void PerformanceMeter::exceededMax(int stepID)
{
	//TODO listener
}

void PerformanceMeter::setName(std::string name, int stepID)
{
	if(stepID<0) stepID=n-1;
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

float PerformanceMeter::getRecentAVG(int stepID)
{
	return avgRecent[stepID];
}

float PerformanceMeter::getSpikes(int stepID)
{
	return spikes[stepID];
}


int PerformanceMeter::getStepCount()
{
	return n-1;
}
void PerformanceMeter::setMaxTolerated(int stepID, int us)
{
	maxTolerated[stepID] = us;
}
