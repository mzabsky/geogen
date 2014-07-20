#include "../InternalErrorException.hpp"
#include "YieldRenderingStep.hpp"
#include "../renderer/RenderingBounds2D.hpp"
#include "../renderer/Renderer.hpp"
#include "../Rectangle.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace renderer;

void YieldRenderingStep::Step(renderer::Renderer* renderer) const
{
	// TODO: Return the map.
}

void YieldRenderingStep::UpdateRenderingBounds(Renderer* renderer, vector<RenderingBounds*> argumentBounds) const
{
	this->SetRenderingBounds(renderer, this->rect);
}