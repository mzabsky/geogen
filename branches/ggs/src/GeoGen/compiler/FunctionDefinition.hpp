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

			inline std::string GetName() const { return this->name; };
			
		};
	}
}