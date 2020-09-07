#ifndef WINDOW_HPP
#     define WINDOW_HPP

#include "event.hpp"

struct Window : public InputEventSource
{
	virtual bool isOpen() const = 0;
	virtual void close() = 0;
};

#endif /* WINDOW_HPP */
