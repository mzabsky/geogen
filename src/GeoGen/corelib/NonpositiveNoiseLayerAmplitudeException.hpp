#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class NonpositiveNoiseLayerAmplitudeException : public runtime::RuntimeException
		{
		private:
		public:
			NonpositiveNoiseLayerAmplitudeException(CodeLocation location) :
				RuntimeException(GGE2711_NonpositiveNoiseLayerAmplitude, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Noise layer amplitude was less than or equal to zero on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}