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
		class ManagedObject;
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
			typedef bool(*Function)(CodeLocation location, runtime::ManagedObject*, runtime::ManagedObject*);

			static bool CallOperatorEqualTo(CodeLocation location, runtime::ManagedObject*, runtime::ManagedObject*);
			static bool CallOperatorNotEqualTo(CodeLocation location, runtime::ManagedObject*, runtime::ManagedObject*);

			Function function;

			EqualityOperatorFunctionDefinition(std::string const& name, Function function) : NativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static EqualityOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}