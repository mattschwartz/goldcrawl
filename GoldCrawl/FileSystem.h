#pragma once

#include <SDL.h>
#include <string>
#include <filesystem>
#include <fstream>

namespace bacon::fs
{
	inline std::string resolve(const std::string& assetName)
	{
		// todo - figure this out when we start building for web
		return assetName;
	}

	/// <summary>
	/// https://stackoverflow.com/questions/2912520/read-file-contents-into-a-string-in-c
	/// </summary>
	inline std::optional<std::string> readText(const std::string& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			SDL_LogError(0, "filepath %s does not exist", filepath.c_str());
			return std::nullopt;
		}

		std::ifstream ifs(filepath);
		std::string content((std::istreambuf_iterator<char>(ifs)),
			std::istreambuf_iterator<char>());
		return content;
	}
}

