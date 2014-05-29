#pragma once

#include <map>

#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		class EnumTypeDefinition : public runtime::TypeDefinition
		{
		public:
			typedef std::map<std::string, int> const ValueDefinitions;
		private:
			ValueDefinitions valueDefinitions;
		public:
			EnumTypeDefinition(CodeLocation location, std::string const& name, ValueDefinitions values);

			virtual void Initialize(runtime::VirtualMachine* vm) const;

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, Number value) const;

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}