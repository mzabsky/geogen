#pragma once

#include <string>
#include <vector>

#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class RelationalOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			enum Operator
			{
				LESS_THAN,
				LESS_THAN_OR_EQUAL_TO,
				GREATER_THAN,
				GREATER_THAN_OR_EQUAL_TO
			};
		private:
			typedef bool(*Function)(CodeLocation location, Number, Number);

			static bool CallOperatorLessThan(CodeLocation location, Number, Number);
			static bool CallOperatorLessThanOrEqualTo(CodeLocation location, Number, Number);
			static bool CallOperatorGreaterThan(CodeLocation location, Number, Number);
			static bool CallOperatorGreaterThanOrEqualTo(CodeLocation location, Number, Number);

			Function function;

			RelationalOperatorFunctionDefinition(std::string const& name, Function function) : GlobalNativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static RelationalOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}