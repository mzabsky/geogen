#pragma once

#include <string>

namespace geogen 
{
	namespace compiler 
	{
		class FunctionDefinition
		{
		private:
			std::string name;
		public:
			FunctionDefinition(std::string name) { this->name = name; }
			inline std::string GetName() const { return this->name; };
			
		};
	}
}