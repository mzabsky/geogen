#pragma once

#include <vector>

#include "../CodeLocation.hpp"
#include "../String.hpp"

#include "ManagedObjectHolder.hpp"

namespace geogen 
{
	/// Kind of a function - operator vs. function.
	enum FunctionType
	{
		/// Function. 
		FUNCTION_TYPE_FUNCTION,

        /// Operator.
        FUNCTION_TYPE_OPERATOR
	};

	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;
		class TypeDefinition;

		/// Base class for function definitions.
		class FunctionDefinition : public Serializable
		{
		private:
			String name;
		protected:
			/// Constructor.
			/// @param name The function name.			
			FunctionDefinition(String const& name) { this->name = name; }
		public:
			/// Destructor.
			virtual ~FunctionDefinition() {};

			/// Gets the function name.
			/// @return The function name.
			inline String const& GetName() const { return this->name; };

			/// Gets type of the function. This mostly determines errors which are thrown on type check failures.
			/// @return The function type.
			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_FUNCTION; }

			/// Method invoked when the function is called from a script.
			/// @param location The code location of the call.
			/// @param vm The virtual machine.
			/// @param instance Instance on which the function is being called, if it is being called as a member method.
			/// @param numberOfArguments Number of arguments being actually passed.
			virtual void Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const = 0;

			/// Helper method which performs a type check and type conversion (if required) on a single input argument. Throws exception in case of any errors.
			/// @param vm The virtual machine.
			/// @param location The code location of the call.
			/// @param expectedType Definition of the expected type.
			/// @param actualArgument The actual argument object. This will be replaced with a result of an conversion, if it is performed.
			/// @param managedObjectHolder Holder of an conversion result object, if it was performed. Otherwise the holder remains empty.
			/// @return true if an conversion was performed, false otherwise.
			bool CheckArgument(VirtualMachine* vm, CodeLocation location, TypeDefinition const* expectedType, ManagedObject*& actualArgument, ManagedObjectHolder& managedObjectHolder) const;

			/// Helper method which performs type checks and type conversions (if required) on input arguments. Throws exception in case of any errors.
			/// @param vm If non-null, the virtual memory.
			/// @param location The code location.
			/// @param expectedTypes The list of definitions of expected parameters.
			/// @param actualArguments The actual argument objects. If any conversions are performed, the appropriate object in the vector is replaced with result of the conversion.
			/// @param requiredArgumentCount Number of required arguments, if -1 all arguments are
			/// required.
			/// @return Vector of holders of any objects that produced in conversions.
			std::vector<ManagedObjectHolder> CheckArguments(VirtualMachine* vm, CodeLocation location, std::vector<TypeDefinition const*> expectedTypes, std::vector<ManagedObject*>& actualArguments, int requiredArgumentCount = -1) const;

			/// Check a count of actual arguments against a list of expected parameters. If these don't match, an exception is thrown.
			/// @param location The code location.
			/// @param expectedArgumentCount The expected number of arguments.
			/// @param actualArguments The actual arguments.
			void CheckArguments(CodeLocation location, unsigned expectedArgumentCount, std::vector<ManagedObject*> actualArguments) const;

			virtual void Serialize(IOStream& stream) const = 0;
		};
	}
}