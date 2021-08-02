#include "exception.hpp"
#include <sstream>

Exception::Exception(int line, const char* file)
{
	_line = line;
	_file = file;
}

const char* Exception::what()
{
	std::ostringstream oss;
	oss << type() << "\n" << origin_string();
	return (what_buffer = oss.str()).c_str();
}

const char* Exception::type()
{
	return "loosehead exception";
}

int Exception::line()
{
	return _line;
}

const std::string& Exception::file()
{
	return _file;
}

std::string Exception::origin_string()
{
	std::ostringstream oss;
	oss << "[File] " << _file << "\n" << "[Line] " << _line;
	return oss.str();
}