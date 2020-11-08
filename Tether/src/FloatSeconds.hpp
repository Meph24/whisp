#ifndef FLOATSECONDS_HPP
#     define FLOATSECONDS_HPP

#include <chrono>


/**
 * @brief FloatSeconds internal representation. Shall be a floating-point type !
 */
typedef double FloatSecondsFloatingType;

struct FloatSeconds : public std::chrono::duration<FloatSecondsFloatingType>
{ 
	using duration::duration; //inherit constructors
	explicit operator FloatSecondsFloatingType() { return count(); }
	explicit operator float(){ return (float)count(); }
};

#endif /* FLOATSECONDS_HPP */
