#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"
#include "../Orientation.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapProjectProfileRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Orientation orientation;
		public:
			HeightMapProjectProfileRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Orientation orientation)
				: RenderingStep2D(location, argumentSlots, returnSlot), orientation(orientation) {};

			virtual String GetName() const { return GG_STR("HeightMap.ProjectProfile"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}