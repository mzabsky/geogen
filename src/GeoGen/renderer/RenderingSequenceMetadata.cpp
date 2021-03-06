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

#include "RenderingSequenceMetadata.hpp"
#include "RenderingStep.hpp"
#include "RenderingBounds1D.hpp"
#include "RenderingBounds2D.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

RenderingSequenceMetadata::RenderingSequenceMetadata(RenderingSequence const& renderingSequence)
{
	unsigned stepNumber = 0;
	for (RenderingSequence::const_iterator it = renderingSequence.Begin(); it != renderingSequence.End(); it++)
	{
		this->stepNumbers.insert(pair<RenderingStep const*, unsigned>(*it, stepNumber));

		RenderingStepType stepType = (*it)->GetRenderingStepType();
		switch (stepType)
		{
		case RENDERING_STEP_TYPE_1D:
			this->renderingBounds.push_back(new RenderingBounds1D(Interval()));
			break;
		case RENDERING_STEP_TYPE_2D:
			this->renderingBounds.push_back(new RenderingBounds2D(Rectangle()));
			break;
		default:
			throw InternalErrorException(GG_STR("Invalid rendering step type."));
		}

		this->objectsIndexesToRelease.push_back(vector<unsigned>());
		this->memoryRequirements.push_back(1);

		stepNumber++;
	}
}

RenderingSequenceMetadata::~RenderingSequenceMetadata()
{
	for (vector<RenderingBounds*>::iterator it = this->renderingBounds.begin(); it != this->renderingBounds.end(); it++)
	{
		delete *it;
	}
}

unsigned RenderingSequenceMetadata::GetStepNumberByAddress(RenderingStep const* step) const
{
	map<RenderingStep const*, unsigned>::const_iterator it = this->stepNumbers.find(step);

	if (it != this->stepNumbers.end())
	{
		return it->second;
	}
	else 
	{
		throw InternalErrorException(GG_STR("Step not present in the sequence."));
	}
}

RenderingBounds* RenderingSequenceMetadata::GetRenderingBounds(RenderingStep const* step)
{
	return this->renderingBounds[this->GetStepNumberByAddress(step)];
}

std::vector<unsigned>& RenderingSequenceMetadata::GetObjectIndexesToRelease(RenderingStep const* step)
{
	return this->objectsIndexesToRelease[this->GetStepNumberByAddress(step)];
}

std::vector<unsigned> const& RenderingSequenceMetadata::GetObjectIndexesToRelease(RenderingStep const* step) const
{
	return this->objectsIndexesToRelease[this->GetStepNumberByAddress(step)];
}

unsigned RenderingSequenceMetadata::GetMemoryRequirement(RenderingStep const* step) const
{
	return this->memoryRequirements[this->GetStepNumberByAddress(step)];
}

void RenderingSequenceMetadata::SetMemoryRequirement(RenderingStep const* step, unsigned memory)
{
	this->memoryRequirements[this->GetStepNumberByAddress(step)] = memory;
}

void RenderingSequenceMetadata::Serialize(IOStream& stream) const
{
	for (std::map<RenderingStep const*, unsigned>::const_iterator it = this->stepNumbers.begin(); it != this->stepNumbers.end(); it++)
	{
		stream << it->second << GG_STR("# ");
		//it->first->Serialize(stream);
		stream << GG_STR(": bounds ");
		this->renderingBounds[it->second]->Serialize(stream);
		stream << GG_STR(",memory ");
		this->memoryRequirements[it->second];
		stream << GG_STR(", objects to release ");

		for (vector<unsigned>::const_iterator it2 = this->objectsIndexesToRelease[it->second].begin(); it2 != this->objectsIndexesToRelease[it->second].end(); it2++)
		{
			stream << *it2;

			if (it2 + 1 != this->objectsIndexesToRelease[it->second].end())
			{
				stream << GG_STR(", ");
			}
		}

		stream << endl;
	}
}