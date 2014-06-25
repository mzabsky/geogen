#pragma once

#include "RendererTypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileTypeDefinition : public RendererTypeDefinition
		{
		public:
			HeightProfileTypeDefinition() : RendererTypeDefinition(GG_STR("HeightProfile")) {}

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm) const;
		};
	}
}