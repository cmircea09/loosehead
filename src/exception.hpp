#pragma once

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(int line, const char* file);
	~Exception() = default;

	const char* what();
	virtual const char* type();
	int line();
	const std::string& file();
	std::string origin_string();

private:
	int _line;
	std::string _file;
	
protected:
	mutable std::string what_buffer;
};