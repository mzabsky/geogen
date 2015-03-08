#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"
#include "../genlib/TransformationMatrix.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapTransformRenderingStep : public renderer::RenderingStep2D
		{
		private:
			genlib::TransformationMatrix matrix;
		public:
			HeightMapTransformRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, genlib::TransformationMatrix const& matrix)
				: RenderingStep2D(location, argumentSlots, returnSlot), matrix(matrix) {};

			virtual String GetName() const { return GG_STR("HeightMap.Transform"); };

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual void Step(renderer::Renderer* renderer) const;

			virtual unsigned GetPeakExtraMemory(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds const*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}