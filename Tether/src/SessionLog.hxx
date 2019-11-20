#include <sstream>
#include <string>

#include <set>

using std::string;

namespace log {

template<typename clock_type>
SessionLog<clock_type>::SessionLog(
		const Session& session,
		const string& topic, 
		const string& info)
	: Logging<clock_type>(	
		assembleSessionMessage(session, topic, info), 
		ofstream(assembleFilePath(session, topic))
		)
{}

} /* namespace log */
