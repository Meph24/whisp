/*
 * WarnErrReporter.h
 *
 *  Created on:	09.07.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_WARNERRREPORTER_H_
#define SRC_WARNERRREPORTER_H_

#include <string>

class WarnErrReporter
{
public:

	//TODO log errors and warnings to file

	//warnings:


	//soft errors (no crash, some default behavior is used)
	static void notInitializedErr(std::string text);
	static void unknownTypeErr(std::string text);
	static void wrongTypeErr(std::string text);
	static void noOverrideErr(std::string text);
	static void doubleErr(std::string text);
	static void noEffectErr(std::string text);//despite expected effect
	static void notFoundWhereBelongsErr(std::string text);
	static void notFoundErr(std::string text);
	static void outsideWorldErr(std::string text);
	static void timeBackwardErr(std::string text);
	static void alreadyDeadErr(std::string text);
	static void missingAttachmentErr(std::string text);
	//TODO go through them and sort to semi-hard where needed

	//semi-hard errors (no crash so far, default behavior can correct error but may fail to do so
	static void notDeletedErr(std::string text);
	static void wrongAttachmentErr(std::string text);


	//hard errors (requires crash and extensive log)
	//TODO

};

#endif /* SRC_WARNERRREPORTER_H_ */
