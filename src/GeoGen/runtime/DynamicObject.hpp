#pragma once

#include <string>
#include <functional>

#include "../compiler/TypeDefinition.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class DynamicObject
		{
		private:
			compiler::TypeDefinition const* type;
		public:
			DynamicObject(compiler::TypeDefinition const* type) { this->type = type; }
			inline compiler::TypeDefinition const* GetType() const { return this->type; };

			inline bool operator<(const DynamicObject* rhs) 
			{
				return this->LessThan(rhs);
			}

			virtual bool LessThan(DynamicObject const* other)
			{
				return this < other;
			}
		};
	}
}