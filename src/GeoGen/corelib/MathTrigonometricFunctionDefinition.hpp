#pragma once

#include <vector>

#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
	}

	namespace corelib
	{
		class MathTrigonometricFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			enum Function
			{
				SIN,
				COS,
				TAN,
				ASIN,
				ACOS,
				ATAN,
				DEG_TO_RAD,
				RAD_TO_DEG
			};
		private:
			Function function;

			MathTrigonometricFunctionDefinition(String const& name, Function function) : GlobalNativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static MathTrigonometricFunctionDefinition* Create(Function op);

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}