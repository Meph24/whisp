/*
 * ClassIDs.hpp
 *
 *  Created on:	11.02.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_CLASSIDS_HPP_
#define SRC_CLASSIDS_HPP_

#include "ShortNames.h"
#include <string>


static const u32 CLASS_ID_INVALID = 0;

static const u32 CLASS_ID_Zombie_World = 1;
static const u32 CLASS_ID_Zombie_Enemy = 2;
static const u32 CLASS_ID_EntityProjectileBulletLike = 3;
static const u32 CLASS_ID_Simulation_World = 4;
static const u32 CLASS_ID_EntitySound = 5;
static const u32 CLASS_ID_EntityPlayer = 6;
static const u32 CLASS_ID_ModelEntity = 7;
static const u32 CLASS_ID_TransModelEntity = 8;
static const u32 CLASS_ID_GridEntity = 9;
static const u32 CLASS_ID_OxelEntity = 10;
static const u32 CLASS_ID_Zombie_Tree = 11;
static const u32 CLASS_ID_BenchEntitySlave = 12;
static const u32 CLASS_ID_BenchEntityMaster = 13;
static const u32 CLASS_ID_BenchEntityS = 14;
//static const u32 CLASS_ID_ = 15;
//static const u32 CLASS_ID_ = 16;
//static const u32 CLASS_ID_ = 17;
//static const u32 CLASS_ID_ = 18;
//static const u32 CLASS_ID_ = 19;
//static const u32 CLASS_ID_ = 20;
//static const u32 CLASS_ID_ = 21;
//static const u32 CLASS_ID_ = 22;
//static const u32 CLASS_ID_ = 23;
//static const u32 CLASS_ID_ = 24;
//static const u32 CLASS_ID_ = 25;
//static const u32 CLASS_ID_ = 26;
//static const u32 CLASS_ID_ = 27;
//static const u32 CLASS_ID_ = 28;
//static const u32 CLASS_ID_ = 29;
//static const u32 CLASS_ID_ = 30;
//static const u32 CLASS_ID_ = 31;
//static const u32 CLASS_ID_ = 32;
//static const u32 CLASS_ID_ = 33;
//static const u32 CLASS_ID_ = 34;
//static const u32 CLASS_ID_ = 35;
//static const u32 CLASS_ID_ = 36;
//static const u32 CLASS_ID_ = 37;
//static const u32 CLASS_ID_ = 38;
//static const u32 CLASS_ID_ = 39;

inline std::string className(u32 classID)
{
	switch(classID)
	{
	case CLASS_ID_INVALID:
		return "invalid class";
	case CLASS_ID_Zombie_World:
		return "Zombie_World";
	case CLASS_ID_Zombie_Enemy:
		return "Zombie_Enemy";
	case CLASS_ID_EntityProjectileBulletLike:
		return "EntityProjectileBulletLike";
	case CLASS_ID_Simulation_World:
		return "Simulation_World";
	case CLASS_ID_EntitySound:
		return "EntitySound";
	case CLASS_ID_EntityPlayer:
		return "EntityPlayer";
	case CLASS_ID_ModelEntity:
		return "ModelEntity";
	case CLASS_ID_TransModelEntity:
		return "TransModelEntity";
	case CLASS_ID_OxelEntity:
		return "OxelEntity";
	case CLASS_ID_Zombie_Tree:
		return "Zombie_Tree";
	case CLASS_ID_BenchEntitySlave:
		return "BenchEntitySlave";
	case CLASS_ID_BenchEntityMaster:
		return "BenchEntityMaster";
	case CLASS_ID_BenchEntityS:
		return "BenchEntityS";
	default:
		return "unknown: classID="+classID;
	}
}

#endif /* SRC_CLASSIDS_HPP_ */
