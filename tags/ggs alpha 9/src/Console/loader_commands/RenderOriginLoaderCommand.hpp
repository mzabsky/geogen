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
			class RenderOriginLoaderCommand : public LoaderCommand
			{
			public:
				RenderOriginLoaderCommand()
				{
					this->cues.push_back(GG_STR("o"));
					this->cues.push_back(GG_STR("ro"));
					this->cues.push_back(GG_STR("origin"));
					this->cues.push_back(GG_STR("rorigin"));
					this->cues.push_back(GG_STR("renorigin"));
					this->cues.push_back(GG_STR("renderorigin"));
				}

				virtual String GetName() const { return GG_STR("Render origin"); };

				virtual String GetHelpString() const { return GG_STR("rorigin [x] [y] - Sets render origin. If no arguments passed, displays current render origin."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						loader->GetOut() << GG_STR("Render origin: ");
						loader->GetOut() << loader->GetRenderOrigin().ToString();
						loader->GetOut() << std::endl << std::endl;
						return;
					}

					StringStream argumentsStream(arguments);

					String xString;
					argumentsStream >> xString;

					Coordinate newX;
					{
						StringStream ss(xString);
						ss >> newX;

						if (ss.fail())
						{
							loader->GetOut() << GG_STR("Invalid X.") << std::endl << std::endl;
							return;
						}
					}

					String yString;
					argumentsStream >> yString;

					Coordinate newY;
					if (argumentsStream.fail())
					{
						newY = newX;
					}
					else
					{
						StringStream ss(yString);
						ss >> newY;

						if (ss.fail())
						{
							loader->GetOut() << GG_STR("Invalid Y.") << std::endl << std::endl;
							return;
						}
					}

					loader->SetRenderOrigin(Point(newX, newY));

					loader->GetOut() << GG_STR("Render size: ");
					loader->GetOut() << loader->GetRenderOrigin().ToString();
					loader->GetOut() << std::endl << std::endl;
				}
			};
		}
	}
}