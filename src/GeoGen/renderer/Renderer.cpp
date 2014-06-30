#include "Renderer.hpp"
#include "../InternalErrorException.hpp"

using namespace geogen;
using namespace renderer;

Renderer::Renderer(RenderingSequence& renderingSequence)
: renderingSequence(renderingSequence), objectTable(renderingSequence.GetRequiredObjectTableSize()), status(RENDERER_STATUS_READY), renderingSequenceMetadata(renderingSequence)
{
}

void Renderer::Run()
{
	while (this->status == RENDERER_STATUS_READY)
	{
		this->Step();
	}
}