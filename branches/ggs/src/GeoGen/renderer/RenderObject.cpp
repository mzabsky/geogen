#include "RendererObject.hpp"
#include "../InternalErrorException.hpp"

using namespace geogen;
using namespace renderer;

String geogen::renderer::RenderObjectTypeToString(RendererObjectType objectType)
{
	switch (objectType)
	{
	case RENDERER_OBJECT_TYPE_HEIGHT_MAP:
		return GG_STR("HeightMap");
	case RENDERER_OBJECT_TYPE_HEIGHT_PROFILE:
		return GG_STR("HeightProfile");
	default:
		throw InternalErrorException("Invalid renderer object type.");
	}
}