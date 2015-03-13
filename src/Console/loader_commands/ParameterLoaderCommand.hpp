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
			class ParameterLoaderCommand : public LoaderCommand
			{
			public:
				ParameterLoaderCommand()
				{
					this->cues.push_back(GG_STR("p"));
					this->cues.push_back(GG_STR("par"));
					this->cues.push_back(GG_STR("param"));
					this->cues.push_back(GG_STR("parameter"));
				}

				virtual String GetName() const { return GG_STR("Parameter"); };

				virtual String GetHelpString() const { return GG_STR("param [name] [value] - Sets value of a script parameter to value. If value is not set, prints its current value."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					StringStream ss(arguments);
					String parameterName;
					ss >> parameterName;

					if (ss.fail())
					{
						loader->GetOut() << GG_STR("Script parameter name not specified.") << std::endl << std::endl;
						return;
					}
					
					String parameterStringValue;
					ss >> parameterStringValue;
					if (ss.fail())
					{
						if (loader->GetParameterValues().find(parameterName) != loader->GetParameterValues().end())
						{
							loader->GetOut() << GG_STR("Value of script parameter \"") << parameterName << GG_STR("\": \"") << loader->GetParameterValues()[parameterName] << GG_STR("\"");
						}
						else
						{
							loader->GetOut() << GG_STR("Value of script parameter \"" << parameterName << "\" is not set.");
						}
						loader->GetOut() << std::endl << std::endl;
						return;
					}

					loader->GetParameterValues()[parameterName] = parameterStringValue;

					loader->GetOut() << GG_STR("Set value of script parameter \"") << parameterName << GG_STR("\" to \"") << loader->GetParameterValues()[parameterName] << GG_STR("\".");
					loader->GetOut() << std::endl << std::endl;
				}
			};
		}
	}
}