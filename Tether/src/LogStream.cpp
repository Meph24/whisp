#include "LogStream.hpp"

#include <iostream>

using std::cout;

namespace log {

void LogStream::addOfstream(ofstream&& fs)
{
	fss.emplace_back(std::move(fs));
}

LogStream::LogStream(ofstream&& stream)
{
	addOfstream(std::move(stream));
}

LogStream::LogStream(vector<ofstream>&& streams)
	: fss(std::move(streams))
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
