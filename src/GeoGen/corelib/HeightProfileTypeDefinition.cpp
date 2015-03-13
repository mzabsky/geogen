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

#include "HeightProfileTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"

#include "HeightProfileAbsFunctionDefinition.hpp"
#include "HeightProfileAddFunctionDefinition.hpp"
#include "HeightProfileBlurFunctionDefinition.hpp"
#include "HeightProfileClampHeightsFunctionDefinition.hpp"
#include "HeightProfileCombineFunctionDefinition.hpp"
#include "HeightProfileCropFunctionDefinition.hpp"
#include "HeightProfileCropHeightsFunctionDefinition.hpp"
#include "HeightProfileFillFunctionDefinition.hpp"
#include "HeightProfileFillIntervalFunctionDefinition.hpp"
#include "HeightProfileFlipFunctionDefinition.hpp"
#include "HeightProfileIntersectFunctionDefinition.hpp"
#include "HeightProfileInvertFunctionDefinition.hpp"
#include "HeightProfileMoveFunctionDefinition.hpp"
#include "HeightProfileMultiplyFunctionDefinition.hpp"
#include "HeightProfileRescaleFunctionDefinition.hpp"
#include "HeightProfileUnifyFunctionDefinition.hpp"

#include "HeightProfileFlatFunctionDefinition.hpp"
#include "HeightProfileFromArrayFunctionDefinition.hpp"
#include "HeightProfileGradientFunctionDefinition.hpp"
#include "HeightProfileNoiseFunctionDefinition.hpp"
#include "HeightProfilePatternFunctionDefinition.hpp"
#include "HeightProfileSliceFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

HeightProfileTypeDefinition::HeightProfileTypeDefinition() : RendererTypeDefinition(GG_STR("HeightProfile")) 
{
	this->GetFunctionDefinitions().AddItem(new HeightProfileAbsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileAddFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileBlurFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileClampHeightsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileCombineFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileCropFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileCropHeightsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileFillFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileFillIntervalFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileFlipFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileIntersectFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileInvertFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileMoveFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileMultiplyFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileRescaleFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileUnifyFunctionDefinition(this));

	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileFlatFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileFromArrayFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileGradientFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileNoiseFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfilePatternFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileSliceFunctionDefinition(this));
}