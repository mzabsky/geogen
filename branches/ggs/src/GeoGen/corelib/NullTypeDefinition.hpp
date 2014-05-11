#pragma once

#include "../runtime/TypeDefinition.hpp"
#include "../runtime/PrimitiveObject.hpp"
#include "../InternalErrorException.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
	}

	namespace corelib
	{
		typedef runtime::PrimitiveObject<Number> NumberObject;

		class NullTypeDefinition : public runtime::TypeDefinition
		{
		public:
			NullTypeDefinition() : TypeDefinition("null") {}

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const
			{
				return a;
			};
		};
	}
}