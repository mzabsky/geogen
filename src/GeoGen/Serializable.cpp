#include <sstream>

#include "Serializable.hpp"

namespace geogen 
{
	std::string Serializable::ToString() const
	{
		std::stringstream stream;
		this->Serialize(stream);
		return stream.str();
	}
}