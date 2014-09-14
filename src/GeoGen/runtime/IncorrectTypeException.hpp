#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2101_IncorrectNativeFunctionArgumentType, geogen::GGE2102_IncorrectOperandType, geogen::GGE2104_IncorrectConditionResultType, geogen::GGE2708_IncorrectNoiseLayerWaveLengthType and geogen::GGE2709_IncorrectNoiseLayerAmplitudeType occurs.
		class IncorrectTypeException : public RuntimeException
		{
		private:
			String expectedTypeName;
			String actualTypeName;
		public:

			/// Constructor.
			/// @param code The error code.
			/// @param location The code location.
			/// @param expectedTypeName Name of the expected type.
			/// @param actualTypeName Name of the actual type.
			explicit IncorrectTypeException(ErrorCode code, CodeLocation location, String const& expectedTypeName, String const& actualTypeName) :
				RuntimeException(code, location), expectedTypeName(expectedTypeName), actualTypeName(actualTypeName) {};

			/// Gets expected type name.
			/// @return The expected type name.
			inline String GetExpectedTypeName() const { return this->expectedTypeName; }

			/// Gets actual type name.
			/// @return The actual type name.
			inline String GetActualTypeName() const { return this->actualTypeName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Object of type \"" << this->GetExpectedTypeName() << "\" expected, got \"" << this->GetActualTypeName() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}