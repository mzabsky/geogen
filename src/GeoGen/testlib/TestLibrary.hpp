#pragma once

#include "../runtime/Library.hpp"

namespace geogen
{
	namespace testlib
	{
		/// Library containing additional functions for writing test scripts. This library is not loaded into geogen::runtime::CompiledScript by default.
		class TestLibrary : public runtime::Library
		{
		public:
			/// Default constructor.
			TestLibrary();
		};
	}
}