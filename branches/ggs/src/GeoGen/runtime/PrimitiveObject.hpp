#pragma once

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		///	Universal class for most managed object types representing primitive types (Number, String etc.).
		/// @tparam T Primitive native type being represented.
		template<class T>
		class PrimitiveObject : public ManagedObject
		{
		private:
			T value;			
		public:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			/// @param value The wrapped value.
			PrimitiveObject(VirtualMachine* vm, TypeDefinition const* type, T value) : ManagedObject(vm, type), value(value)
			{				
			};

			/// Gets the wrapped value.
			/// @return The wrapped  value.
			inline T GetValue() const { return this->value; }

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << this->GetValue();

				return ss.str();
			}
		};
	}
}