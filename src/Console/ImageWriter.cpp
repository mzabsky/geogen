#include <iomanip>

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
						overlayIndex = 255;
					}
					else if (height > 0)
					{
						overlayIndex = 255 + height / 128;

						if (overlayIndex == 255) overlayIndex == 256;
					}
					else
					{
						overlayIndex = 255 + height / 128;
					}

					coloredImage[y][x] = png::rgb_pixel(overlay[overlayIndex][0], overlay[overlayIndex][1], overlay[overlayIndex][2]);
				}
			}	

			coloredImage.write(filename + GG_STR("_colored.png"));
		}
	}
	else if (type == renderer::RENDERER_OBJECT_TYPE_HEIGHT_PROFILE)
	{
		HeightProfile* heightProfile = dynamic_cast<HeightProfile*>(object);

		OFStream stream(filename + GG_STR(".csv"));

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
