#ifndef LOGSTREAM_HPP
#     define LOGSTREAM_HPP

#include <memory>
#include <fstream>
#include <ostream>
#include <vector>

using std::ofstream;
using std::ostream;
using std::unique_ptr;
using std::vector;

namespace log{

class LogStream
{
	vector<ofstream> fss;

public:
	void addOfstream(ofstream&& fs);

	LogStream() = default;
	LogStream(ofstream&& stream);
	LogStream(vector<ofstream>&& streams);

	LogStream(const LogStream&) = delete;
	LogStream& operator=(const LogStream&) = delete;

	~LogStream();

	bool noStream() const;

	template<typename T>
	LogStream& operator<<( const T& to_stream )
	{
		for(auto& fs : fss)
		{
			fs << to_stream;
		}
		return (*this);
	}
};

} /* namespace log */

#endif /* LOGSTREAM_HPP */
