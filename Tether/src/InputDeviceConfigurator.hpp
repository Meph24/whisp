#ifndef INPUTDEVICECONFIGURATOR_HPP
#define INPUTDEVICECONFIGURATOR_HPP

#include <glm/vec2.hpp>
using glm::vec2;
#include "Cfg.hpp"

struct InputDeviceConfigurator
{
	enum MouseMode
	{
		pointer, diff, NUM_MOUSEMODES
	};
	virtual void setMouseMode( MouseMode mode ) = 0;
	virtual MouseMode mouseMode() const = 0;

    vec2 turn_sensitivity;
	virtual vec2 turnSensitivity() 
    { 
        return turn_sensitivity; 
    };

    InputDeviceConfigurator( const Cfg& cfg )
	: turn_sensitivity( 0.0431654676, 0.0431654676 )
    {
        const double* sensptr = cfg.getFlt("input", "sensitivityX");
		float sensx = (sensptr) ? *sensptr : turn_sensitivity.x; //default already stored in mouse_sensitivity at init
		sensptr = cfg.getFlt("input", "sensitivityY");
		float sensy = (sensptr) ? *sensptr : turn_sensitivity.y;
		turn_sensitivity = { sensx, sensy };
    }
};

#endif // INPUTDEVICECONFIGURATOR_HPP
