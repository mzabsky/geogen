#pragma once

#include "../Rectangle.hpp"
#include "RenderingStep.hpp"
#include "RenderingBounds.hpp"
#include "../genlib/HeightMap.hpp"

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
			inline void SetRectangle(Rectangle rectangle) { this->rectangle = rectangle; }
			inline void CombineRectangle(Rectangle rectangle) { this->rectangle = Rectangle::Combine(rectangle, this->rectangle); }

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_2D; };

			virtual unsigned GetMemorySize() const { return sizeof(genlib::HeightMap) + sizeof(Height) * this->rectangle.GetSize().GetTotalLength(); };

			virtual void Serialize(IOStream& stream) const
			{
				this->rectangle.Serialize(stream);
			}
		};
	}
}