#pragma once

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		/// <summary> A stub representing an <see cref="RendererObject"/> during execution of an <see cref="VirtualMachine"/>. This object doesn't carry any value on its own, it is uniquely defined by its address. This address can then be converted into an renderer object slot number using the <see cref="RendererObjectSlotTable"/>.</summary>
		class RendererObjectStub : public ManagedObject
		{
		public:

			/// <summary> Initializes a new instance of the RendererObjectStub class. </summary>
			/// <param name="vm"> The virtual machine. </param>
			/// <param name="type"> The type definition. </param>
			RendererObjectStub(VirtualMachine* vm, TypeDefinition const* type) : ManagedObject(vm, type)
			{
			};

			/// <summary> Gets the value of this object as a string. </summary>
			/// <returns> The string value. </returns>
			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << this;

				return ss.str();
			}
		};
	}
}