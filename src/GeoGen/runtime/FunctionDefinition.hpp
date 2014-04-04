#pragma once

#include <string>

namespace geogen 
{
	namespace runtime
	{
		class VirtualMachine;

		class FunctionDefinition
		{
		private:
			std::string name;
		public:
			FunctionDefinition(std::string const& name) { this->name = name; }
			inline std::string const& GetName() const { return this->name; };

			void Call(VirtualMachine* vm) const {};
		};
	}
}