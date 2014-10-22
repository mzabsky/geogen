#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class InvalidPersistenceException : public runtime::RuntimeException
		{
		private:
		public:
			InvalidPersistenceException(CodeLocation location) :
				RuntimeException(GGE2713_InvalidPersistence, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Persistence was invalid on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ". The peristence must be greater than or equal to 0.10 and less than or equal to 0.9.";
				return ss.str();
			}
		};
	}
}