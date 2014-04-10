#pragma once

#include <string>
#include <vector>

#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/NativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;
	}

	namespace corelib
	{        
		class EqualityOperatorFunctionDefinition : public runtime::NativeFunctionDefinition
		{
		public:
			enum Operator
			{
				EQUAL_TO,
				NOT_EQUAL_TO
			};
		private:
			typedef bool(*Function)(CodeLocation location, runtime::DynamicObject*, runtime::DynamicObject*);

			static bool CallOperatorEqualTo(CodeLocation location, runtime::DynamicObject*, runtime::DynamicObject*);
			static bool CallOperatorNotEqualTo(CodeLocation location, runtime::DynamicObject*, runtime::DynamicObject*);

			Function function;

			EqualityOperatorFunctionDefinition(std::string const& name, Function function) : NativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static EqualityOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::DynamicObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::DynamicObject*> arguments) const;
		};
	}
}