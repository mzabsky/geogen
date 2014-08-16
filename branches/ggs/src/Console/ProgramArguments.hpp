#pragma once

#include <vector>
#include <map>
#include <ctime>

#include "../GeoGen/GeoGen.hpp"
namespace geogen
{
	namespace console
	{
		struct ProgramArguments
		{
			bool debug = false;
			bool displayHelp = false;
			String inputFile = GG_STR("");
			String outputDirectory = GG_STR("./");
			random::RandomSeed seed = (random::RandomSeed)time(time_t());
			std::vector<String> scriptArgumentsStrings;
		};
	}
}