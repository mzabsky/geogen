#pragma once

#include "../Number.hpp"
#include "TypeDefinition.hpp"
#include "ScriptParameters.hpp"

namespace geogen
{
	namespace runtime
	{
		class ParametersTypeDefinition : public TypeDefinition
		{
		public:
			typedef std::map<int, std::string> const ValueDefinitions;
		private:
			ValueDefinitions valueDefinitions;
		public:
			ParametersTypeDefinition(ScriptParameters const& values);

			virtual void Initialize(VirtualMachine* vm) const;

			//virtual ManagedObject* CreateInstance(Number value) const;

			virtual ManagedObject* Copy(VirtualMachine* vm, ManagedObject* a) const;
		};
	}
}