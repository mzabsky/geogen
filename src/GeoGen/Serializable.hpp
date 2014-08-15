#pragma once

#include "String.hpp"

namespace geogen 
{
	/// <summary> Base class for objects that can be serialized into a human readable string representation. </summary>
	class Serializable
	{
	private:
	public:

		/// <summary> Writes the human readable string representation of this instance to the given stream. </summary>
		/// <param name="stream"> The stream. </param>
		virtual void Serialize(IOStream& stream) const = 0;

		virtual void SerializeWithTabs(IOStream& stream, unsigned tabs) const;

		/// <summary> Convert this instance into a human readable string representation. </summary>
		/// <returns> A string that represents this instance. </returns>
		String ToString() const;
	};
}