#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1312_MainMapNotSupportedByScript occurs.
		class MainMapNotSupportedByScriptException : public CompilerException
		{
		public:

			/// Constructor.
			/// @param location The location.
			/// @param sequence The sequence.
			explicit MainMapNotSupportedByScriptException(CodeLocation location) :
				CompilerException(GGE1312_MainMapNotSupportedByScript, location)
			{
			};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Reached end of script without encountering a yield statement yielding the main map on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}