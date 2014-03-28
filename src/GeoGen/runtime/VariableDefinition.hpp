#pragma once

#include <string>

#include "VirtualMachine.hpp"

namespace geogen 
{
	namespace runtime
	{
		class VariableDefinition
		{
		private:
			std::string name;

			bool isConstant;
		protected:
		public:			
			VariableDefinition(std::string const& name, bool isConstant) :
				name(name), isConstant(isConstant)
			{}

			inline std::string const& GetName() const { return this->name; };
			
			inline bool IsConstant() const { return this->isConstant; };

			virtual DynamicObject* CreateDefaultValue() const { return NULL; }

			virtual void Initialize(VirtualMachine& vm) const;
		};
	}
}