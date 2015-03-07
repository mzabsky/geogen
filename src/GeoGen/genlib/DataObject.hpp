#pragma once

#include "../Number.hpp"

namespace geogen
{
	namespace genlib
	{
		/// Base class for data objects. Provides virtual destructor.
		class DataObject
		{
		public:
			/// Destructor.
			virtual ~DataObject() = 0 {};

			/// Gets memory size of this object, in bytes.
			/// @return The memory size.
			virtual unsigned GetMemorySize() const = 0;
		};
	}
}