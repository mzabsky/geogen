#pragma once

#include <string>
#include <vector>

#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/NativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class LogicalOperatorFunctionDefinition : public runtime::NativeFunctionDefinition
		{
		public:
			enum Operator
			{
				LOGICAL_AND,
				LOGICAL_OR
			};
		private:
			typedef bool(*Function)(CodeLocation location, bool, bool);

			static bool CallOperatorLogicalAnd(CodeLocation location, bool, bool);
			static bool CallOperatorLogicalOr(CodeLocation location, bool, bool);

			Function function;

			LogicalOperatorFunctionDefinition(std::string const& name, Function function) : NativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static LogicalOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::DynamicObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::DynamicObject*> arguments) const;
		};
	}
}