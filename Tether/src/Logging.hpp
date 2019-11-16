#ifndef LOGGING_HPP
#     define LOGGING_HPP

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "LogStream.hpp"

using std::numeric_limits;
using std::ofstream;
using std::ostream;
using std::string;
using std::unique_ptr;

namespace log
{

enum TYPE
{
	NONE = 0,
	ERROR = 1,
	WARNING = 2,
	INFO = 3,
	NUM_TYPES	
};

template<typename clock_type>
class Logging
{
public:
	using tp = std::chrono::time_point<clock_type>;
private:
	struct StartEventInfo
	{
		string info;
	};
	bool active;
	unique_ptr<StartEventInfo> start_event_info;

	/* designated initializer list sadly not supported before c++20 :( */
	static const string type_names[NUM_TYPES];
	unsigned int levels[NUM_TYPES] =	
	{
		/*[NONE] =*/	numeric_limits<unsigned int>::max(),
		/*[ERROR] =*/	0,
		/*[WARNING] =*/	0,
		/*[INFO] =*/	0
	};
	LogStream log_stream;

	bool std_out_active;
	ostream* std_out_mapping [NUM_TYPES]=
	{
		/*[NONE] =*/	&std::cout,
		/*[ERROR] =*/	&std::clog,
		/*[WARNING] =*/	&std::cout,
		/*[INFO] =*/	&std::cout
	};

public:
	//for when the logging does not log
	static LogStream no_log_stream;

	void startLogging();
	void setMinLevel(TYPE type, unsigned int lvl);
	void addOfstream(ofstream&& stream);

	void activateStdOut(bool activate = true);
	void setStdOutMapping(TYPE type, ostream&);
	bool stdOutActive() const;

	LogStream& log(TYPE type, unsigned int lvl = 100);
	LogStream& log(const string& type_name);

	LogStream& error(unsigned int lvl = 100);
	LogStream& warning(unsigned int lvl = 100);
	LogStream& info(unsigned int lvl = 100);

	Logging(const string& info);
	Logging(const string& info, ofstream&& stream);
	Logging(const string& info, vector<ofstream>&& streams);
	Logging();

	Logging(const Logging&) = delete;
	Logging& operator=(const Logging&) = delete;
};

template<typename clock_type>
class TimeOutputWrapper
{
	using outTp = std::chrono::time_point<clock_type>;
public:
	string format;
	const outTp& tp;
	//sadly format string cannot be specified by default parameter
	// because default parameters do not work with template specialization
	TimeOutputWrapper(const outTp& tp)
		: format("%F %T")
		, tp(tp)
	{}

	TimeOutputWrapper(const outTp& tp, string format)
		: format(format)
		, tp(tp)
	{}
};

template<typename clock_type>
ostream& operator<< (ostream& os, const TimeOutputWrapper<clock_type>& tow)
{
	std::time_t now_c = clock_type::to_time_t(tow.tp);
	os << std::put_time(std::localtime(&now_c), tow.format.c_str());
	return os;
}

template<>
ostream& operator<< (ostream& os, const TimeOutputWrapper<std::chrono::high_resolution_clock>& tow)
{
	std::time_t now_c = std::chrono::high_resolution_clock::to_time_t(tow.tp);
	os	<< std::put_time(std::localtime(&now_c), tow.format.c_str()) 
		<< '.' << tow.tp.time_since_epoch().count() % 1000000000;
	return os;
}


} /* namespace log */

#include "Logging.hxx"

#endif /* LOGGING_HPP */
