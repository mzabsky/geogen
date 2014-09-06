#pragma once

#include "../CodeLocation.hpp"
#include "../String.hpp"

#include "ManagedObjectHolder.hpp"

#include <vector>

namespace geogen 
{
	enum FunctionType
	{
		FUNCTION_TYPE_FUNCTION,
        FUNCTION_TYPE_OPERATOR
	};

	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;
		class TypeDefinition;

		class FunctionDefinition : public Serializable
		{
		private:
			String name;
		public:
			FunctionDefinition(String const& name) { this->name = name; }
			virtual ~FunctionDefinition() {};

			inline String const& GetName() const { return this->name; };

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_FUNCTION; }

			virtual void Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const = 0;

			bool CheckArgument(VirtualMachine* vm, CodeLocation location, TypeDefinition const* expectedType, ManagedObject*& actualArgument, ManagedObjectHolder& managedObjectHolder) const;

			/// <summary>
			/// Check a list of actual arguments against a list of expected parameters. If these don't match,
			/// an exception is thrown.
			/// </summary>
			/// <param name="location"> The code location. </param>
			/// <param name="expectedTypes"> The list of expected parameters. </param>
			/// <param name="actualArguments"> The actual arguments. </param>
			/// <param name="requiredArgumentCount"> Number of required arguments, if -1 all arguments are required. </param>
			/// <exception cref="IncorrectTypeException"> Thrown when the two lists don't match. </exception>
			std::vector<ManagedObjectHolder> CheckArguments(VirtualMachine* vm, CodeLocation location, std::vector<TypeDefinition const*> expectedTypes, std::vector<ManagedObject*>& actualArguments, int requiredArgumentCount = -1) const;
			
			/// <summary> Check a count of actual arguments against a list of expected parameters. If these don't match, an exception is thrown. </summary>
			/// <exception cref="IncorrectTypeException"> Thrown when the list sizes don't match. </exception>
			/// <param name="location"> The code location. </param>
			/// <param name="expectedArgumentCount"> The list of expected parameters. </param>
			/// <param name="actualArguments"> The actual arguments. </param>			
			void CheckArguments(CodeLocation location, unsigned expectedArgumentCount, std::vector<ManagedObject*> actualArguments) const;

			virtual void Serialize(IOStream& stream) const = 0;
		};
	}
}