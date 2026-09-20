#pragma once

#include <unordered_map>
#include <string>

enum class PlayerUpgrade
{
	DoubleRadius,
	DoubleCleanliness,
	DoubleCleanSpeed,
	DoubleMoveSpeed
};

static std::unordered_map<std::string, PlayerUpgrade> PlayerUpgradesByName = {
	{"DoubleRadius", PlayerUpgrade::DoubleRadius},
	{"DoubleCleanliness", PlayerUpgrade::DoubleCleanliness},
	{"DoubleCleanSpeed", PlayerUpgrade::DoubleCleanSpeed},
	{"DoubleMoveSpeed", PlayerUpgrade::DoubleMoveSpeed},
};