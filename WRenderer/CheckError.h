#pragma once

#include "GL\glew.h"
#include <string>
#include <sstream>

#include <iostream>

#ifndef _DEBUG
//do nothing 
#else
#define MY_ASSERT(x, STR) \
	logMsg(x, STR, __FILE__, __LINE__)
#endif

bool logMsg(bool x, const char *msg, char* file, unsigned int line)
{
	if( false == x)
	{
		//do that extra logging information to a file etc.
		std::cout<<"On line " <<line<<":";
		std::cout<<" in file " <<file <<":"; 
		std::cout<<" Error !! Assert "<<msg << " failed\n";
		abort(); 
		return (true);
	}
	else
	{
		return(true);
	}
}


void CheckError() 
{
	int error = glGetError();
	std::string message("Error " + std::to_string(static_cast<long long>(error)));

	MY_ASSERT(error == GL_NO_ERROR, message.c_str());

}