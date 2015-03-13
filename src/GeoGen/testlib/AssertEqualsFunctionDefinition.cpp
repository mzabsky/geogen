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

#include "AssertEqualsFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "AssertionFailedException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::testlib;
using namespace geogen::runtime;

ManagedObject* AssertEqualsFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	this->CheckArguments(location, 2, arguments);

	if (!arguments[0]->GetType()->InstanceEqualsTo(arguments[0], arguments[1]))
	{
		throw AssertionFailedException(location, arguments[0]->ToString(), arguments[1]->ToString());
	}

	return vm->GetNull();
}