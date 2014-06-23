#include "../InternalErrorException.hpp"
#include "RenderingStep1D.hpp"

using namespace geogen;
using namespace renderer;

void RenderingStep::TriggerRenderingBoundsCalculationError(String message) const
{
	StringStream ss;
	ss << "Could not update rendering bounds (" << message << ") in rendering step " << this->GetName() << " based on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ". This step may require proper custom rendering bounds calculation implementation.";

	throw InternalErrorException(ss.str());
}