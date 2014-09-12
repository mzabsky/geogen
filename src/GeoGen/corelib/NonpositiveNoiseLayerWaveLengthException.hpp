#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class NonpositiveNoiseLayerWaveLengthException : public runtime::RuntimeException
		{
		private:
		public:
			NonpositiveNoiseLayerWaveLengthException(CodeLocation location) :
				RuntimeException(GGE2710_NonpositiveNoiseLayerWaveLength, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Noise layer wave length was less than or equal to zero on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}