#ifndef TIMEOUTPUTWRAPPER_HPP
#     define TIMEOUTPUTWRAPPER_HPP

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


#endif /* TIMEOUTPUTWRAPPER_HPP */

