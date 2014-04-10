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
		class UnaryArithmeticOperatorFunctionDefinition : public runtime::NativeFunctionDefinition
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

			UnaryArithmeticOperatorFunctionDefinition(std::string const& name, Function function) : NativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static UnaryArithmeticOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::DynamicObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::DynamicObject*> arguments) const;
		};
	}
}