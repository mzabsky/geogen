#pragma once

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Stub representing an renderer::RendererObject during execution of an
		/// runtime::VirtualMachine. This object doesn't carry any value on its own, it is uniquely
		/// defined by its address. This address can then be converted into an renderer object slot
		/// number using the renderer::RendererObjectSlotTable.
		class RendererObjectStub : public ManagedObject
		{
		public:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			RendererObjectStub(VirtualMachine* vm, TypeDefinition const* type) : ManagedObject(vm, type)
			{
			};

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << this;

				return ss.str();
			}
		};
	}
}