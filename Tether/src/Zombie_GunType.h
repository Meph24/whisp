/*
 * Zombie_GunType.h
 *
 *  Created on:	12.12.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ZOMBIE_GUNTYPE_H_
#define SRC_ZOMBIE_GUNTYPE_H_
#include "Compatibility.h"
#include "SkilledValue.h"
#include "MatrixLib.h"

#define MOA 0.0002908
#define MIL 0.001

class Zombie_GunType
{
public:
	int ID;
	int firemode;//0=full auto,1=semi-auto,>1=burst with that many rounds
	Compatibility magComp;
	Compatibility ammoComp;
	int internalCapacity;//rounds hold inside the gun, not in a detachable magazine
	float efficiency;
	float additionalPower;
	vec3 RandomSpread;
	vec3 usualRecoil;
	SkilledValue recoilMultiplier;
	vec3 recoilSpread;
	SkilledValue recoilSpreadMultiplier;
	SkilledValue timeBetweenShots;//s
	SkilledValue singleReloadTime;//s
	SkilledValue magReloadTime;//s
	Zombie_GunType();
	~Zombie_GunType();
};

#endif /* SRC_ZOMBIE_GUNTYPE_H_ */
