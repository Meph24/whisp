/*
 * PerformanceMeter.cpp
 *
 *  Created on:	before 18.03.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "PerformanceMeter.h"
#include <ctgmath>
#include "myAssert.h"

PerformanceMeter::PerformanceMeter(FloatSeconds historyLength,FloatSeconds WarmupTime,bool BenchmarkMode):
benchmarkMode(BenchmarkMode)
{
	historyLen=(float)historyLength;
	warmupTime=(float)WarmupTime;
}

PerformanceMeter::~PerformanceMeter()
{
}

void PerformanceMeter::clearMax()
{
	for(StepData& step: stepData)
	{
		step.clearMax();
	}
}
void PerformanceMeter::clearMin()
{
	for(StepData& step: stepData)
	{
		step.clearMin();
	}
}
void PerformanceMeter::clearTimeData()
{
	for(StepData& step: stepData)
	{
		step.clearAllTimeData();
	}
}

std::string PerformanceMeter::getInfo(int stepID,int infoFlags)
{
	if(stepID<0) return getInfo(roundtrip,infoFlags);
	else return getInfo(stepData[stepID],infoFlags);
}

PerformanceMeter::SingleTimer PerformanceMeter::createTimestep(std::string name)
{
	stepData.emplace_back();
	stepData.back().name=name;
	return {this,(int)(stepData.size()-1)};
}

int PerformanceMeter::getStepCount()
{
	return (int)stepData.size();
}

std::string PerformanceMeter::getInfo(StepData& data, int infoFlags)
{
	char format[9];
	format[5] = '.';
	std::string ret = data.name + ':';
	int num;
	float unconverted;
	if (infoFlags&FLAG_ALL_TIME_MIN)
	{
		infoFlags -= FLAG_ALL_TIME_MIN;
		unconverted = data.min;
		if(unconverted>99.999999) unconverted=99.999999;
		num=(int)(unconverted*1000000);
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
		unconverted = data.getLastTime();
		if(unconverted>99.999999) unconverted=99.999999;
		num=(int)(unconverted*1000000);
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
		unconverted = data.getAVG();
		if(unconverted>99.999999) unconverted=99.999999;
		num=(int)(unconverted*1000000);
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
	if (infoFlags&FLAG_SPIKES)
	{
		unconverted = data.getRecentMax();
		if(unconverted>99.999999) unconverted=99.999999;
		num=(int)(unconverted*1000000);
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
		unconverted = data.max;
		if(unconverted>99.999999) unconverted=99.999999;
		num=(int)(unconverted*1000000);
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

#include <iostream>
void PerformanceMeter::registerTime(int stepID)
{
	float time=clock.restart().asMicroseconds()*0.000001f;//TODO replace: uses 2k processor cycles
	stepData[stepID].registerTime(time);
	roundtripTime+=time;
	if(stepID==roundtripIndex)
	{
		roundtrip.registerTime(roundtripTime);
		roundtripTime=0;
		float avg=roundtrip.getAVG();
		float allowedSteps=historyLen/avg;
		if(roundtrip.times.size()>allowedSteps+1)
		{
			int deleteTimes=(int)(roundtrip.times.size()-allowedSteps);
			//std::cout<<"delete times: "<<deleteTimes<<std::endl;
			roundtrip.shrinkTimeBuffer(deleteTimes);
			for(StepData& st: stepData)
			{
				deleteTimes=(int)(st.times.size()-allowedSteps);
				st.shrinkTimeBuffer(deleteTimes);
			}
		}
		if(warmupTime>0)
		{
			warmupTime-=time;
			if(warmupTime<=0)
			{
				warmupTime=0;
				clearTimeData();
			}
		}
		if(benchmarkMode) resetTimer();
	}
}


void PerformanceMeter::resetTimer()
{
	clock.restart();//TODO replace, reason: bad performance
}

void PerformanceMeter::StepData::registerTime(float time)
{
	if(time<min) min=time;
	if(time>max) max=time;
	times.push_back(time);
	//if(time>maxTolerated)
	//{
	//TODO
	//}
}

void PerformanceMeter::SingleTimer::registerTime()
{
	assert(pm);
	pm->registerTime(myID);
}

void PerformanceMeter::StepData::clearMin()
{
	min=std::numeric_limits<float>::infinity();
}

void PerformanceMeter::StepData::clearMax()
{
	max=0;
}

float PerformanceMeter::StepData::getLastTime()
{
	if(times.empty()) return 0;
	return times.back();
}

float PerformanceMeter::StepData::getAVG()
{
	float size=times.size();
	if(size==0) return 0;
	float sum=0;
	for(float t: times)
	{
		sum+=t;
	}
	return sum/size;
}

float PerformanceMeter::StepData::getRecentMax()
{
	float max=0;
	for(float t: times)
	{
		if(t>max) max=t;
	}
	return max;
}

void PerformanceMeter::StepData::clearAllTimeData()
{
	clearMin();
	clearMax();
	times.clear();
}

void PerformanceMeter::SingleTimer::setAsRoundtripMarker(std::string roundtripName)
{
	assert(pm);
	pm->roundtripIndex=myID;
	pm->roundtrip.name=roundtripName;
}

PerformanceMeter::StepData& PerformanceMeter::SingleTimer::getData()
{
	assert(pm);
	return pm->stepData[myID];
}

void PerformanceMeter::StepData::shrinkTimeBuffer(int by)
{
	for(int i=0;i<by;i++)
	{
		times.pop_front();
	}
}

#include <GL/glew.h>
#include "Spacevec.h"
#include "Frustum.h"
#include "IWorld.h"
#include "DrawServiceProvider.h"
void PerformanceMeter::draw(const SimClock::time_point& draw_time, Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp)
{
	spacevec interPos=-viewFrustum->observerPos;
	vec3 interPosMeters=iw.toMeters(interPos);
	if(roundtrip.times.size()<2) return;
	float offsetH=2.0f;// m
	glPushMatrix();
	glTranslatef(interPosMeters.x-8, interPosMeters.y+offsetH, interPosMeters.z);
	glColor3f(1,1,1);//white
	roundtrip.draw(roundtrip.times,dsp->g);
	int size=stepData.size();
	for(int i=0;i<size;i++)
	{
		glTranslatef(0,0,1);
		float H=(255.0f*i)/size;//HSV conversion from https://gist.github.com/marukrap/7c361f2c367eaf40537a8715e3fd952a
		float HPrime = std::fmod(H / 60, 6.f); // H'
		float X = (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
		float R=0,G=0,B=0;
		switch (static_cast<int>(HPrime))
		{
		case 0: R = 1; G = X;        break; // [0, 1)
		case 1: R = X; G = 1;        break; // [1, 2)
		case 2:        G = 1; B = X; break; // [2, 3)
		case 3:        G = X; B = 1; break; // [3, 4)
		case 4: R = X;        B = 1; break; // [4, 5)
		case 5: R = 1;        B = X; break; // [5, 6)
		}
		glColor3f(R,G,B);
		stepData[i].draw(roundtrip.times,dsp->g);
	}


	glPopMatrix();
}
#include "Graphics2D.h"
void PerformanceMeter::StepData::draw(std::deque<float>& roundtrip,Graphics2D * g)
{
	if(times.size()<2) return;
	float scaleH=0.1f;// m/ms
	float scaleW=0.5f;// m/ms
	float lineSize=0.003f;// m
	lineSize/=scaleH;

	int steps=roundtrip.size();
	if(steps>(int)times.size()) steps=times.size();
	float pos=0;
	glPushMatrix();
	glTranslatef(0,0,-1);
	g->drawString(name,pos,-0.25f,0.2f,20);
	glPopMatrix();
	for(int i=1;i<steps;i++)
	{
		float lineLength=roundtrip[i];
		float lineHeightA=times[i-1]*1000;
		float lineHeightB=times[i]*1000;
		glPushMatrix();
		glScalef(scaleW,scaleH,1);
		glBegin(GL_QUADS);
		glVertex3f(pos,lineHeightA-lineSize,0);
		glVertex3f(pos,lineHeightA+lineSize,0);
		pos+=lineLength;
		glVertex3f(pos,lineHeightB+lineSize,0);
		glVertex3f(pos,lineHeightB-lineSize,0);
		glEnd();
		glPopMatrix();
	}
}
