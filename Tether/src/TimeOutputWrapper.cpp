#include "TimeOutputWrapper.hpp"
	
template<>
ostream& operator<< (ostream& os, const TimeOutputWrapper<std::chrono::high_resolution_clock>& tow)
{
	std::time_t now_c = std::chrono::high_resolution_clock::to_time_t(tow.tp);
	os	<< std::put_time(std::localtime(&now_c), tow.format.c_str()) 
		<< '.' << tow.tp.time_since_epoch().count() % 1000000000;
	return os;
}

