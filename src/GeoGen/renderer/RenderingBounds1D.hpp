#pragma once

#include "../Interval.hpp"
#include "RenderingStep.hpp"
#include "RenderingBounds.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingBounds1D : public RenderingBounds
		{
		private:
			Interval interval;
		public:
			RenderingBounds1D(Interval interval) : interval(interval) {}

			inline Interval GetInterval() const { return this->interval; }
			inline void SetInterval(Interval interval) { this->interval = interval; }

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_1D; };

			virtual void Serialize(IOStream& stream) const
			{
				this->interval.Serialize(stream);
			}
		};
	}
}