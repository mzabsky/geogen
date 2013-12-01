#pragma once

#include <string>

namespace geogen 
{
	namespace compiler 
	{
		class VariableDefinition
		{
		private:
			std::string name;

			bool isConstant;
		public:

			inline std::string const& GetName() const { return this->name; };
			
			inline bool IsConstant() const { return this->isConstant; };
		};
	}
}