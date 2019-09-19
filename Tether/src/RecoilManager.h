/*
 * RecoilManager.h
 *
 *  Created on:	05.01.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_RECOILMANAGER_H_
#define SRC_RECOILMANAGER_H_

#include <glm/glm.hpp>

using glm::vec3;

class RecoilManager
{

	float * timer;
	vec3 * recoilComponents;

	int size;
	int start;
	int curLen;
	vec3 lastRecoil;
	int incr(int pos);
	int incr(int pos,int by);
	vec3 getCurrentRecoil(float timePassed);
protected:
	float totalLength;
	virtual vec3 getRecoil(float time)=0;
public:
	RecoilManager(int arraySize,float recoilLength);
	virtual ~RecoilManager();

	vec3 getCurrentRecoil();
	vec3 getRecoilDiff(float timePassed);
	void registerRecoil(vec3 component1,vec3 component2,vec3 component3);

};

#endif /* SRC_RECOILMANAGER_H_ */
