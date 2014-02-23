#pragma once

#include "../Number.hpp"
#include "TypeDefinition.hpp"
#include "VirtualMachine.hpp"
#include "PrimitiveObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class EnumTypeDefinition : public TypeDefinition
		{
		public:
			typedef std::map<int, std::string> const ValueDefinitions;
		private:
			ValueDefinitions valueDefinitions;
		public:
			EnumTypeDefinition(CodeLocation location, std::string const& name, ValueDefinitions values);

			virtual DynamicObject* CreateInstance(Number value) const;

			virtual bool InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const;
			virtual bool InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const;

			virtual DynamicObject* Copy(DynamicObject* a) const;
		};
	}
}