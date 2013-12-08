#pragma once

#include <string>
#include <ostream>

namespace geogen 
{
	class Serializable
	{
	private:
	public:
		virtual void Serialize(std::iostream& stream) const = 0;
		std::string ToString() const;
	};
}