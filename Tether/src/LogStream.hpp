#ifndef LOGSTREAM_HPP
#     define LOGSTREAM_HPP

#include <fstream>
#include <ostream>
#include <string>
#include <vector>

using std::ofstream;
using std::ostream;
using std::vector;

namespace log{

class LogStream
{
	vector<ofstream> fss;

	std::ostream* std_out;
	bool stdout_active;
	
public:
	void setStdOut(ostream& os);
	void activateStdOut(bool activate = true);
	bool stdOutActive() const;

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
		if(stdout_active) (*std_out) << to_stream;

		for(auto& fs : fss)
		{
			fs << to_stream;
		}
		return (*this);
	}
};

} /* namespace log */

#endif /* LOGSTREAM_HPP */
