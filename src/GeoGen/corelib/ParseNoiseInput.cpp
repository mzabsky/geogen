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

#include "ParseNoiseInput.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/IncorrectTypeException.hpp"
#include "../genlib/NoiseLayersFactory.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "HeightMapNoiseRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "NonpositiveNoiseLayerWaveLengthException.hpp"
#include "NonpositiveNoiseLayerAmplitudeException.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace genlib;

genlib::NoiseLayers geogen::corelib::ParseNoiseInput(VirtualMachine* vm, CodeLocation location, std::vector<runtime::ManagedObject*> arguments)
{
	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Array")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	NoiseLayers layers;
	if (arguments.size() > 0)
	{
		ArrayObject* array = dynamic_cast<ArrayObject*>(arguments[0]);

		for (ArrayObject::iterator it = array->Begin(); it != array->End(); it++)
		{
			ManagedObject* key = *it;
			ManagedObject* value = array->Get(vm, location, key);
			if (key->GetType() != numberTypeDefinition)
			{
				throw IncorrectTypeException(GGE2708_IncorrectNoiseLayerWaveLengthType, location, GG_STR("Number"), (*it)->GetType()->GetName());
			}

			if (value->GetType() != numberTypeDefinition)
			{
				throw IncorrectTypeException(GGE2709_IncorrectNoiseLayerAmplitudeType, location, GG_STR("Number"), (*it)->GetType()->GetName());
			}

			Number waveLengthNumber = dynamic_cast<NumberObject*>(key)->GetValue();
			Number amplitudeNumber = dynamic_cast<NumberObject*>(value)->GetValue();

			if (waveLengthNumber <= 0)
			{
				throw NonpositiveNoiseLayerWaveLengthException(location);
			}

			if (amplitudeNumber <= 0)
			{
				throw NonpositiveNoiseLayerAmplitudeException(location);
			}

			Size1D waveLength = (Size1D)waveLengthNumber;

			Height amplitude;
			if (!TryNumberToHeight(amplitudeNumber, amplitude))
			{
				throw HeightOverflowException(location);
			}

			layers[waveLength] = amplitude;
		}
	}
	else
	{
		layers = NoiseLayersFactory::CreateDefaultLayers();
	}

	return layers;
}