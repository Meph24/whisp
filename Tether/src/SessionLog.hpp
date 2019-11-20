#ifndef SESSIONLOG_HPP
#     define SESSIONLOG_HPP

#include "Logging.hpp"

#include <chrono>
#include <filesystem>

using log::Logging;
namespace fs = std::filesystem;

namespace log {

using SysTp = std::chrono::time_point<std::chrono::system_clock>;

string sanitizeFilename(const string& name);

struct Session
{
public:
	Session(const string& info, const fs::path& root_dir);
	SysTp start;
	string info;
	fs::path root_dir;
};

string assembleSessionMessage(	const Session& session,
								const string& topic,
								const string& info);

fs::path assembleFilePath(	const Session& session,
							const string& topic		);

template<typename clock_type>
class SessionLog : public Logging<clock_type>
{
public:
	SessionLog( const Session& session,
				const string& topic, 
				const string& info);
};

} /* namespace log */

#include "SessionLog.hxx"

#endif /* SESSIONLOG_HPP */
