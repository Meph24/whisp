#include "SessionLog.hpp"

namespace log {

string assembleSessionMessage(	const Session& session,
								const string& topic,
								const string& info)
{
	std::stringstream ss;

	ss << "Session Log under topic: " << topic << '\n';  
	ss << "Session started " << TimeOutputWrapper(session.start, "%F %T") << '\n';
	ss << '\n' << info << "\n\n";

	return ss.str();
}

fs::path assembleFilePath(	const Session& session,
							const string& topic		)
{
	std::time_t now_c = std::chrono::system_clock::to_time_t(session.start);
	std::stringstream time;
    time << std::put_time(std::localtime(&now_c), "%F_%H-%M-%S");
	fs::path dir = session.root_dir / (string("log_") + time.str());
	if(!fs::exists(dir))
	{
		fs::create_directory(dir);
	}
	return dir / (sanitizeFilename(topic)+".log");
}

string sanitizeFilename(const string& name)
{
	std::set<char> filename_character_whitelist =
		{'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','_'};

	string newstring = "";
	newstring.reserve(name.size());

	for(char c : name)
	{
		if(	filename_character_whitelist.find(c) 
				== 
			filename_character_whitelist.end() )

			newstring += '_';
		else
			newstring += c;
	}
	return newstring;
}

Session::Session(const string& info, const fs::path& root_dir)
	: start(std::chrono::system_clock::now())
	, info("")
	, root_dir(root_dir)
{}

} /* namespace log */
