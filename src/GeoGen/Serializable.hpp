#pragma once

#include "String.hpp"

namespace geogen 
{
	class Serializable
	{
	private:
	public:
		virtual void Serialize(IOStream& stream) const = 0;
		String ToString() const;
	};
}