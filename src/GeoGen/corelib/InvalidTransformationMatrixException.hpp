#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class InvalidTransformationMatrixException : public runtime::RuntimeException
		{
		private:
		public:
			InvalidTransformationMatrixException(CodeLocation location) :
				RuntimeException(GGE2712_InvalidTransformationMatrix, location) {};

			virtual ~InvalidTransformationMatrixException() throw () {}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << GG_STR("Transformation matrix was invalid on line ") << this->GetLocation().GetLine() << GG_STR(", column ") << this->GetLocation().GetColumn() << GG_STR(". The transformation matrix must be invertible.");
				return ss.str();
			}
		};
	}
}