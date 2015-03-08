#pragma once

#include "../String.hpp"
#include "../Serializable.hpp"

namespace geogen 
{
	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;

		/// Base class for definitions of pre-defined variables. These are often not strictly necessary though - any code can insert items straight into an appropriate VariableTable.
		class VariableDefinition : public Serializable
		{
		private:
			String name;

			bool isConstant;
		protected:
		public:			

			/// Constructor.
			/// @param name The variable name.
			/// @param isConstant True if the variable is to be read-only.
			VariableDefinition(String const& name, bool isConstant) :
				name(name), isConstant(isConstant)
			{}

			/// Gets the name.
			/// @return The name.
			inline String const& GetName() const { return this->name; };

			/// Query if the variable is to be read-only.
			/// @return true if the variable is read-only.
			inline bool IsConstant() const { return this->isConstant; };

			/// Creates default value of the variable (called by the default implementation Initialize).
			/// @param vm The virtual machine.
			/// @return The default value.
			virtual ManagedObject* CreateDefaultValue(VirtualMachine* vm) const { return NULL; }

			/// Initializes this variable. Executed when the VirtualMachine is starting up.
			/// @param vm The virtual machine.
			virtual void Initialize(VirtualMachine* vm) const;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}