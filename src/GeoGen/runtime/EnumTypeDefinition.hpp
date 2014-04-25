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

			virtual ManagedObject* CreateInstance(VirtualMachine* vm, Number value) const;

			virtual bool InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const;

			virtual ManagedObject* Copy(VirtualMachine* vm, ManagedObject* a) const;
		};
	}
}