#include "RenderingSequence.hpp"
#include "../InternalErrorException.hpp"
#include "RenderingStep.hpp"

using namespace geogen;
using namespace renderer;

const unsigned RenderingSequence::SIZE_LIMIT = 1000;

RenderingSequence::~RenderingSequence()
{
	for (iterator it = this->steps.begin(); it != this->steps.end(); it++)
	{
		delete *it;
	}
}

bool RenderingSequence::AddStep(RenderingStep* step)
{
	if (this->Size() >= SIZE_LIMIT)
	{
		return false;
	}

	// Validate that the sequence does not reference slots that weren't used yet.
	for (unsigned i = 0; i < step->GetArgumentSlots().size(); i++)
	{
		if (step->GetArgumentSlots()[i] >= this->objectTableSize)
		{
			throw new InternalErrorException("Rendering step references object slot that doesn't exist yet.");
		}
	}

	if (step->GetReturnSlot() == this->objectTableSize)
	{
		// A new slot is being added
		this->objectTableSize++;
	}
	else if (step->GetReturnSlot() > this->objectTableSize)
	{
		throw new InternalErrorException("Rendering step return slot has too high number (previous slots weren't referenced yet)."); 
	}

	this->steps.push_back(step);

	return true;
}

void RenderingSequence::Serialize(IOStream& stream) const
{
	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{
		(*it)->Serialize(stream);
		stream << std::endl;
	}
}