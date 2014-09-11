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
			class GenTilesLoaderCommand : public LoaderCommand
			{
			private:
				bool TryGeteRectanglFromArgs(Loader* loader, String arguments, Rectangle& out) const
				{
					StringStream argumentsStream(arguments);

					String arg1, arg2, arg3, arg4;

					unsigned argCount = 0;
					argumentsStream >> arg1;
					if (!argumentsStream.fail())
					{
						argumentsStream >> arg2;
						if (!argumentsStream.fail())
						{
							argumentsStream >> arg3;
							if (!argumentsStream.fail())
							{
								argumentsStream >> arg4;
								if (argumentsStream.fail())
								{
									argCount = 3;
								}
							}
							else
							{
								argCount = 2;
							}
						}
						else
						{
							argCount = 1;
						}
					}
					else
					{
						argCount = 0;
					}

					if (argCount == 0)
					{
						out = Rectangle(Point(COORDINATE_MIN, COORDINATE_MIN), SIZE2D_MAX);
						return true;
					}
					else if (argCount == 1)
					{
						Size1D size;
						StringStream parseStream(arg1);
						parseStream >> size;

						if (parseStream.fail())
						{
							loader->GetOut() << GG_STR("Invalid width/height.");
							return false;
						}

						out = Rectangle(Point(0, 0), Size2D(size, size));
						return true;
					}
					else if (argCount == 2)
					{
						Size1D width, height;
						StringStream widthParseStream(arg1);
						widthParseStream >> width;

						if (widthParseStream.fail())
						{
							loader->GetOut() << GG_STR("Invalid width.");
							return false;
						}

						StringStream heightParseStream(arg2);
						heightParseStream >> height;

						if (heightParseStream.fail())
						{
							loader->GetOut() << GG_STR("Invalid height.");
							return false;
						}

						out = Rectangle(Point(0, 0), Size2D(width, height));
						return true;
					}
					else if (argCount == 3)
					{
						loader->GetOut() << GG_STR("Height not specified.");
						return false;
					}
					else
					{
						Coordinate x, y;
						Size1D width, height;

						StringStream xParseStream(arg1);
						xParseStream >> x;

						if (xParseStream.fail())
						{
							loader->GetOut() << GG_STR("Invalid origin X.");
							return false;
						}

						StringStream yParseStream(arg2);
						yParseStream >> y;

						if (yParseStream.fail())
						{
							loader->GetOut() << GG_STR("Invalid origin Y.");
							return false;
						}

						StringStream widthParseStream(arg3);
						widthParseStream >> width;

						if (widthParseStream.fail())
						{
							loader->GetOut() << GG_STR("Invalid width.");
							return false;
						}

						StringStream heightParseStream(arg4);
						heightParseStream >> height;

						if (heightParseStream.fail())
						{
							loader->GetOut() << GG_STR("Invalid height.");
							return false;
						}

						out = Rectangle(Point(x, y), Size2D(width, height));
						return true;
					}

					return false;
				}
				void RenderTile(Loader* loader, Point origin, Size2D size, Rectangle bounds) const
				{
					clock_t startTime = clock();

					loader->GetOut() << GG_STR("Tile ") << origin.ToString() << GG_STR(": Runnning script.") << std::endl;

					Rectangle renderRectangle(origin, size);
					Rectangle actualRenderRectangle = Rectangle::Intersect(renderRectangle, bounds);

					runtime::ScriptParameters scriptParameters = loader->CreateScriptParameters();
					scriptParameters.SetRenderRectangle(actualRenderRectangle);
					runtime::VirtualMachine vm(*loader->GetCompiledScript(), scriptParameters);
					vm.Run();

					loader->GetOut() << GG_STR("Tile ") << origin.ToString() << GG_STR(": Rendering.") << std::endl;

					renderer::Renderer renderer(vm.GetRenderingSequence());
					renderer.CalculateMetadata();

					loader->GetOut() << GG_STR("0% ");

					int i = 0;
					while (renderer.GetStatus() == geogen::renderer::RENDERER_STATUS_READY)
					{
						renderer.Step();

						loader->GetOut() << round(renderer.GetProgress() * 10) / 10 << GG_STR("% ");

						i++;
					}

					loader->GetOut() << std::endl;

					loader->GetOut() << GG_STR("Tile ") << origin.ToString() << GG_STR(": Saving maps.") << std::endl;

					StringStream tileNameStream;
					tileNameStream << GG_STR("tile_") << origin.GetX() << GG_STR("_") << origin.GetY() << GG_STR("_");
					loader->SaveRenderedMaps(renderer.GetRenderedMapTable(), tileNameStream.str());

					double seconds = (double)(clock() - startTime) / (double)CLOCKS_PER_SEC;

					loader->GetOut() << GG_STR("Tile ") << origin.ToString() << GG_STR(": Finished in ") << seconds << GG_STR(" seconds.") << std::endl << std::endl;
				}
			public:
				GenTilesLoaderCommand()
				{
					this->cues.push_back(GG_STR("gt"));
					this->cues.push_back(GG_STR("tg"));
					this->cues.push_back(GG_STR("gentile"));
					this->cues.push_back(GG_STR("tilegen"));
					this->cues.push_back(GG_STR("gentiles"));
					this->cues.push_back(GG_STR("generatetiles"));
				}

				virtual String GetName() const { return GG_STR("GenTiles"); };

				virtual String GetHelpString() const { return GG_STR("gentiles [rectX] [rectY] [rectW] [rectH]  - Generate specified rectangle (or infinite if not specified) cut into tiles."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (loader->GetCompiledScript() == NULL)
					{
						loader->GetOut() << GG_STR("No script loaded. Use \"load\" command to load a script.") << std::endl << std::endl;
						return;
					}

					Rectangle bounds;
					if (!this->TryGeteRectanglFromArgs(loader, arguments, bounds))
					{
						loader->GetOut() << std::endl << std::endl;
						return;
					}					

					runtime::ScriptParameters scriptParameters = loader->CreateScriptParameters();
					Rectangle mapRectangle(Point(scriptParameters.IsMapInfinite(DIRECTION_HORIZONTAL) ? COORDINATE_MIN : 0, scriptParameters.IsMapInfinite(DIRECTION_VERTICAL) ? COORDINATE_MIN : 0), Size2D(scriptParameters.IsMapInfinite(DIRECTION_HORIZONTAL) ? runtime::MAP_SIZE_MAX : scriptParameters.GetMapWidth(), scriptParameters.IsMapInfinite(DIRECTION_VERTICAL) ? runtime::MAP_SIZE_MAX : scriptParameters.GetMapHeight()));

					bounds = Rectangle::Intersect(mapRectangle, bounds);

					bool boundsInfiniteHorizontal = bounds.GetSize().GetWidth() == runtime::MAP_SIZE_MAX;
					bool boundsInfiniteVertical = bounds.GetSize().GetHeight() == runtime::MAP_SIZE_MAX;

					clock_t totalStartTime = clock();

					Size2D actualTileSize(
						loader->GetRenderSize().GetWidth() == runtime::MAP_SIZE_AUTOMATIC ? runtime::RENDER_SIZE_DEFAULT : loader->GetRenderSize().GetWidth(),
						loader->GetRenderSize().GetHeight() == runtime::MAP_SIZE_AUTOMATIC ? runtime::RENDER_SIZE_DEFAULT : loader->GetRenderSize().GetHeight());

					loader->GetOut() << GG_STR("Generating tiles in rectangle ") << bounds.ToString() << GG_STR(" with tile size ") << actualTileSize.ToString() << GG_STR(".") << std::endl << std::endl;


					if (boundsInfiniteHorizontal && boundsInfiniteVertical)
					{
						Coordinate spiralTop = 0;
						Coordinate spiralBottom = 0;
						Coordinate spiralLeft = 0;
						Coordinate spiralRight = 0;

						Coordinate currentChangeX = 1;
						Coordinate currentChangeY = 0;

						Coordinate currentX = 0;
						Coordinate currentY = 0;

						while (true)
						{
							if (currentX > spiralRight)
							{
								currentChangeX = 0;
								currentChangeY = +1;
								spiralRight++;
							}
							else if (currentX < spiralLeft)
							{
								currentChangeX = 0;
								currentChangeY = -1;
								spiralLeft--;
							}
							else if (currentY > spiralTop)
							{
								currentChangeX = -1;
								currentChangeY = 0;
								spiralTop++;
							}
							else if (currentY < spiralBottom)
							{
								currentChangeX = +1;
								currentChangeY = 0;
								spiralBottom--;
							}

							this->RenderTile(loader, Point(currentX * actualTileSize.GetWidth(), currentY * actualTileSize.GetHeight()), actualTileSize, bounds);

							currentX += currentChangeX;
							currentY += currentChangeY;
						}
					}
					else if (boundsInfiniteHorizontal)
					{						
						for (Coordinate x = 0;; x = x <= 0 ? -x + actualTileSize.GetWidth() : -x)
						for (Coordinate y = bounds.GetPosition().GetY(); y < bounds.GetEndingPoint().GetY(); y += actualTileSize.GetHeight())
						{
							this->RenderTile(loader, Point(x, y), actualTileSize, bounds);
						}
					}
					else if (boundsInfiniteVertical)
					{
						for (Coordinate y = 0;; y = y <= 0 ? -y + actualTileSize.GetHeight() : -y)
						for (Coordinate x = bounds.GetPosition().GetX(); x < bounds.GetEndingPoint().GetX(); x += actualTileSize.GetWidth())
						{
							this->RenderTile(loader, Point(x, y), actualTileSize, bounds);
						}
					}
					else
					{
						for (Coordinate y = bounds.GetPosition().GetY(); y < bounds.GetEndingPoint().GetY(); y += actualTileSize.GetHeight())
						for (Coordinate x = bounds.GetPosition().GetX(); x < bounds.GetEndingPoint().GetX(); x += actualTileSize.GetWidth())
						{
							this->RenderTile(loader, Point(x, y), actualTileSize, bounds);
						}
					}


					double seconds = (double)(clock() - totalStartTime) / (double)CLOCKS_PER_SEC; 
					loader->GetOut() << GG_STR("Batch finished in ") << seconds << GG_STR(" seconds.") << std::endl << std::endl;
				}
			};
		}
	}
}