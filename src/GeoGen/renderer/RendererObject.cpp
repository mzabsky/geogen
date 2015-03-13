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

#include "RendererObject.hpp"
#include "../InternalErrorException.hpp"

#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../utils/StringUtils.hpp"

using namespace geogen;
using namespace renderer;
using namespace genlib;
using namespace utils;
using namespace std;

String geogen::renderer::RendererObjectTypeToString(RendererObjectType objectType)
{
	switch (objectType)
	{
	case RENDERER_OBJECT_TYPE_HEIGHT_MAP:
		return GG_STR("HeightMap");
	case RENDERER_OBJECT_TYPE_HEIGHT_PROFILE:
		return GG_STR("HeightProfile");
	default:
		throw InternalErrorException(GG_STR("Invalid renderer object type."));
	}
}

void RendererObject::Serialize(IOStream& stream) const
{
	stream << RendererObjectTypeToString(this->objectType);

	if (this->objectType == RENDERER_OBJECT_TYPE_HEIGHT_MAP)
	{
		HeightMap* heightMap = dynamic_cast<HeightMap*>(this->ptr);
		stream << GG_STR(", origin ");
		heightMap->GetRectangle().GetPosition().Serialize(stream);
		stream << GG_STR(", size ");
		heightMap->GetRectangle().GetSize().Serialize(stream);
		stream << GG_STR(", data size ") << FormatFileSize(heightMap->GetRectangle().GetSize().GetTotalLength() * sizeof(Height));
	}
	else if (this->objectType == RENDERER_OBJECT_TYPE_HEIGHT_PROFILE)
	{
		HeightProfile* heightProfile = dynamic_cast<HeightProfile*>(this->ptr);
		stream << GG_STR(", origin ") << heightProfile->GetInterval().GetStart() << GG_STR(", length ") << heightProfile->GetInterval().GetLength()
			<< GG_STR(", data size ") << FormatFileSize((heightProfile->GetInterval().GetLength() * sizeof(Height)));
	}

	stream << " at address 0x" << this->ptr;
}