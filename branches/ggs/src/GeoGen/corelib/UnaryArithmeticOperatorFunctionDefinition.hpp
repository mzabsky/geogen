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
		class UnaryArithmeticOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			enum Operator
			{
				UNARY_PLUS,
				UNARY_MINUS
			};
		private:
			typedef Number(*Function)(CodeLocation location, Number);

			static Number CallOperatorUnaryPlus(CodeLocation location, Number);
			static Number CallOperatorUnaryMinus(CodeLocation location, Number);

			Function function;

			UnaryArithmeticOperatorFunctionDefinition(std::string const& name, Function function) : GlobalNativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static UnaryArithmeticOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}