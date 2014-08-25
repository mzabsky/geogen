#include "HeightProfileTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"

#include "HeightProfileAbsFunctionDefinition.hpp"
#include "HeightProfileAddFunctionDefinition.hpp"
#include "HeightProfileBlurFunctionDefinition.hpp"
#include "HeightProfileClampHeightsFunctionDefinition.hpp"
#include "HeightProfileCombineFunctionDefinition.hpp"
#include "HeightProfileCropHeightsFunctionDefinition.hpp"
#include "HeightProfileFillFunctionDefinition.hpp"
#include "HeightProfileFillIntervalFunctionDefinition.hpp"
#include "HeightProfileIntersectFunctionDefinition.hpp"
#include "HeightProfileInvertFunctionDefinition.hpp"
#include "HeightProfileMoveFunctionDefinition.hpp"
#include "HeightProfileMultiplyFunctionDefinition.hpp"
#include "HeightProfileRescaleFunctionDefinition.hpp"
#include "HeightProfileUnifyFunctionDefinition.hpp"

#include "HeightProfileFlatFunctionDefinition.hpp"
#include "HeightProfileFromArrayFunctionDefinition.hpp"
#include "HeightProfileGradientFunctionDefinition.hpp"
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
	this->GetFunctionDefinitions().AddItem(new HeightProfileCropHeightsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileFillFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileFillIntervalFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileIntersectFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileInvertFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileMoveFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileMultiplyFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileRescaleFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightProfileUnifyFunctionDefinition(this));

	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileFlatFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileFromArrayFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileGradientFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfilePatternFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightProfileSliceFunctionDefinition(this));
}