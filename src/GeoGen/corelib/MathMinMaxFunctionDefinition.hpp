#pragma once

#include <vector>

#include "../String.hpp"
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
		class MathMinMaxFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			enum Function
			{
				MIN,
				MAX
			};
		private:
			Function function;

			MathMinMaxFunctionDefinition(String const& name, Function function) : GlobalNativeFunctionDefinition(name), function(function)
			{
			};

			Number Min(std::vector<runtime::ManagedObject*> arguments) const;
			Number Max(std::vector<runtime::ManagedObject*> arguments) const;
		public:
			static MathMinMaxFunctionDefinition* Create(Function op);

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}