#include "exception.hpp"
#include <sstream>

Exception::Exception(int line, const char* file) noexcept
{
	_line = line;
	_file = file;
}

const char* Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << type() << "\n" << origin_string();
	return (what_buffer = oss.str()).c_str();
}

const char* Exception::type() const noexcept
{
	return "loosehead exception";
}

int Exception::line() const noexcept
{
	return _line;
}

const std::string& Exception::file() const noexcept
{
	return _file;
}

std::string Exception::origin_string() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << _file << "\n" << "[Line] " << _line;
	return oss.str();
}