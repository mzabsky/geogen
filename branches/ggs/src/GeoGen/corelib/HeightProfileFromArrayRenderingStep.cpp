#include "HeightProfileFromArrayRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;
using namespace std;

void HeightProfileFromArrayRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* profile = new HeightProfile(dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetInterval(), 0, renderer->GetRenderingSequence().GetRenderScale());
	profile->FromArray(this->heights);
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_PROFILE, profile);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

unsigned HeightProfileFromArrayRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetMemorySize();
}

void HeightProfileFromArrayRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << GG_STR("{");
	for (map<Coordinate, Height>::const_iterator it = this->heights.begin(); it != this->heights.end(); it++)
	{
		stream << GG_STR("[") << it->first << GG_STR(", ") << it->second << GG_STR("], ");
	}
	stream << GG_STR("}");
}