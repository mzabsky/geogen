#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when errors geogen::GGE1402_ScriptParametersNotKeyValueCollection, geogen::GGE1403_ScriptParameterNotKeyValueCollection, geogen::GGE1408_WidthNotKeyValueCollection and geogen::GGE1409_HeightNotKeyValueCollection occur.
		class IncorrectMetadataValueTypeException : public CompilerException
		{
		private:
			String valueName;
			String expectedType;
			String actualType;
		public:

			/// Constructor.
			/// @param errorCode The error code.
			/// @param location The location.
			/// @param valueName The value name.
			/// @param expectedType The expected type.
			/// @param actualType The actual type.
			IncorrectMetadataValueTypeException(ErrorCode errorCode, CodeLocation location, String const& valueName, String const& expectedType, String const& actualType) :
				CompilerException(errorCode, location), valueName(valueName), expectedType(expectedType), actualType(actualType) {};

			/// Gets value name.
			/// @return The value name.
			inline String GetValueName() const { return this->valueName; }

			/// Gets expected type.
			/// @return The expected type.
			inline String GetExpectedType() const { return this->expectedType; }

			/// Gets actual type.
			/// @return The actual type.
			inline String GetActualType() const { return this->actualType; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Metadata value \"" << valueName << "\" expected to be of type \"" << expectedType << "\", had \"" << actualType << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}