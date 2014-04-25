#pragma once

#include <string>

#include "ManagedObject.hpp"
#include "EnumTypeDefinition.hpp"

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

			virtual std::string GetStringValue() const
			{
				std::stringstream ss;
				ss << this->GetValue();

				return ss.str();
			}
		};
	}
}