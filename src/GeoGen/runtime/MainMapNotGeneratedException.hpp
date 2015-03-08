#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2801_MainMapNotGenerated occurs.
		class MainMapNotGeneratedException : public RuntimeException
		{
		private:
		public:
			explicit MainMapNotGeneratedException(CodeLocation location) :
				RuntimeException(GGE2801_MainMapNotGenerated, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Reached end of script without generating the main map on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}