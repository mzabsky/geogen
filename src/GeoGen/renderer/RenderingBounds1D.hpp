#pragma once

#include "../Number.hpp"
#include "../Interval.hpp"
#include "RenderingStep.hpp"
#include "RenderingBounds.hpp"
#include "../genlib/HeightProfile.hpp"

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
			inline void CombineInterval(Interval interval) { this->interval = Interval::Combine(interval, this->interval); }

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_1D; };

			virtual unsigned GetMemorySize(Scale scale) const { return genlib::HeightProfile::GetMemorySize(this->interval, scale); };

			virtual void Serialize(IOStream& stream) const
			{
				this->interval.Serialize(stream);
			}
		};
	}
}