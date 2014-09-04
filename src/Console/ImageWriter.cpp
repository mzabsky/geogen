#include <iomanip>

#include "../png++/png.hpp"

#include "ImageWriter.hpp"

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

		png::image<png::ga_pixel_16>image(heightMap->GetRectangle().GetSize().GetWidth(), heightMap->GetRectangle().GetSize().GetHeight());
		for (size_t y = 0; y < image.get_height(); ++y)
		{
			for (size_t x = 0; x < image.get_width(); ++x)
			{
				image[y][x] = png::ga_pixel_16((unsigned short)((long)-HEIGHT_MIN + (long)(*heightMap)((Coordinate)x, (Coordinate)y)));
			}
		}

		image.write(filename);
	}
	else if (type == renderer::RENDERER_OBJECT_TYPE_HEIGHT_PROFILE)
	{
		HeightProfile* heightProfile = dynamic_cast<HeightProfile*>(object);

		OFStream stream(filename);

		stream << GG_STR("Coordinate;Height") << endl;
		
		for (Coordinate x = 0; x < heightProfile->GetLength(); x++)
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
