#pragma once

#include <unordered_map>
#include <string>

enum class PlayerUpgrade
{
	DoubleRadius,
	DoubleCleanliness,
	DoubleSpeed
};

static std::unordered_map<std::string, PlayerUpgrade> PlayerUpgradesByName = {
	{"DoubleRadius", PlayerUpgrade::DoubleRadius},
	{"DoubleCleanliness", PlayerUpgrade::DoubleCleanliness},
	{"DoubleSpeed", PlayerUpgrade::DoubleSpeed},
};