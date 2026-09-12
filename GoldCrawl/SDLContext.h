#pragma once

#include <string>
#include <exception>

namespace bacon
{
	class SDLError : public std::exception
	{
	public:
		SDLError(const std::string& msg) : std::exception(msg.c_str()) {}
	};

	/// <summary>
	/// helper class to manage SDL context and resources
	/// </summary>
	class SDLContext
	{
	public:
		SDLContext();
		~SDLContext();
	};
}

