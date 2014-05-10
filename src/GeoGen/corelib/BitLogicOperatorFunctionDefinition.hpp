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
		class BitLogicOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			enum Operator
			{
				BIT_AND,
				BIT_OR,
				BIT_XOR,
				GREATER_THAN_OR_EQUAL_TO
			};
		private:
			typedef int(*Function)(CodeLocation location, int, int);

			static int CallOperatorBitAnd(CodeLocation location, int, int);
			static int CallOperatorBitOr(CodeLocation location, int, int);
			static int CallOperatorBitXor(CodeLocation location, int, int);

			Function function;

			BitLogicOperatorFunctionDefinition(std::string const& name, Function function) : GlobalNativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static BitLogicOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}