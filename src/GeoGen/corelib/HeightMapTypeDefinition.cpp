
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
#include "HeightMapFillFunctionDefinition.hpp"
#include "HeightMapFillRectangleFunctionDefinition.hpp"
#include "HeightMapIntersectFunctionDefinition.hpp"
#include "HeightMapInvertFunctionDefinition.hpp"
#include "HeightMapMoveFunctionDefinition.hpp"
#include "HeightMapMultiplyFunctionDefinition.hpp"
#include "HeightMapRescaleFunctionDefinition.hpp"
#include "HeightMapRotateFunctionDefinition.hpp"
#include "HeightMapShearFunctionDefinition.hpp"
#include "HeightMapShiftFunctionDefinition.hpp"
#include "HeightMapTransformFunctionDefinition.hpp"
#include "HeightMapUnifyFunctionDefinition.hpp"

#include "HeightMapCloneFunctionDefinition.hpp"
#include "HeightMapConvexityMapFunctionDefinition.hpp"
#include "HeightMapDistanceMapFunctionDefinition.hpp"
#include "HeightMapFlatFunctionDefinition.hpp"
#include "HeightMapGradientFunctionDefinition.hpp"
#include "HeightMapNoiseFunctionDefinition.hpp"
#include "HeightMapProjectionFunctionDefinition.hpp"
#include "HeightMapRadialGradientFunctionDefinition.hpp"
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
	this->GetFunctionDefinitions().AddItem(new HeightMapFillFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapFillRectangleFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapIntersectFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapInvertFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapMoveFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapMultiplyFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapRescaleFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapRotateFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapShearFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapShiftFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapTransformFunctionDefinition(this));
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
}