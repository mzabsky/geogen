/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <string>
#include <math.h>
#include <stdarg.h>
#include <iostream>

#include "ggen.h"
#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"
#include "ggen_scriptarg.h"
#include "ggen_progress.h"

#include "ggen_squirrel.h"

DECLARE_ENUM_TYPE(GeoGen::NormalizationMode);
DECLARE_ENUM_TYPE(GeoGen::OverflowMode);
DECLARE_ENUM_TYPE(GeoGen::Direction);
DECLARE_ENUM_TYPE(GeoGen::MessageLevel);
DECLARE_ENUM_TYPE(GeoGen::VoronoiNoiseMode);
DECLARE_ENUM_TYPE(GeoGen::ComparisonMode);
DECLARE_ENUM_TYPE(GeoGen::OutlineMode);

namespace GeoGen{
	void GGen_ErrorHandler(HSQUIRRELVM,const SQChar * desc,const SQChar * source,SQInteger line,SQInteger column){
		Generator::GetInstance()->ThrowMessage(desc, GGEN_ERROR, line, column);
	}

	void GGen_PrintHandler(HSQUIRRELVM v,const SQChar * s,...){
		static SQChar temp[2048];
		va_list vl;
		va_start(vl,s);
		GGen_Vsprintf( temp,s,vl);

		va_end(vl);

		Generator::GetInstance()->ThrowMessage(temp, GGEN_ERROR);
	}

	SquirrelGenerator::SquirrelGenerator(){
	

		HSQUIRRELVM v = sq_open(1024);

		sq_pushroottable(v);

		sq_pop(v,1);

		SquirrelVM::InitNoRef(v);

		//SquirrelVM::Init();

		sqstd_register_mathlib(SquirrelVM::GetVMPtr());

		sq_setprintfunc(SquirrelVM::GetVMPtr(), GGen_PrintHandler);
		sq_setcompilererrorhandler(SquirrelVM::GetVMPtr(), GGen_ErrorHandler);
		sq_enabledebuginfo(SquirrelVM::GetVMPtr(), false);
		sqstd_seterrorhandlers(SquirrelVM::GetVMPtr());

		/* Arguments */
		RegisterGlobal(&AddIntArg, _SC("AddIntArg"));
		RegisterGlobal(&AddBoolArg, _SC("AddBoolArg"));
		RegisterGlobal(&AddEnumArg, _SC("AddEnumArg"));
		RegisterGlobal(&GetArgValue, _SC("GetArgValue"));

		/* Progress */
		RegisterGlobal(&InitProgress, _SC("InitProgress"));
		RegisterGlobal(&SetProgress, _SC("SetProgress"));
		RegisterGlobal(&IncreaseProgress, _SC("IncreaseProgress"));

		/* Constraint getters */
		RegisterGlobal(&Generator::GetMaxMapSize, _SC("GetMaxMapSize"));	
		RegisterGlobal(&Generator::GetMaxMapCount, _SC("GetMaxMapCount"));

		/* Enum: NormalizationMode */
		BindConstant(GGEN_ADDITIVE, _SC("GGEN_ADDITIVE"));
		BindConstant(GGEN_SUBSTRACTIVE, _SC("GGEN_SUBSTRACTIVE"));

		/* Enum: OverflowMode */
		BindConstant(GGEN_CYCLE, _SC("GGEN_CYCLE"));
		BindConstant(GGEN_DISCARD, _SC("GGEN_DISCARD"));
		BindConstant(GGEN_DISCARD_AND_FILL, _SC("GGEN_DISCARD_AND_FILL"));

		/* Enum: Direction */
		BindConstant(GGEN_HORIZONTAL, _SC("GGEN_HORIZONTAL"));
		BindConstant(GGEN_VERTICAL, _SC("GGEN_VERTICAL"));

		/* Enum: GGen_Message_Mode */
		BindConstant(GGEN_MESSAGE, _SC("GGEN_MESSAGE"));
		BindConstant(GGEN_NOTICE, _SC("GGEN_NOTICE"));
		BindConstant(GGEN_WARNING, _SC("GGEN_WARNING"));
		BindConstant(GGEN_ERROR, _SC("GGEN_ERROR"));

		/* Enum: GGen_Vornoi_Noise_Mode */
		BindConstant(GGEN_BUBBLES, _SC("GGEN_BUBBLES"));
		BindConstant(GGEN_RIDGES, _SC("GGEN_RIDGES"));

		/* Enum: ComparisonMode */
		BindConstant(GGEN_EQUAL_TO, _SC("GGEN_EQUAL_TO"));
		BindConstant(GGEN_NOT_EQUAL_TO, _SC("GGEN_NOT_EQUAL_TO"));
		BindConstant(GGEN_LESS_THAN, _SC("GGEN_LESS_THAN"));
		BindConstant(GGEN_GREATER_THAN, _SC("GGEN_GREATER_THAN"));
		BindConstant(GGEN_LESS_THAN_OR_EQUAL_TO, _SC("GGEN_LESS_THAN_OR_EQUAL_TO"));
		BindConstant(GGEN_GREATER_THAN_OR_EQUAL_TO, _SC("GGEN_GREATER_THAN_OR_EQUAL_TO"));

		/* Enum: OutlineMode */
		BindConstant(GGEN_INSIDE, _SC("GGEN_INSIDE"));
		BindConstant(GGEN_OUTSIDE, _SC("GGEN_OUTSIDE"));

		/* Class: Data_1D */
		SQClassDefNoConstructor<Data_1D>(_SC("Data_1D")).
			overloadConstructor<Data_1D(*)(uint16, int16)>().
			func(&Data_1D::Clone, _T("Clone")).

			func(&Data_1D::GetLength, _T("GetLength")).
			func(&Data_1D::SetValue, _T("SetValue")).
			func(&Data_1D::SetValueInRange, _T("SetValueInRange")).
			func(&Data_1D::GetValue, _T("GetValue")).
			func(&Data_1D::GetValueInterpolated, _T("GetValueInterpolated")).
		
			func(&Data_1D::Add, _T("Add")).
			func(&Data_1D::AddArray, _T("AddArray")).
			func(&Data_1D::AddTo, _T("AddTo")).
			func(&Data_1D::Multiply, _T("Multiply")).
			func(&Data_1D::MultiplyArray, _T("MultiplyArray")).
			func(&Data_1D::Invert,_T("Invert")).
			func(&Data_1D::Scale,_T("Scale")).
			func(&Data_1D::ScaleTo,_T("ScaleTo")).
			func(&Data_1D::ScaleValuesTo,_T("ScaleValuesTo")).
			func(&Data_1D::Fill,_T("Fill")).
			func(&Data_1D::ResizeCanvas,_T("ResizeCanvas")).
			func(&Data_1D::Clamp,_T("Clamp")).
			func(&Data_1D::Flip,_T("Flip")).
			func(&Data_1D::Min,_T("Min")).
			func(&Data_1D::Max,_T("Max")).
			func(&Data_1D::Shift,_T("Shift")).
			func(&Data_1D::Union,_T("Union")).
			func(&Data_1D::Intersection,_T("Intersection")).
			func(&Data_1D::Abs,_T("Abs")).
		
			func(&Data_1D::Monochrome,_T("Monochrome")).
			func(&Data_1D::Normalize,_T("Normalize")).
			func(&Data_1D::Gradient,_T("Gradient")).
			func(&Data_1D::Noise,_T("Noise")).
			func(&Data_1D::Smooth,_T("Smooth")).
			func(&Data_1D::Flood,_T("Flood")).
			func(&Data_1D::ToPath,_T("ToPath"));
	

		/* Class: Data_2D */
		SQClassDefNoConstructor<Data_2D>(_SC("Data_2D")).
			overloadConstructor<Data_2D(*)(uint16, uint16, int16)>().
			func(&Data_2D::Clone, _T("Clone")).
		
			func(&Data_2D::GetWidth, _T("GetWidth")).
			func(&Data_2D::GetHeight, _T("GetHeight")).
			func(&Data_2D::GetLength, _T("GetLength")).
			func(&Data_2D::SetValue, _T("SetValue")).
			func(&Data_2D::SetValueInRect, _T("SetValueInRect")).
			func(&Data_2D::GetValue, _T("GetValue")).
			func(&Data_2D::GetValueInterpolated, _T("GetValueInterpolated")).
		
			func(&Data_2D::Add,_T("Add")).
			func(&Data_2D::AddMap,_T("AddMap")).
			func(&Data_2D::AddTo, _T("AddTo")).
			func(&Data_2D::AddMasked,_T("AddMasked")).
			func(&Data_2D::AddMapMasked,_T("AddMapMasked")).
			func(&Data_2D::Multiply,_T("Multiply")).
			func(&Data_2D::MultiplyMap,_T("MultiplyMap")).
		
			func(&Data_2D::Invert,_T("Invert")).
			func(&Data_2D::Scale,_T("Scale")).
			func(&Data_2D::ScaleTo,_T("ScaleTo")).
			func(&Data_2D::ScaleValuesTo,_T("ScaleValuesTo")).
			func(&Data_2D::Fill,_T("Fill")).
			func(&Data_2D::ResizeCanvas,_T("ResizeCanvas")).
			func(&Data_2D::Clamp,_T("Clamp")).
			func(&Data_2D::CropValues,_T("CropValues")).
			func(&Data_2D::Min,_T("Min")).
			func(&Data_2D::Max,_T("Max")).
			func(&Data_2D::Flip,_T("Flip")).
			func(&Data_2D::Union,_T("Union")).
			func(&Data_2D::UnionTo,_T("UnionTo")).
			func(&Data_2D::Intersection,_T("Intersection")).
			func(&Data_2D::IntersectionTo,_T("IntersectionTo")).
			func(&Data_2D::TransformValues,_T("TransformValues")).
			func(&Data_2D::ReplaceValue,_T("ReplaceValue")).
			func(&Data_2D::Abs,_T("Abs")).
			func(&Data_2D::Combine,_T("Combine")).
		
			func(&Data_2D::Shift,_T("Shift")).
			func(&Data_2D::Project,_T("Project")).
			func(&Data_2D::GetProfile,_T("GetProfile")).

			func(&Data_2D::Gradient,_T("Gradient")).
			func(&Data_2D::GradientFromProfile,_T("GradientFromProfile")).
			func(&Data_2D::RadialGradient,_T("RadialGradient")).
			func(&Data_2D::RadialGradientFromProfile,_T("RadialGradientFromProfile")).
			func(&Data_2D::Smooth,_T("Smooth")).	
			func(&Data_2D::SmoothDirection,_T("SmoothDirection")).	
			func(&Data_2D::Noise,_T("Noise")).		
			func(&Data_2D::VoronoiNoise,_T("VoronoiNoise")).		
			func(&Data_2D::Flood,_T("Flood")).
			func(&Data_2D::Pattern,_T("Pattern")).
			func(&Data_2D::Monochrome,_T("Monochrome")).
			func(&Data_2D::SelectValue,_T("SelectValue")).
			func(&Data_2D::SlopeMap,_T("SlopeMap")).
			func(&Data_2D::Scatter,_T("Scatter")).
			func(&Data_2D::ReturnAs,_T("ReturnAs")).
			func(&Data_2D::Transform,_T("Transform")).
			func(&Data_2D::Rotate,_T("Rotate")).
			func(&Data_2D::Shear,_T("Shear")).
			func(&Data_2D::Normalize,_T("Normalize")).
			func(&Data_2D::NormalizeDirection,_T("NormalizeDirection")).
			func(&Data_2D::FillPolygon,_T("FillPolygon")).
			func(&Data_2D::StrokePath,_T("StrokePath")).
			func(&Data_2D::FloodFill,_T("FloodFill")).
			func(&Data_2D::FloodSelect,_T("FloodSelect")).
			func(&Data_2D::GetMaxValueOnPath,_T("GetMaxValueOnPath")).
			func(&Data_2D::GetMinValueOnPath,_T("GetMinValueOnPath")).
			func(&Data_2D::Expand,_T("Expand")).
			func(&Data_2D::Shrink,_T("Shrink")).
			func(&Data_2D::ExpandDirection,_T("ExpandDirection")).
			func(&Data_2D::ShrinkDirection,_T("ShrinkDirection")).
			func(&Data_2D::Outline,_T("Outline")).
			func(&Data_2D::ConvexityMap,_T("ConvexityMap")).
			func(&Data_2D::Distort,_T("Distort")).
			func(&Data_2D::NormalMap,_T("NormalMap")).
			func(&Data_2D::NormalDifferenceMap,_T("NormalDifferenceMap")).
			func(&Data_2D::GetNormal,_T("GetNormal")).
			func(&Data_2D::SimpleErosion,_T("SimpleErosion"));

		/* Class: Amplitudes */
		SQClassDefNoConstructor<Amplitudes>(_SC("Amplitudes")).
			overloadConstructor<Amplitudes(*)(uint8)>().
			func(&Amplitudes::AddAmplitude,_T("AddAmplitude"));

		/* Class: Point */
		SQClassDefNoConstructor<Point>(_SC("Point")).
			overloadConstructor<Point(*)(Coord, Coord)>().
			func(&Point::GetX,_T("GetX")).
			func(&Point::GetY,_T("GetY")).
			func(&Point::SetX,_T("SetX")).
			func(&Point::SetY,_T("SetY")).
			func(&Point::SetCoords,_T("SetCoords"));

		/* Class: Path */
		SQClassDefNoConstructor<Path>(_SC("Path")).
			overloadConstructor<Path(*)()>().
			func(&Path::AddPoint,_T("AddPoint")).
			func(&Path::AddPointByCoords,_T("AddPointByCoords")).
			func(&Path::RemovePoint,_T("RemovePoint")).
			func(&Path::InsertPoint,_T("InsertPoint")).
			func(&Path::InsertPointByCoords,_T("InsertPointByCoords")).
			func(&Path::GetMinX,_T("GetMinX")).
			func(&Path::GetMinY,_T("GetMinY")).
			func(&Path::GetMaxX,_T("GetMaxX")).
			func(&Path::GetMaxY,_T("GetMaxY")).
			func(&Path::GetAverageX,_T("GetAverageX")).
			func(&Path::GetAverageY,_T("GetAverageY")).
			func(&Path::GetCenterX,_T("GetCenterX")).
			func(&Path::GetCenterY,_T("GetCenterY")).
			func(&Path::Transform,_T("Transform")).
			func(&Path::Rotate,_T("Rotate")).
			func(&Path::Flip,_T("Flip")).
			func(&Path::Shear,_T("Shear")).
			func(&Path::Scale,_T("Scale")).
			func(&Path::Clear,_T("Clear")).
			func(&Path::Move,_T("Move"));

		/* Constants: GGEN_MIN/MAX/INVALID_HEIGHT */
		BindConstant(GGEN_MIN_HEIGHT, _SC("GGEN_MIN_HEIGHT"));
		BindConstant(GGEN_MAX_HEIGHT, _SC("GGEN_MAX_HEIGHT"));
		BindConstant(GGEN_INVALID_HEIGHT, _SC("GGEN_INVALID_HEIGHT"));
		BindConstant(GGEN_MAX_PATH_LENGTH, _SC("GGEN_MAX_PATH_LENGTH"));

		#include "ggen_presets.h"
	}

	SquirrelGenerator::~SquirrelGenerator(){	
		// delete contents of all presets
		for(list<void*>::iterator it = this->presets.begin(); it != this->presets.end(); it++){
			delete *it;
		}

		// empty the preset list
		this->presets.clear();

		SquirrelVM::Shutdown();	
	}


	bool SquirrelGenerator::SetScript(const String& script){
		try {
			SquirrelObject sqScript = SquirrelVM::CompileBuffer(script.c_str());

			SquirrelVM::RunScript(sqScript);

			this->status = GGEN_SCRIPT_LOADED;

			return true;
		} catch (SquirrelError &) {
			return false;
		}
	}

	String SquirrelGenerator::GetInfo(const String& label){
		assert(this->status == GGEN_SCRIPT_LOADED || this->status == GGEN_READY_TO_GENERATE);

		Status statusBackup = this->status;

		this->status = GGEN_LOADING_MAP_INFO;

		try{
			SquirrelFunction<const SQChar*> callFunc(StaticString("GetInfo"));

			const SQChar* output = callFunc(label.c_str());

			this->status = statusBackup;

			return String(output);
		}
		catch(...){
			this->status = statusBackup;

			return NULL;
		}
	}

	int SquirrelGenerator::GetInfoInt(const String& label){
		assert(this->status == GGEN_SCRIPT_LOADED || this->status == GGEN_READY_TO_GENERATE);

		Status statusBackup = this->status;

		this->status = GGEN_LOADING_MAP_INFO;

		try{
			SquirrelFunction<int> callFunc(StaticString("GetInfo"));

			int value = callFunc(label.c_str());

			this->status = statusBackup;

			return value;
		}
		catch(...){
			this->status = statusBackup;

			return -1;
		}
	}

	int16* SquirrelGenerator::Generate(){		
		assert(this->status == GGEN_READY_TO_GENERATE);
	
		HSQUIRRELVM v = SquirrelVM::GetVMPtr();

		// remember the current state of the stack (so it can be restored later)
		int top = sq_gettop(v);

		try {
			int16* return_data;
			Data_2D* data;

			{
				this->status = GGEN_GENERATING;			

				sq_pushroottable(v);

				// name of the function being called
				sq_pushstring(v, StaticString("Generate"),-1); 
			
				Amplitudes* a = (Amplitudes*) (*(this->presets.begin()));

				// does the Generate() function exist?
				if(SQ_SUCCEEDED(sq_get(v,-2))) {
					// push "this" (since Generate() is supposed to be a global function use root table)
					sq_pushroottable(v);

					// invoke the function
					sq_call(v, 1, true, true);							

					// the output instance
					SQUserPointer outInstance;

					// did the function return an instance of a Data_2D class?
					if(SQ_SUCCEEDED(sq_getinstanceup(v, -1, &outInstance, ClassType<Data_2D>::type()))){
						data = (Data_2D*) outInstance;
					}
					else {
						throw SquirrelError();
					}
				}
				else {
					Generator::GetInstance()->ThrowMessage(StaticString("\"Generate()\" function was not found in the script!"), GGEN_ERROR, -1);

					throw SquirrelError();
				}

				// restore original state of the stack
				sq_settop(v, top);

				this->status = GGEN_READY_TO_GENERATE;
			}

			assert(data != NULL && data->data != NULL);

			output_width = data->width;
			output_height = data->height;

			return_data = new int16[output_width * output_height];

			assert(return_data != NULL);

			memcpy(return_data, data->data, sizeof(int16) * output_width * output_height);

			/* The internal squirrel reference to the returned object is not released until next c++ => squirrel
			call. So we call the script header with most likely nonexistant identification string to release the 
			reference held by squirrel.h */
			this->GetInfoInt(StaticString("voidCall"));

			/* Free all remaining 2D instances (those created via Clone) */
			Data_2D::FreeAllInstances();

			return return_data;		
		} 
		/*catch (SquirrelError &) {		
			Data_2D::FreeAllInstances();		

			this->status = GGEN_READY_TO_GENERATE;

			return NULL;
		}*/
		catch (ScriptAssertException &) {
			Data_2D::FreeAllInstances();

			this->status = GGEN_READY_TO_GENERATE;

			return NULL;
		}
		catch (bad_alloc){
			Data_2D::FreeAllInstances();

			this->status = GGEN_READY_TO_GENERATE;
		
			Generator::GetInstance()->ThrowMessage(StaticString("GGen_Data memory allocation failed!"), GGEN_ERROR, -1);

			return NULL;
		}
	}

	void SquirrelGenerator::RegisterPreset(Data_1D* preset, const String& label){
		BindVariable(preset, label.c_str(), VAR_ACCESS_READ_ONLY);

		this->presets.push_back(preset);
	}

	void SquirrelGenerator::RegisterPreset(Data_2D* preset, const String& label){
		BindVariable(preset, label.c_str(), VAR_ACCESS_READ_ONLY);

		this->presets.push_back(preset);
	}

	void SquirrelGenerator::RegisterPreset(Amplitudes* preset, const String& label){
		//BindVariable(preset, label.c_str(), VAR_ACCESS_READ_ONLY);

		int i = preset->data[1];

		this->presets.push_back(preset);
	}
}


