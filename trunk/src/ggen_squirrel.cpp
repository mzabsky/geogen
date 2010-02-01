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

#include "ggen.h"
#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"
#include "ggen_scriptarg.h"

#include "sqplus.h"
#include "sqstdmath.h"

#include "ggen_squirrel.h"

DECLARE_ENUM_TYPE(GGen_Normalization_Mode);
DECLARE_ENUM_TYPE(GGen_Overflow_Mode);
DECLARE_ENUM_TYPE(GGen_Direction);
DECLARE_ENUM_TYPE(GGen_Message_Level);
DECLARE_ENUM_TYPE(GGen_Voronoi_Noise_Mode);
DECLARE_ENUM_TYPE(GGen_Comparsion_Mode);

void GGen_ErrorHandler(HSQUIRRELVM,const SQChar * desc,const SQChar * source,SQInteger line,SQInteger column){
	GGen::GetInstance()->ThrowMessage(desc, GGEN_ERROR, line, column);
}

void GGen_PrintHandler(HSQUIRRELVM v,const SQChar * s,...){
	static SQChar temp[2048];
	va_list vl;
	va_start(vl,s);
	scvsprintf( temp,s,vl);
	va_end(vl);

	GGen::GetInstance()->ThrowMessage(temp, GGEN_ERROR);
}

GGen_Squirrel::GGen_Squirrel(){
	//ggen_current_object = this;

	SquirrelVM::Init();

	sqstd_register_mathlib(SquirrelVM::GetVMPtr());

	sq_setprintfunc(SquirrelVM::GetVMPtr(), GGen_PrintHandler);
	sq_setcompilererrorhandler(SquirrelVM::GetVMPtr(), GGen_ErrorHandler);
	sq_enabledebuginfo(SquirrelVM::GetVMPtr(), false);

	/* Arguments */
	RegisterGlobal(&GGen_AddIntArg, _SC("GGen_AddIntArg"));
	RegisterGlobal(&GGen_AddBoolArg, _SC("GGen_AddBoolArg"));
	RegisterGlobal(&GGen_AddEnumArg, _SC("GGen_AddEnumArg"));
	RegisterGlobal(&GGen_GetParam, _SC("GGen_GetParam"));

	/* Progress */
	RegisterGlobal(&GGen::InitProgress, _SC("GGen_InitProgress"));
	RegisterGlobal(&GGen::SetProgress, _SC("GGen_SetProgress"));
	RegisterGlobal(&GGen::IncreaseProgress, _SC("GGen_IncreaseProgress"));

	/* Constraint getters */
	RegisterGlobal(&GGen::GetMaxHeight, _SC("GGen_GetMaxHeight"));
	RegisterGlobal(&GGen::GetMaxWidth, _SC("GGen_GetMaxWidth"));
	RegisterGlobal(&GGen::GetMaxMapCount, _SC("GGen_GetMaxMapCount"));

	/* Enum: GGen_Normalization_Mode */
	BindConstant(GGEN_ADDITIVE, _SC("GGEN_ADDITIVE"));
	BindConstant(GGEN_SUBSTRACTIVE, _SC("GGEN_SUBSTRACTIVE"));

	/* Enum: GGen_Overflow_Mode */
	BindConstant(GGEN_CYCLE, _SC("GGEN_CYCLE"));
	BindConstant(GGEN_DISCARD, _SC("GGEN_DISCARD"));
	BindConstant(GGEN_DISCARD_AND_FILL, _SC("GGEN_DISCARD_AND_FILL"));

	/* Enum: GGen_Direction */
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

	/* Enum: GGen_Comparsion_Mode */
	BindConstant(GGEN_EQUAL_TO, _SC("GGEN_EQUAL_TO"));
	BindConstant(GGEN_NOT_EQUAL_TO, _SC("GGEN_NOT_EQUAL_TO"));
	BindConstant(GGEN_LESS_THAN, _SC("GGEN_LESS_THAN"));
	BindConstant(GGEN_GREATER_THAN, _SC("GGEN_GREATER_THAN"));
	BindConstant(GGEN_LESS_THAN_OR_EQUAL_TO, _SC("GGEN_LESS_THAN_OR_EQUAL_TO"));
	BindConstant(GGEN_GREATER_THAN_OR_EQUAL_TO, _SC("GGEN_GREATER_THAN_OR_EQUAL_TO"));


	// WARNING: I'm too lazy to replace these with the TypeDefs currently. Maybe later...
	
	/* Class: GGen_Data_1D */
	SQClassDefNoConstructor<GGen_Data_1D>(_SC("GGen_Data_1D")).
		overloadConstructor<GGen_Data_1D(*)(uint16, int16)>().
		func(&GGen_Data_1D::Clone, _T("Clone")).

		func(&GGen_Data_1D::GetLength, _T("GetLength")).
		func(&GGen_Data_1D::SetValue, _T("SetValue")).
		func(&GGen_Data_1D::SetValueInRange, _T("SetValueInRange")).
		func(&GGen_Data_1D::GetValue, _T("GetValue")).
		func(&GGen_Data_1D::GetValueInterpolated, _T("GetValueInterpolated")).
		
		func(&GGen_Data_1D::Add, _T("Add")).
		func(&GGen_Data_1D::AddArray, _T("AddArray")).
		func(&GGen_Data_1D::AddTo, _T("AddTo")).
		func(&GGen_Data_1D::Multiply, _T("Multiply")).
		func(&GGen_Data_1D::MultiplyArray, _T("MultiplyArray")).
		func(&GGen_Data_1D::Invert,_T("Invert")).
		func(&GGen_Data_1D::Scale,_T("Scale")).
		func(&GGen_Data_1D::ScaleTo,_T("ScaleTo")).
		func(&GGen_Data_1D::ScaleValuesTo,_T("ScaleValuesTo")).
		func(&GGen_Data_1D::Fill,_T("Fill")).
		func(&GGen_Data_1D::ResizeCanvas,_T("ResizeCanvas")).
		func(&GGen_Data_1D::Clamp,_T("Clamp")).
		func(&GGen_Data_1D::Flip,_T("Flip")).
		func(&GGen_Data_1D::Min,_T("Min")).
		func(&GGen_Data_1D::Max,_T("Max")).
		func(&GGen_Data_1D::Shift,_T("Shift")).
		func(&GGen_Data_1D::Union,_T("Union")).
		func(&GGen_Data_1D::Intersection,_T("Intersection")).
		func(&GGen_Data_1D::Abs,_T("Abs")).
		
		func(&GGen_Data_1D::Monochrome,_T("Monochrome")).
		func(&GGen_Data_1D::Normalize,_T("Normalize")).
		func(&GGen_Data_1D::Gradient,_T("Gradient")).
		func(&GGen_Data_1D::Noise,_T("Noise")).
		func(&GGen_Data_1D::Smooth,_T("Smooth")).
		func(&GGen_Data_1D::Flood,_T("Flood")).
		func(&GGen_Data_1D::ToPath,_T("ToPath"));
	

	/* Class: GGen_Data_2D */
	SQClassDefNoConstructor<GGen_Data_2D>(_SC("GGen_Data_2D")).
		overloadConstructor<GGen_Data_2D(*)(uint16, uint16, int16)>().
		func(&GGen_Data_2D::Clone, _T("Clone")).
		
		func(&GGen_Data_2D::GetWidth, _T("GetWidth")).
		func(&GGen_Data_2D::GetHeight, _T("GetHeight")).
		func(&GGen_Data_2D::GetLength, _T("GetLength")).
		func(&GGen_Data_2D::SetValue, _T("SetValue")).
		func(&GGen_Data_2D::SetValueInRect, _T("SetValueInRect")).
		func(&GGen_Data_2D::GetValue, _T("GetValue")).
		func(&GGen_Data_2D::GetValueInterpolated, _T("GetValueInterpolated")).
		
		func(&GGen_Data_2D::Add,_T("Add")).
		func(&GGen_Data_2D::AddMap,_T("AddMap")).
		func(&GGen_Data_2D::AddTo, _T("AddTo")).
		func(&GGen_Data_2D::AddMasked,_T("AddMasked")).
		func(&GGen_Data_2D::AddMapMasked,_T("AddMapMasked")).
		func(&GGen_Data_2D::Multiply,_T("Multiply")).
		func(&GGen_Data_2D::MultiplyMap,_T("MultiplyMap")).
		
		func(&GGen_Data_2D::Invert,_T("Invert")).
		func(&GGen_Data_2D::Scale,_T("Scale")).
		func(&GGen_Data_2D::ScaleTo,_T("ScaleTo")).
		func(&GGen_Data_2D::ScaleValuesTo,_T("ScaleValuesTo")).
		func(&GGen_Data_2D::Fill,_T("Fill")).
		func(&GGen_Data_2D::ResizeCanvas,_T("ResizeCanvas")).
		func(&GGen_Data_2D::Clamp,_T("Clamp")).
		func(&GGen_Data_2D::Min,_T("Min")).
		func(&GGen_Data_2D::Max,_T("Max")).
		func(&GGen_Data_2D::Flip,_T("Flip")).
		func(&GGen_Data_2D::Union,_T("Union")).
		func(&GGen_Data_2D::UnionTo,_T("UnionTo")).
		func(&GGen_Data_2D::Intersection,_T("Intersection")).
		func(&GGen_Data_2D::IntersectionTo,_T("IntersectionTo")).
		func(&GGen_Data_2D::TransformValues,_T("TransformValues")).
		func(&GGen_Data_2D::ReplaceValue,_T("ReplaceValue")).
		func(&GGen_Data_2D::Abs,_T("Abs")).
		func(&GGen_Data_2D::Combine,_T("Combine")).
		
		func(&GGen_Data_2D::Shift,_T("Shift")).
		func(&GGen_Data_2D::Project,_T("Project")).
		func(&GGen_Data_2D::GetProfile,_T("GetProfile")).

		func(&GGen_Data_2D::Gradient,_T("Gradient")).
		func(&GGen_Data_2D::GradientFromProfile,_T("GradientFromProfile")).
		func(&GGen_Data_2D::RadialGradient,_T("RadialGradient")).
		func(&GGen_Data_2D::RadialGradientFromProfile,_T("RadialGradientFromProfile")).
		func(&GGen_Data_2D::Smooth,_T("Smooth")).	
		func(&GGen_Data_2D::SmoothDirection,_T("SmoothDirection")).	
		func(&GGen_Data_2D::Noise,_T("Noise")).		
		func(&GGen_Data_2D::VoronoiNoise,_T("VoronoiNoise")).		
		func(&GGen_Data_2D::Flood,_T("Flood")).
		func(&GGen_Data_2D::Pattern,_T("Pattern")).
		func(&GGen_Data_2D::Monochrome,_T("Monochrome")).
		func(&GGen_Data_2D::SelectValue,_T("SelectValue")).
		func(&GGen_Data_2D::SlopeMap,_T("SlopeMap")).
		func(&GGen_Data_2D::Scatter,_T("Scatter")).
		func(&GGen_Data_2D::ReturnAs,_T("ReturnAs")).
		func(&GGen_Data_2D::Transform,_T("Transform")).
		func(&GGen_Data_2D::Rotate,_T("Rotate")).
		func(&GGen_Data_2D::Shear,_T("Shear")).
		func(&GGen_Data_2D::Normalize,_T("Normalize")).
		func(&GGen_Data_2D::NormalizeDirection,_T("NormalizeDirection")).
		func(&GGen_Data_2D::FillPolygon,_T("FillPolygon")).
		func(&GGen_Data_2D::StrokePath,_T("StrokePath")).
		func(&GGen_Data_2D::FloodFill,_T("FloodFill")).
		func(&GGen_Data_2D::FloodSelect,_T("FloodSelect"));

	/* Class: GGen_Amplitudes */
	SQClassDefNoConstructor<GGen_Amplitudes>(_SC("GGen_Amplitudes")).
		overloadConstructor<GGen_Amplitudes(*)(uint8)>().
		func(&GGen_Amplitudes::AddAmplitude,_T("AddAmplitude"));

	/* Class: GGen_Point */
	SQClassDefNoConstructor<GGen_Point>(_SC("GGen_Point")).
		overloadConstructor<GGen_Point(*)(GGen_Coord, GGen_Coord)>().
		func(&GGen_Point::GetX,_T("GetX")).
		func(&GGen_Point::GetY,_T("GetY")).
		func(&GGen_Point::SetX,_T("SetX")).
		func(&GGen_Point::SetY,_T("SetY")).
		func(&GGen_Point::SetCoords,_T("SetCoords"));

	/* Class: GGen_Path */
	SQClassDefNoConstructor<GGen_Path>(_SC("GGen_Path")).
		overloadConstructor<GGen_Path(*)()>().
		func(&GGen_Path::AddPoint,_T("AddPoint")).
		func(&GGen_Path::AddPointByCoords,_T("AddPointByCoords")).
		func(&GGen_Path::RemovePoint,_T("RemovePoint")).
		func(&GGen_Path::InsertPoint,_T("InsertPoint")).
		func(&GGen_Path::InsertPointByCoords,_T("InsertPointByCoords")).
		func(&GGen_Path::Transform,_T("Transform")).
		func(&GGen_Path::Rotate,_T("Rotate")).
		func(&GGen_Path::Flip,_T("Flip")).
		func(&GGen_Path::Shear,_T("Shear")).
		func(&GGen_Path::Scale,_T("Scale")).
		func(&GGen_Path::Clear,_T("Clear")).
		func(&GGen_Path::Move,_T("Move"));

	/* Constants: GGEN_MIN/MAX/INVALID_HEIGHT */
	BindConstant(GGEN_MIN_HEIGHT, _SC("GGEN_MIN_HEIGHT"));
	BindConstant(GGEN_MAX_HEIGHT, _SC("GGEN_MAX_HEIGHT"));
	BindConstant(GGEN_INVALID_HEIGHT, _SC("GGEN_INVALID_HEIGHT"));
}

GGen_Squirrel::~GGen_Squirrel(){
	SquirrelVM::Shutdown();	
}


bool GGen_Squirrel::SetScript(const char* script){
	
	wchar_t* buf = new wchar_t[strlen(script) + 1];
	
	mbstowcs(buf, script, strlen(script) + 1);

	try {
		SquirrelObject sqScript = SquirrelVM::CompileBuffer(buf);

		SquirrelVM::RunScript(sqScript);
	
		delete [] buf;

		return true;
    } catch (SquirrelError &) {
		delete [] buf;
    
		return false;
    }	
}

char* GGen_Squirrel::GetInfo(char* label){
	SQChar* in_buf = new SQChar[strlen(label) + 1];
	
	try{
		mbstowcs(in_buf, label, strlen(label));

		in_buf[strlen(label)] = _SC('\0');

		SqPlus::sq_std_string input = SqPlus::sq_std_string(in_buf);

		SquirrelFunction<const SQChar*> callFunc(_SC("GetInfo"));

		const SQChar* output =  callFunc(in_buf);

		delete [] in_buf;

		return GGen_ToCString(output);
	}
	catch(SquirrelError &){
		delete [] in_buf;
	
		return NULL;
	}
}

int GGen_Squirrel::GetInfoInt(char* label){
	SQChar* in_buf = new SQChar[strlen(label) + 1];
	
	try{
		mbstowcs(in_buf, label, strlen(label));

		in_buf[strlen(label)]=_SC('\0');

		SqPlus::sq_std_string input = SqPlus::sq_std_string(in_buf);

		SquirrelFunction<int> callFunc(_SC("GetInfo"));
		int ret = callFunc(in_buf);
		
		delete [] in_buf;
		
		return ret;
	}
	catch(SquirrelError &){
		delete [] in_buf;
	
		return -1;
	}
}

int16* GGen_Squirrel::Generate(){		
	try {
		int16* return_data;
		GGen_Data_2D* data;

		{
			SquirrelFunction<GGen_Data_2D*> callFunc(_SC("Generate"));
			
			presetTarget = &callFunc.object;
			
			#include "ggen_presets.h"
			
			presetTarget = NULL;
			
			data =  callFunc();	
		}

		GGen_Script_Assert(data != NULL && data->data != NULL);


		output_width = data->width;
		output_height = data->height;

		return_data = new int16[output_width * output_height];

		GGen_Script_Assert(return_data != NULL);

		memcpy(return_data, data->data, sizeof(int16) * output_width * output_height);

		return return_data;
		
    } 
    catch (SquirrelError &) {
		return NULL;
    }	
    catch (bad_alloc){
		GGen::GetInstance()->ThrowMessage("GGen_Data memory allocation failed!", GGEN_ERROR, -1);
		return NULL;
    }
}

void GGen_Squirrel::RegisterPreset(GGen_Data_1D* preset, char* label){
	SQChar* in_buf = new SQChar[strlen(label) + 1];

	mbstowcs(in_buf, label, strlen(label));

	in_buf[strlen(label)]=_SC('\0');

	BindVariable(*presetTarget, preset, in_buf, VAR_ACCESS_READ_ONLY);

	delete [] in_buf;
}

void GGen_Squirrel::RegisterPreset(GGen_Data_2D* preset, char* label){
	SQChar* in_buf = new SQChar[strlen(label) + 1];

	mbstowcs(in_buf, label, strlen(label));

	in_buf[strlen(label)]=_SC('\0');

	BindVariable(*presetTarget, preset, in_buf, VAR_ACCESS_READ_ONLY);

	delete [] in_buf;
}

void GGen_Squirrel::RegisterPreset(GGen_Amplitudes* preset, char* label){
	SQChar* in_buf = new SQChar[strlen(label) + 1];

	mbstowcs(in_buf, label, strlen(label));

	in_buf[strlen(label)]=_SC('\0');

	BindVariable(*presetTarget, preset, in_buf, VAR_ACCESS_READ_ONLY);

	delete [] in_buf;
}


