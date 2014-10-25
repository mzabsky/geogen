#pragma once

#include "../Point.hpp"
#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{
	namespace corelib
	{
		class RandomSequence2DTypeDefinition : public runtime::TypeDefinition
		{
		public:
			RandomSequence2DTypeDefinition();

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, random::RandomSeed randomSeed) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}