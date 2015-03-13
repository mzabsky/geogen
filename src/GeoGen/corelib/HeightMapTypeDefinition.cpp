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


#include "HeightMapTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"

#include "HeightMapAbsFunctionDefinition.hpp"
#include "HeightMapAddFunctionDefinition.hpp"
#include "HeightMapBlurFunctionDefinition.hpp"
#include "HeightMapClampHeightsFunctionDefinition.hpp"
#include "HeightMapCombineFunctionDefinition.hpp"
#include "HeightMapCropFunctionDefinition.hpp"
#include "HeightMapCropHeightsFunctionDefinition.hpp"
#include "HeightMapDistortFunctionDefinition.hpp"
#include "HeightMapDrawLineFunctionDefinition.hpp"
#include "HeightMapFillFunctionDefinition.hpp"
#include "HeightMapFillRectangleFunctionDefinition.hpp"
#include "HeightMapFlipFunctionDefinition.hpp"
#include "HeightMapGlaciateFunctionDefinition.hpp"
#include "HeightMapIntersectFunctionDefinition.hpp"
#include "HeightMapInvertFunctionDefinition.hpp"
#include "HeightMapMoveFunctionDefinition.hpp"
#include "HeightMapMultiplyFunctionDefinition.hpp"
#include "HeightMapRescaleFunctionDefinition.hpp"
#include "HeightMapRotateFunctionDefinition.hpp"
#include "HeightMapShearFunctionDefinition.hpp"
#include "HeightMapShiftFunctionDefinition.hpp"
#include "HeightMapStratifyFunctionDefinition.hpp"
#include "HeightMapTransformFunctionDefinition.hpp"
#include "HeightMapTransformHeightsFunctionDefinition.hpp"
#include "HeightMapUnifyFunctionDefinition.hpp"

#include "HeightMapCloneFunctionDefinition.hpp"
#include "HeightMapConvexityMapFunctionDefinition.hpp"
#include "HeightMapDistanceMapFunctionDefinition.hpp"
#include "HeightMapFlatFunctionDefinition.hpp"
#include "HeightMapGradientFunctionDefinition.hpp"
#include "HeightMapNoiseFunctionDefinition.hpp"
#include "HeightMapProjectionFunctionDefinition.hpp"
#include "HeightMapRadialGradientFunctionDefinition.hpp"
#include "HeightMapRidgedNoiseFunctionDefinition.hpp"
#include "HeightMapPatternFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

HeightMapTypeDefinition::HeightMapTypeDefinition() : RendererTypeDefinition(GG_STR("HeightMap"))
{
	this->GetFunctionDefinitions().AddItem(new HeightMapAbsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapAddFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapBlurFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapClampHeightsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapCombineFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapCropFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapCropHeightsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapDistortFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapDrawLineFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapFillFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapFillRectangleFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapFlipFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapGlaciateFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapIntersectFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapInvertFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapMoveFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapMultiplyFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapRescaleFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapRotateFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapShearFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapShiftFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapStratifyFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapTransformFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapTransformHeightsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapUnifyFunctionDefinition(this));

	this->GetStaticFunctionDefinitions().AddItem(new HeightMapCloneFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapConvexityMapFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapDistanceMapFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapFlatFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapGradientFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapProjectionFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapRadialGradientFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapPatternFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapNoiseFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapRidgedNoiseFunctionDefinition(this));
}