#pragma once

#include "../CodeLocation.hpp"

#include <string>

namespace geogen 
{
	enum FunctionType
	{
		FUNCTION_TYPE_FUNCTION,
        FUNCTION_TYPE_OPERATOR
	};

	namespace runtime
	{
		class VirtualMachine;

		class FunctionDefinition
		{
		private:
			std::string name;
		public:
			FunctionDefinition(std::string const& name) { this->name = name; }
			virtual ~FunctionDefinition() {};

			inline std::string const& GetName() const { return this->name; };

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_FUNCTION; }

			virtual void Call(CodeLocation location, VirtualMachine* vm, unsigned numberOfArguments) const = 0;
		};
	}
}