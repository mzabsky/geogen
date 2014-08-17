#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapAddRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Height addend;
		public:
			HeightMapAddRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height addend)
				: RenderingStep2D(location, argumentSlots, returnSlot), addend(addend) {};

			virtual String GetName() const { return GG_STR("HeightMap.Add"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}