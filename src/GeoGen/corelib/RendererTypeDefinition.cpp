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

#include "RendererTypeDefinition.hpp"
#include "../runtime/RendererObjectStub.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "../runtime/VirtualMachine.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

ManagedObject* RendererTypeDefinition::CreateInstance(VirtualMachine* vm) const
{
	auto_ptr<ManagedObject> object(new RendererObjectStub(vm, this));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

runtime::ManagedObject* RendererTypeDefinition::Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const
{
	return a;
}