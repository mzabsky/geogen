#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileFlatStep : public renderer::RenderingStep1D
		{
		private:
			Height height;
		public:
			HeightProfileFlatStep(CodeLocation location, Height height) 
				: RenderingStep1D(location), height(height) {};
			
			virtual String GetName() const { return GG_STR("HeightProfile.Flat"); };

			virtual void Step(renderer::Renderer* renderer) const = 0;
		};
	}
}