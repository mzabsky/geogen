#include "CoreLibrary.hpp"
#include "NumberTypeDefinition.hpp"
#include "BooleanTypeDefinition.hpp"
#include "NullTypeDefinition.hpp"

using namespace geogen::runtime;

CoreLibrary::CoreLibrary()
{
	// Core types
	this->typeDefinitions.AddItem(new NullTypeDefinition());
	this->typeDefinitions.AddItem(new BooleanTypeDefinition());
	this->typeDefinitions.AddItem(new NumberTypeDefinition());
	//this->typeDefinitions.AddItem(new StringTypeDefinition());
}