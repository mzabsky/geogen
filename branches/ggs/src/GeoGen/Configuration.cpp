#include "Configuration.hpp"

using namespace std;
using namespace geogen;

void Configuration::Serialize(IOStream& stream) const
{
	stream << "MainMapIsMandatory: " << this->MainMapIsMandatory << endl;
}