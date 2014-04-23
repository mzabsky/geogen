#pragma once

#include <string>

#include "DynamicObject.hpp"
#include "EnumTypeDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		template<class T>
		class PrimitiveObject : public DynamicObject
		{
		private:
			T value;			
		public:
			PrimitiveObject(VirtualMachine* vm, TypeDefinition const* type, T value) : DynamicObject(vm, type), value(value)
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