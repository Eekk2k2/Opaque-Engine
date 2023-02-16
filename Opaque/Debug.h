#pragma once

#include <iostream>

// Log levels in ANSI Escape Codes https://en.wikipedia.org/wiki/ANSI_escape_code
#define LOG_LEVEL_INFO "\033[0m"
#define LOG_LEVEL_WARNING "\033[33m"
#define LOG_LEVEL_ERROR "\033[31m"
#define LOG_LEVEL_CRITICAL "\033[31;1m"
#define LOG_LEVEL_NOTICE ""

// Log types
#define LOG_TYPE_EASY		0		// If set to easy only this one will be shown
#define LOG_TYPE_MEDIUM		1		// If set to medium both easy and medium will be shown
#define LOG_TYPE_VERBOSE	2		// If set to verbose all Easy, Medium and Verbose will be shown.

#define LOG_TYPE_DEBUG		3		// ONLY in debug
#define LOG_TYPE_RELEASE	4		// ONLY in release

class Debug
{
public:
	Debug();
	~Debug();

	// ------------------------------ Command Line ---

	/// <summary>
	/// Print something to the console.
	/// </summary>
	/// <param name="LogMessage"></param>
	void Log(const char* LogMessage);

	/// <summary>
	/// Print something to the console using a log type. (ERROR, MESSAGE, WARNING, HEADSUP)
	/// </summary>
	/// <param name="LogMessage"></param>
	/// <param name="ANSI"></param>
	void Log(const char* LogMessage, const char* LogLevel);

	// ------------------------------ In Scene ---

	void Ray(/*Not Finished*/); // TODO : Implement 

	void Sphere(/*Not Finished*/); // TODO : Implement 

	void Cube(/*Not Finished*/); // TODO : Implement 

private:

};


