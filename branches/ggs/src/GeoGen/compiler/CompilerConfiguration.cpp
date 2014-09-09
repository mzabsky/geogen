#include "CompilerConfiguration.hpp"

using namespace std;
using namespace geogen;
using namespace compiler;

void CompilerConfiguration::Serialize(IOStream& stream) const
{
	stream << "MainMapIsMandatory: " << this->MainMapIsMandatory << endl;
}