#pragma once

#include "DynamicObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class StaticObject : public DynamicObject
		{
		public:
			StaticObject(TypeDefinition const* type) : DynamicObject(type) {};

			virtual bool IsStaticObject(){ return true; };

			virtual void Serialize(std::iostream& stream) const
			{
				stream << "Static[" << this->GetType()->GetName() << "]";
			};
		};
	}
}