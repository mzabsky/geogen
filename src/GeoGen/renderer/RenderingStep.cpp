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

#include "../InternalErrorException.hpp"
#include "RenderingStep1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace std;

void RenderingStep::TriggerRenderingBoundsCalculationError(String message) const
{
	StringStream ss;
	ss << "Could not update rendering bounds (" << message << ") in rendering step " << this->GetName() << " based on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ". This step may require proper custom rendering bounds calculation implementation.";

	throw InternalErrorException(ss.str());
}

unsigned RenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return 0;
}

void RenderingStep::SimulateOnRenderingBounds(RenderingBounds* renderingBounds) const
{
	// Most steps have no effect on the return object's rendering bounds.
}

void RenderingStep::Serialize(IOStream& stream) const
{
	stream << this->GetReturnSlot() << GG_STR(" <- ") << this->GetName() << GG_STR(" [");
	
	for (std::vector<unsigned>::const_iterator it = this->GetArgumentSlots().begin(); it != this->GetArgumentSlots().end(); it++)
	{
		stream << *it;

		if (it + 1 != this->GetArgumentSlots().end())
		{
			stream << GG_STR(", ");
		}
	}

	stream << GG_STR("] (");

	this->SerializeArguments(stream);

	stream << GG_STR(")");
}