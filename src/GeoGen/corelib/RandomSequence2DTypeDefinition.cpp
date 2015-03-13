/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <memory>

#include "RandomSequence2DTypeDefinition.hpp"
#include "../runtime/RendererObjectStub.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "RandomSequence2DObject.hpp"
#include "RandomSequence2DCreateFunctionDefinition.hpp"
#include "RandomSequence2DAdvanceFunctionDefinition.hpp"
#include "RandomSequence2DGetNumberFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace random;
using namespace std;

RandomSequence2DTypeDefinition::RandomSequence2DTypeDefinition() : TypeDefinition(GG_STR("RandomSequence2D"))
{
	this->GetFunctionDefinitions().AddItem(new RandomSequence2DAdvanceFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new RandomSequence2DGetNumberFunctionDefinition(this));

	this->GetStaticFunctionDefinitions().AddItem(new RandomSequence2DCreateFunctionDefinition(this));
}

ManagedObject* RandomSequence2DTypeDefinition::CreateInstance(VirtualMachine* vm, RandomSeed randomSeed) const
{
	auto_ptr<ManagedObject> object(new RandomSequence2DObject(vm, this, randomSeed));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

runtime::ManagedObject* RandomSequence2DTypeDefinition::Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const
{
	return a;
}