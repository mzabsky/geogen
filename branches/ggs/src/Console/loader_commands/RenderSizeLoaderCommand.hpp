#pragma once

#include <vector>
#include <fstream>


#include "../LoaderCommand.hpp"
#include "../Loader.hpp"
#include "../ConsoleUtils.hpp"
#include "../../GeoGen/GeoGen.hpp"
#include "../../GeoGen/utils/StringUtils.hpp"

namespace geogen
{
	namespace console
	{
		namespace loader_commands
		{
			static void SerializeSizeWithSpecial(OStream& out, Size2D size)
			{
				out << GG_STR("[");

				if (size.GetWidth() == runtime::MAP_SIZE_AUTOMATIC)
				{
					out << GG_STR("auto");
				}
				else
				{
					out << size.GetWidth();
				}

				out << GG_STR(", ");

				if (size.GetHeight() == runtime::MAP_SIZE_AUTOMATIC)
				{
					out << GG_STR("auto");
				}
				else
				{
					out << size.GetHeight();
				}

				out << GG_STR("]");
			}

			class RenderSizeLoaderCommand : public LoaderCommand
			{
			public:
				RenderSizeLoaderCommand()
				{
					this->cues.push_back(GG_STR("size"));
					this->cues.push_back(GG_STR("rsize"));
					this->cues.push_back(GG_STR("rensize"));
					this->cues.push_back(GG_STR("rendersize"));
				}

				virtual String GetName() const { return GG_STR("Render size"); };

				virtual String GetHelpString() const { return GG_STR("rsize [w] [h] - Sets render size of the map. If no arguments passed, displays current render size. Use \"auto\" to determine the size automatically."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						loader->GetOut() << GG_STR("Render size: ");
						SerializeSizeWithSpecial(loader->GetOut(), loader->GetRenderSize());
						loader->GetOut() << std::endl << std::endl;
						return;
					}

					StringStream argumentsStream(arguments);

					String widthString;
					argumentsStream >> widthString;

					Size1D newWidth;
					if (widthString == GG_STR("a") || widthString == GG_STR("auto") || widthString == GG_STR("automatic") || widthString == GG_STR("d") || widthString == GG_STR("def")|| widthString == GG_STR("default"))
					{
						newWidth = runtime::MAP_SIZE_AUTOMATIC;
					}
					else 
					{
						StringStream ss(widthString);
						ss >> newWidth;

						if (ss.fail())
						{
							loader->GetOut() << GG_STR("Invalid width.") << std::endl << std::endl;
							return;
						}
					}

					Size1D newHeight;
					if (argumentsStream.eof())
					{
						newHeight = newWidth;
					}
					else
					{
						String heightString;
						argumentsStream >> heightString;

						if (heightString == GG_STR("a") || heightString == GG_STR("auto") || heightString == GG_STR("automatic") || widthString == GG_STR("d") || widthString == GG_STR("def") || widthString == GG_STR("default"))
						{
							newHeight = runtime::MAP_SIZE_AUTOMATIC;
						}
						else
						{
							StringStream ss(heightString);
							ss >> newHeight;

							if (ss.fail())
							{
								loader->GetOut() << GG_STR("Invalid height.") << std::endl << std::endl;
								return;
							}
						}
					}

					loader->SetRenderSize(Size2D(newWidth, newHeight));

					loader->GetOut() << GG_STR("Render size: ");
					SerializeSizeWithSpecial(loader->GetOut(), loader->GetRenderSize());
					loader->GetOut() << std::endl << std::endl;
				}
			};
		}
	}
}