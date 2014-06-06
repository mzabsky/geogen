#pragma once

#include <string>

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		template<class T>
		class PrimitiveObject : public ManagedObject
		{
		private:
			T value;			
		public:
			PrimitiveObject(VirtualMachine* vm, TypeDefinition const* type, T value) : ManagedObject(vm, type), value(value)
			{				
			};
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