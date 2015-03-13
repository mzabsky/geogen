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
			class RenderScaleLoaderCommand : public LoaderCommand
			{
			public:
				RenderScaleLoaderCommand()
				{
					this->cues.push_back(GG_STR("rsc"));
					this->cues.push_back(GG_STR("rensc"));
					this->cues.push_back(GG_STR("rscale"));
					this->cues.push_back(GG_STR("renscale"));
					this->cues.push_back(GG_STR("renderscale"));
				}

				virtual String GetName() const { return GG_STR("Render scale"); };

				virtual String GetHelpString() const { return GG_STR("rscale [scale] - Sets render scale. If no argument passed, displays current render scale."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						loader->GetOut() << GG_STR("Render scale: ") << loader->GetRenderScale() << std::endl << std::endl;
						return;
					}

					StringStream argumentsStream(arguments);

					String scaleString;
					argumentsStream >> scaleString;

					Scale newScale;
					if (scaleString == GG_STR("a") || scaleString == GG_STR("auto") || scaleString == GG_STR("automatic") || scaleString == GG_STR("d") || scaleString == GG_STR("def") || scaleString == GG_STR("default"))
					{
						newScale = 1;
					}
					else
					{
						StringStream ss(scaleString);
						ss >> newScale;

						if (ss.fail())
						{
							loader->GetOut() << GG_STR("Invalid scale.") << std::endl << std::endl;
							return;
						}
					}

					loader->SetRenderScale(newScale);

					loader->GetOut() << GG_STR("Render scale: ") << loader->GetRenderScale() << std::endl << std::endl;
				}
			};
		}
	}
}