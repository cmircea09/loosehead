#pragma once

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(int line, const char* file) noexcept;
	
	const char* what() const noexcept override;
	virtual const char* type() const noexcept;
	int line() const noexcept;
	const std::string& file() const noexcept;
	std::string origin_string() const noexcept;

private:
	int _line;
	std::string _file;
	
protected:
	mutable std::string what_buffer;
};