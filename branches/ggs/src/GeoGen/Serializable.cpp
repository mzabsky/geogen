#include "Serializable.hpp"

namespace geogen 
{
	String Serializable::ToString() const
	{
		StringStream stream;
		this->Serialize(stream);
		return stream.str();
	}
}