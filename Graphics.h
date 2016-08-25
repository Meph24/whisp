#pragma once

#include <thread>
#include "IMediaHandle.h"
#include <vector>


/*
// Render data ressources
*/
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"



/*
//	Graphics provides renderint handled in its own thread
*/
class Graphics
{
public: 


	/*
	//	All settings concerning Graphics can be passed through this struct
	*/
	struct settings
	{
		int someplaceholdersetting; // delete when add some real settings to it
	};


private:

	/*
	//	An interface for the MediaHandle to access window contexts for initialisation of the graphic thread
	*/
	IMediaHandle& mediaHandle;


	/*
	//	The thread the graphics will work in
	//	All the rendering is done in this separate thread
	*/
	std::thread graphicThread;

	/*
	//	a bool which controls the thread
	//	the bool is the condition for the rendering loop
	//	by setting to false the render loop can be stopped
	*/
	bool thread_isOn;



	/*
	//	Render data
	*/
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	Camera* camera;
	Transform* transform;


	//test_begin
	float counter;
	//test_begin


	/*
	//	Main-function of the graphic thread
	*/
	void graphicMain();

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

	/*
	//	Initializes threads and render data
	*/
	Graphics(IMediaHandle& mediaHandle, settings settings);
	~Graphics();

	/*
	//	starts the graphic thread
	//	can also be used to restart the thread completely by calling while the thread is still running
	*/
	void start();

	/*
	//	stops the graphic thread
	*/
	void stop();

	

	
};

