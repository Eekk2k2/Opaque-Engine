#include "Debug.h"

Debug::Debug()
{
	// Logging tests
	Log("Normal Log Test"); 
	Log("Log Info", LOG_LEVEL_INFO);
	Log("Log Warning", LOG_LEVEL_WARNING);
	Log("Log Error", LOG_LEVEL_ERROR);
	Log("Log Crititcal", LOG_LEVEL_CRITICAL);
	Log("Log Notice", LOG_LEVEL_NOTICE);
}

Debug::~Debug()
{
}

void Debug::Log(const char* LogMessage)
{
	std::cout << LOG_LEVEL_INFO << LogMessage << "\033[0m" << std::endl;
}

void Debug::Log(const char* LogMessage, const char* LogLevel)
{
	std::cout << LogLevel << LogMessage << "\033[0m" << std::endl;
}

void Debug::Ray()
{

}

void Debug::Sphere()
{

}

void Debug::Cube()
{

}
