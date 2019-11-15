#include "LogStream.hpp"

#include <iostream>

using std::cout;

namespace log {

void LogStream::setStdOut(ostream& os)
{
	std_out = &os;
}

void LogStream::activateStdOut(bool activate)
{
	stdout_active = activate;
}

bool LogStream::stdOutActive() const
{
	return stdout_active;
}

void LogStream::addOfstream(ofstream&& fs)
{
	fss.emplace_back(std::move(fs));
}

LogStream::LogStream(ofstream&& stream)
	: std_out(&std::cout)
	, stdout_active(false)
{
	addOfstream(std::move(stream));
}

LogStream::LogStream(vector<ofstream>&& streams)
	: fss(std::move(streams))
	, std_out(&std::cout)
	, stdout_active(false)
{}

LogStream::~LogStream()
{
	for(auto& ofs : fss)
		ofs.close();
}

bool LogStream::noStream() const
{
	return fss.empty();
}

} /* namespace log */
