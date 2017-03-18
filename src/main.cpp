
#include "MainApp.h"

#include <iostream>
#include <exception>


#include "mat4.h"
int main()
{
	
	try
	{
		MainApp A;
		A.run();
	}

	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	

	//getchar();
	return 0;
}
