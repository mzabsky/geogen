#pragma once

#include <string>
#include <vector>

#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class LogicalOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
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

			LogicalOperatorFunctionDefinition(std::string const& name, Function function) : GlobalNativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static LogicalOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}