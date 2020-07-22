#pragma once

#include <thread>
#include <vector>

class IMediaHandle;


/*
//	Graphics provides renderint handled in its own thread
*/
class Graphics
{
public:

	/*
	//	An interface for the MediaHandle to access window contexts for initialisation of the graphic thread
	*/
	IMediaHandle& mediaHandle;

	/*
	//	rendering of RenderData
	*/
	void render();

	/*
	// sets parameters for a Graphics::windowClear() call
	*/
	void setClearParameters(float r, float g, float b, float a = 255, float depth = 1.0f);

	/*
	//	clears the window with previously set parameters
	//  clears depth and color
	*/
	void clear();

	/*
	//	sets clear parameters new and calls clear() immediatly after
	*/
	void clear(float r, float g, float b, float a = 255, float depth = 1.0f);

	/*
	//	Initializes OpenGL data / Render data
	//	Can be called after getting a new Render Target (Window/Context) to set OpenGL towards this new Target
	//	Is called when starting the graphic thread
	*/
	void init();

	/*
	//	
	*/
	void display();

public:

	/*
	//	Constructor initializes threads
	//	Render data is not initialized in this constructor
	*/
	Graphics(IMediaHandle& mediaHandle);
};

