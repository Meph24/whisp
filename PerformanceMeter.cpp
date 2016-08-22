#include "PerformanceMeter.h"


PerformanceMeter::PerformanceMeter(int count)
{
	n = count;
	time = new int[count*4+1];
	maxTolerated = time+count+1;
	maxMeasured = time + count*2 + 1;
	avg = (float *) (time + count*3 + 1);//dirty: blame HL65536
}


PerformanceMeter::~PerformanceMeter()
{
	delete[] time;
}

void PerformanceMeter::clearMax()
{
	for (int i = 0; i < n; i++)
		maxMeasured[i] = -1;
}

bool PerformanceMeter::registerTime(int stepID)
{
	time[stepID] = clock.restart().asMicroseconds();
	if (stepID != 0)
	{
		avg[stepID - 1] = avg[stepID - 1] * (1 - avgWeight) + time[stepID] * avgWeight;
		bool tooLong = time[stepID] > maxTolerated[stepID-1];
		if (tooLong)
		{
			exceededMax(stepID - 1);
			return true;
		}
	}
}

int PerformanceMeter::getMaxMeasured(int stepID)
{
	return maxMeasured[stepID];
}

int PerformanceMeter::getTime(int stepID)
{
	return time[stepID];
}

float PerformanceMeter::getAVG(int stepID)
{
	return avg[stepID];
}

void PerformanceMeter::exceededMax(int stepID)
{
	
}

void PerformanceMeter::setMaxTolerated(int stepID, int us)
{

}