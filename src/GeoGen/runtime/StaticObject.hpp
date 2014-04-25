#pragma once

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class StaticObject : public ManagedObject
		{
		public:
			StaticObject(VirtualMachine* vm, TypeDefinition const* type) : ManagedObject(vm, type) {};

			virtual bool IsStaticObject(){ return true; };

			virtual std::string GetStringValue() const
			{
				return "Static";
			};
		};
	}
}