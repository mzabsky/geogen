#pragma once

#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"
#include "../runtime/ScriptParameters.hpp"

namespace geogen
{
	namespace corelib
	{
		class ParametersTypeDefinition : public runtime::TypeDefinition
		{
		public:
			typedef std::map<int, String> const ValueDefinitions;
		private:
			ValueDefinitions valueDefinitions;
		public:
			ParametersTypeDefinition(runtime::ScriptParameters const& values);

			virtual void Initialize(runtime::VirtualMachine* vm) const;

			//virtual ManagedObject* CreateInstance(Number value) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}