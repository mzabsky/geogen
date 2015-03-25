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

#include <iomanip>
#include <cstring>

#include "../png++/png.hpp"

#include "ImageWriter.hpp"
#include "Overlay.hpp"

using namespace geogen;
using namespace genlib;
using namespace console;
using namespace utils;
using namespace std;

void geogen::console::WriteImage(genlib::DataObject* object, renderer::RendererObjectType type, String filename)
{
	if (type == renderer::RENDERER_OBJECT_TYPE_HEIGHT_MAP)
	{
		HeightMap* heightMap = dynamic_cast<HeightMap*>(object);

		{
			png::image<png::ga_pixel_16>image(heightMap->GetRectangle().GetSize().GetWidth(), heightMap->GetRectangle().GetSize().GetHeight());
			for (size_t y = 0; y < image.get_height(); ++y)
			{
				for (size_t x = 0; x < image.get_width(); ++x)
				{
					image[y][x] = png::ga_pixel_16((unsigned short)((long)-HEIGHT_MIN + (long)(*heightMap)((Coordinate)x, (Coordinate)y)));
				}
			}

			image.write(filename + GG_STR(".png"));
		}

		{
			png::image<png::rgb_pixel>coloredImage(heightMap->GetRectangle().GetSize().GetWidth(), heightMap->GetRectangle().GetSize().GetHeight());
			for (size_t y = 0; y < coloredImage.get_height(); ++y)
			{
				for (size_t x = 0; x < coloredImage.get_width(); ++x)
				{
					Height height = (long)(*heightMap)((Coordinate)x, (Coordinate)y);
					unsigned overlayIndex;
					if (height == 0)
					{
						overlayIndex = 256;
					}
					else if (height > 0)
					{
						overlayIndex = 256 + height / 128;

						if (overlayIndex == 256) 
						{
							overlayIndex = 257;
						}
					}
					else
					{
						overlayIndex = 256 + height / 128;
					}

					char r = overlay[overlayIndex][0];
					char g = overlay[overlayIndex][1];
					char b = overlay[overlayIndex][2];

					coloredImage[y][x] = png::rgb_pixel(r, g, b);
				}
			}	

			coloredImage.write(filename + GG_STR("_colored.png"));
		}
	}
	else if (type == renderer::RENDERER_OBJECT_TYPE_HEIGHT_PROFILE)
	{
		HeightProfile* heightProfile = dynamic_cast<HeightProfile*>(object);

		String filenameWithExtension = filename + GG_STR(".csv");
		OFStream stream(filenameWithExtension.c_str());

		stream << GG_STR("Coordinate;Height") << endl;
		
		for (Coordinate x = 0; Size1D(x) < heightProfile->GetLength(); x++)
		{
			stream << (x + heightProfile->GetStart()) << GG_STR(";") << (*heightProfile)(x) << endl;
		}

		stream.flush();

		if (!stream)
		{
			throw exception();
		}
	}

}
