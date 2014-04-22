#pragma once

#include "../Number.hpp"
#include "TypeDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		class EnumTypeDefinition : public TypeDefinition
		{
		public:
			typedef std::map<int, std::string> const ValueDefinitions;
		private:
			ValueDefinitions valueDefinitions;
		public:
			EnumTypeDefinition(CodeLocation location, std::string const& name, ValueDefinitions values);

			virtual DynamicObject* CreateInstance(VirtualMachine* vm, Number value) const;

			virtual bool InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const;
			virtual bool InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const;

			virtual DynamicObject* Copy(VirtualMachine* vm, DynamicObject* a) const;
		};
	}
}