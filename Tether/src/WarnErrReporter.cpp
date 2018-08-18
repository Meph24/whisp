/*
 * WarnErrReporter.cpp
 *
 *  Created on:	09.07.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "WarnErrReporter.h"


#include <iostream>


void WarnErrReporter::notInitializedErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (notInitializedErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::noOverrideErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (noOverrideErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::doubleErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (doubleErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::noEffectErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (noEffectErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::notFoundWhereBelongsErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (notFoundWhereBelongsErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::notFoundErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (notFoundErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::outsideWorldErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (outsideWorldErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::timeBackwardErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (timeBackwardErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::alreadyDeadErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (alreadyDeadErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}
