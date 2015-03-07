#pragma once

#include "RenderingStep.hpp"

namespace geogen
{
	namespace renderer
	{
		/// Contains rendering bounds for a RenderingStep.
		class RenderingBounds: public Serializable
		{
		private:
		public:
			/// Gets the type of the rendering step to which this object is associated. Indicates the specific derived type of this instance.
			/// @return The rendering step type.
			virtual RenderingStepType GetRenderingStepType() const = 0;

			/// Gets the number of bytes that will be required to allocate object of size specified by these bounds.
			/// @return The memory size, in bytes.
			virtual unsigned GetMemorySize() const = 0;

			virtual void Serialize(IOStream& stream) const = 0;
		};
	}
}