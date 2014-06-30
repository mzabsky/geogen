#pragma once

#include "../Rectangle.hpp"
#include "RenderingStep.hpp"
#include "RenderingBounds.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingBounds2D : public RenderingBounds
		{
		private:
			Rectangle rectangle;
		public:
			RenderingBounds2D(Rectangle rectangle) : rectangle(rectangle) {}

			inline Rectangle GetRectangle() const { return this->rectangle; }
			inline void GetRectangle(Rectangle rectangle) { this->rectangle = rectangle; }

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_2D; };
		};
	}
}