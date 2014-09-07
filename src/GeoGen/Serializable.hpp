#pragma once

#include "String.hpp"

namespace geogen 
{
	/// Base class for objects that can be serialized into a human readable string representation.
	class Serializable
	{
	private:
	public:

		/// Writes the human readable string representation of this instance to the given stream.
		/// @param stream The stream.
		virtual void Serialize(IOStream& stream) const = 0;

		/// Serialize with tabs.
		/// @param stream The stream.
		/// @param	tabs The tabs.
		virtual void SerializeWithTabs(IOStream& stream, unsigned tabs) const;

		/// Convert this instance into a human readable string representation.
		/// @return A string that represents this instance.
		String ToString() const;
	};
}