/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <vector>
#include <fstream>


#include "../LoaderCommand.hpp"
#include "../Loader.hpp"
#include "../ConsoleUtils.hpp"
#include <GeoGen/GeoGen.hpp>

namespace geogen
{
	namespace console
	{
		namespace loader_commands
		{		
			class RandomSeedLoaderCommand : public LoaderCommand
			{
			public:
				RandomSeedLoaderCommand()
				{
					this->cues.push_back(GG_STR("seed"));
					this->cues.push_back(GG_STR("rseed"));
					this->cues.push_back(GG_STR("randseed"));
					this->cues.push_back(GG_STR("randomseed"));
				}

				virtual String GetName() const { return GG_STR("Random seed"); };

				virtual String GetHelpString() const { return GG_STR("seed [s] - Sets the random seed to a specified number. If string is entered, the seed will be its hash."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (arguments != GG_STR(""))
					{
						loader->SetRandomSeed(arguments);
					}

					loader->GetOut() << GG_STR("Random seed: \"") << loader->GetRandomSeed() << GG_STR("\"");
					loader->GetOut() << GG_STR(" (") << random::RecognizeSeed(loader->GetRandomSeed()) << GG_STR(")");
					
					loader->GetOut() << std::endl << std::endl;
				}
			};
		}
	}
}