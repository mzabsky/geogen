#pragma once

#include "../String.hpp"

namespace geogen 
{
	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;

		class VariableDefinition
		{
		private:
			String name;

			bool isConstant;
		protected:
		public:			
			VariableDefinition(String const& name, bool isConstant) :
				name(name), isConstant(isConstant)
			{}

			inline String const& GetName() const { return this->name; };
			
			inline bool IsConstant() const { return this->isConstant; };

			virtual ManagedObject* CreateDefaultValue() const { return NULL; }

			virtual void Initialize(VirtualMachine* vm) const;
		};
	}
}