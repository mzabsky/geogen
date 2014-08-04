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

	stream << GG_STR(")") << endl;
}