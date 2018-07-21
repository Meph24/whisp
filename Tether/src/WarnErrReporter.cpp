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
