#pragma once

#include <string>
#include <exception>

namespace bacon
{
	class SDLError : public std::exception
	{
	public:
		SDLError(const std::string& msg) : _msg(msg) {}
		virtual const char* what() const noexcept { return _msg.c_str(); }

	private:
		std::string _msg;
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

