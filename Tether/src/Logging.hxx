#ifndef LOGGING_CPP
#     define LOGGING_CPP

#include <iostream>

using std::cout;

namespace log{

template<typename clock_type>
LogStream Logging<clock_type>::no_log_stream = LogStream();

template<typename clock_type>
string Logging<clock_type>::type_names[NUM_TYPES] =	
	{
		/*[NONE] =*/	"",
		/*[ERROR] =*/	"ERROR",
		/*[WARNING] =*/	"WARNING",
		/*[INFO] =*/	"INFO"
	};

template<typename clock_type>
void Logging<clock_type>::setMinLevel(Logging::TYPE type, unsigned int lvl)
{
	if(type > NUM_TYPES) return;

	levels[type] = lvl;
}

template<typename clock_type>
void Logging<clock_type>::addOfstream(ofstream&& stream)
{
	log_stream.addOfstream(std::move(stream));
}

template<typename clock_type>
LogStream& Logging<clock_type>::log(Logging::TYPE tid, unsigned int lvl)
{
	if(!active && !log_stream.noStream()) startLogging();

	if(lvl < levels[tid]) return Logging::no_log_stream;

	string type_name_tag;

	switch(tid)
	{
		case TYPE::NONE:
			type_name_tag = "";
			break;
		/* range type case statement in c++20 :( 
		case TYPE::ERROR ... TYPE::NUM_TYPES-1: */
		case TYPE::ERROR: case TYPE::WARNING: case TYPE::INFO:
			type_name_tag = '['+type_names[tid]+']';
			break;
		default:
			type_name_tag = '['+std::to_string(tid)+']';
			break;
	}
	
	auto t = clock_type::now();

	log_stream	<< '\n'
				<< "[" << TimeOutputWrapper<clock_type>(t) << ']' 
				<< '[' << std::setw(6) << lvl << ']'
				<< type_name_tag 
				<< "|";

	return log_stream;
}

template<typename clock_type>
LogStream& Logging<clock_type>::error(unsigned int lvl)
{
	return log(TYPE::ERROR, lvl);
}


template<typename clock_type>
LogStream& Logging<clock_type>::warning(unsigned int lvl)
{
	return log(TYPE::WARNING, lvl);
}

template<typename clock_type>
LogStream& Logging<clock_type>::info(unsigned int lvl)
{
	return log(TYPE::INFO, lvl);
}


template<typename clock_type>
void Logging<clock_type>::startLogging()
{
	if (active) return;
	log_stream		<< start_event_info->info << '\n'
					<< "Logging starts" << '\n';
	active = true;
	start_event_info.reset();
}


template<typename clock_type>
Logging<clock_type>::Logging(const string& info)
	: active(false)
	, start_event_info(new StartEventInfo)
{
	start_event_info->info = info;
}


template<typename clock_type>
Logging<clock_type>::Logging(const string& info, ofstream&& stream)
	: active(false)
	, start_event_info(new StartEventInfo)
	, log_stream (std::move(stream))
{
	start_event_info->info = info;
}

template<typename clock_type>
Logging<clock_type>::Logging(const string& info, vector<ofstream>&& streams)
	: start_event_info(new StartEventInfo)
	, log_stream (std::move(streams))
{
	start_event_info->info = info;
}

template<typename clock_type>
Logging<clock_type>::Logging()
	:	Logging("")
{}

} /* namespace log */
	
#endif /* LOGGING_CPP */
