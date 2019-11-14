#ifndef LOGGING_HPP
#     define LOGGING_HPP

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
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

namespace log
{
using SysTp = std::chrono::time_point<std::chrono::system_clock>;

template<typename clock_type>
class Logging
{
	using tp = std::chrono::time_point<clock_type>;
public:
	enum TYPE
	{
		NONE = 0,
		ERROR = 1,
		WARNING = 2,
		INFO = 3,
		NUM_TYPES	
	};
private:
	struct StartEventInfo
	{
		string info;
	};
	bool active;
	unique_ptr<StartEventInfo> start_event_info;

	/* designated initializer list sadly not supported before c++20 :( */
	static string type_names[NUM_TYPES];
	unsigned int levels[NUM_TYPES] =	
	{
		/*[NONE] =*/	numeric_limits<unsigned int>::max(),
		/*[ERROR] =*/	0,
		/*[WARNING] =*/	0,
		/*[INFO] =*/	0
	};
	LogStream log_stream;


public:
	//for when the logging does not log
	static LogStream no_log_stream;

	void startLogging();
	void setMinLevel(Logging::TYPE type, unsigned int lvl);
	void addOfstream(ofstream&& stream);

	LogStream& log(Logging::TYPE tid, unsigned int lvl = 100);

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
	const outTp& tp;
	string format;
public:
	TimeOutputWrapper(const outTp& tp, string format = "%F %T")
		: tp(tp)
		, format(format)
	{}

	friend ostream& operator<< (ostream& os, const TimeOutputWrapper& tow)
	{
		std::time_t now_c = clock_type::to_time_t(tow.tp);
		os << std::put_time(std::localtime(&now_c), tow.format.c_str());
		return os;
	}

};

} /* namespace log */

#include "Logging.hxx"

#endif /* LOGGING_HPP */
