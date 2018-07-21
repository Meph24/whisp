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

	//warnings:


	//soft errors (no crash, some default behavior is used)
	static void notInitializedErr(std::string text);
	static void noOverrideErr(std::string text);
	static void doubleErr(std::string text);
	static void noEffectErr(std::string text);

	//hard errors (requires crash and extensive log)
	//TODO

};

#endif /* SRC_WARNERRREPORTER_H_ */
