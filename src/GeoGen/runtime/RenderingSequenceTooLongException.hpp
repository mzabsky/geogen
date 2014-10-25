#pragma once

#include <stdexcept>

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2504_RenderingSequenceTooLong occurs.
		class RenderingSequenceTooLongException : public RuntimeException
		{
		private:
		public:

			/// Constructor.
			/// @param location The code location.
			RenderingSequenceTooLongException(CodeLocation location) :
				RuntimeException(GGE2504_RenderingSequenceTooLong, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Rendering sequence was too long on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			};
		};
	}
}