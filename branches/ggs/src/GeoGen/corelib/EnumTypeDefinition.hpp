#pragma once

#include <map>

#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"

namespace geogen
{
	namespace runtime 
	{
		class ManagedObject;
		class VirtualMachine;
	}

	namespace corelib
	{
		class EnumTypeDefinition : public runtime::TypeDefinition
		{
		public:
			typedef std::map<std::string, int> const ValueDefinitions;
		private:
			ValueDefinitions valueDefinitions;
		public:
			EnumTypeDefinition(std::string const& name, ValueDefinitions values);

			virtual void Initialize(runtime::VirtualMachine* vm) const;

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, Number value) const;

			virtual bool IsEnumType() const { return true; };

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;

			inline ValueDefinitions const& GetValueDefinitions() const { return this->valueDefinitions; }

			runtime::ManagedObject* GetValueByInt(runtime::VirtualMachine* vm, int intValue) const;
			bool IsValueIntDefined(int intValue) const;
			int GetDefaultValueInt() const;
			bool IsValueStringDefined(std::string stringValue) const;
		};
	}
}