#pragma once

#include <string>

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
		};
	}
}