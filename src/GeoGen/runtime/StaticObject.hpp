#pragma once

#include "DynamicObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class StaticObject : public DynamicObject
		{
		public:
			StaticObject(VirtualMachine* vm, TypeDefinition const* type) : DynamicObject(vm, type) {};

			virtual bool IsStaticObject(){ return true; };

			virtual std::string GetStringValue() const
			{
				return "Static";
			};
		};
	}
}