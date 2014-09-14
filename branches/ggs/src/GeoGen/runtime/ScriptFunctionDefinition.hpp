#pragma once

#include <vector>

#include "FunctionDefinition.hpp"
#include "VariableDefinition.hpp"
#include "SymbolDefinitionTable.hpp"
#include "CodeBlock.hpp"
#include "../CodeLocation.hpp"

namespace geogen 
{
	namespace runtime
	{
		/// Definition of a function declared by the script.
		class ScriptFunctionDefinition : public FunctionDefinition
		{
		private:
			int parameterCount;
			SymbolDefinitionTable<VariableDefinition> localVariableDefinitions;
			CodeBlock rootCodeBlock;
			CodeLocation location;
		public:

			/// Constructor.
			/// @param name The function name.
			/// @param location The code location.
			/// @param parameterCount Number of parameters.
			ScriptFunctionDefinition(String const& name, CodeLocation location, int parameterCount) 
				: FunctionDefinition(name), location(location), parameterCount(parameterCount)
			{
			}

			/// Gets the location.
			/// @return The location.
			inline CodeLocation GetLocation() const { return this->location; }

			/// Gets parameter count.
			/// @return The parameter count.
			inline int GetParameterCount() const { return this->parameterCount; }

			/// Gets the function's root code block.
			/// @return The root code block.
			inline CodeBlock& GetRootCodeBlock() { return this->rootCodeBlock; }
			
			/// Gets the function's root code block.
			/// @return The root code block.
			inline CodeBlock const& GetRootCodeBlock() const { return this->rootCodeBlock; }

			virtual void Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}