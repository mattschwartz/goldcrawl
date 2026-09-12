#pragma once

#include <string>
#include <filesystem>

namespace bacon::fs
{
	std::string resolve(const std::string& assetName)
	{
		// todo - figure this out when we start building for web
		return assetName; 
	}
}

