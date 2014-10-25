#include "TestLibrary.hpp"
#include "AssertEqualsFunctionDefinition.hpp"
using namespace geogen::testlib;

TestLibrary::TestLibrary()
{
	this->globalFunctionDefinitions.AddItem(new AssertEqualsFunctionDefinition());
}