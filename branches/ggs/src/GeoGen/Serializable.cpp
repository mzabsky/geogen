#include "Serializable.hpp"

using namespace geogen;
using namespace std;

String Serializable::ToString() const
{
	StringStream stream;
	this->Serialize(stream);
	return stream.str();
}

void Serializable::SerializeWithTabs(IOStream& stream, unsigned tabs) const
{
	StringStream ss;
	this->Serialize(ss);
	String line;
	while (getline(ss, line))
	{
		stream << String(tabs, '\t') << line << endl;
	}
}
