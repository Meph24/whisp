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

void WarnErrReporter::unknownTypeErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (unknownTypeErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::notDeletedErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"semi-hard error (notDeletedErr): no crash so far, my still happen later, this definitely should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::wrongTypeErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (wrongTypeErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::wrongAttachmentErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"semi-hard error (wrongAttachmentErr): no crash so far, my still happen later, this definitely should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void WarnErrReporter::missingAttachmentErr(std::string text)
{
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<"soft error (missingAttachmentErr): no crash, but this should not have happened:"<<std::endl;
	std::cout<<text<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
}
