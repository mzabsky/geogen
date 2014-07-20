#pragma once

#include "../runtime/TypeDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class TypeDefinition;
	}

	namespace corelib
	{
		/// <summary> Base type for objects that represent <see cref="RendererObject"/>s during execution of the <see cref="VirtualMachine"/>. </summary>
		class RendererTypeDefinition : public runtime::TypeDefinition
		{
		public:

			/// <summary> Initializes a new instance of the RendererTypeDefinition class. </summary>
			/// <param name="name"> The type name. </param>
			RendererTypeDefinition(String const& name) : TypeDefinition(name) {}

			/// <summary> Creates a new instance of this type. </summary>
			/// <param name="vm"> The virtual machine. </param>
			/// <returns> New instance of this type. This instance will be already registered with the <see cref="MemoryManager"/>.</returns>
			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}