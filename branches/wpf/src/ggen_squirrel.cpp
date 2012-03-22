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

DECLARE_ENUM_TYPE(GGen_Normalization_Mode);
DECLARE_ENUM_TYPE(GGen_Overflow_Mode);
DECLARE_ENUM_TYPE(GGen_Direction);
DECLARE_ENUM_TYPE(GGen_Message_Level);
DECLARE_ENUM_TYPE(GGen_Voronoi_Noise_Mode);
DECLARE_ENUM_TYPE(GGen_Comparison_Mode);
DECLARE_ENUM_TYPE(GGen_Outline_Mode);

void GGen_ErrorHandler(HSQUIRRELVM,const SQChar * desc,const SQChar * source,SQInteger line,SQInteger column){
    GGen::GetInstance()->ThrowMessage(desc, GGEN_ERROR, line, column);
}

void GGen_PrintHandler(HSQUIRRELVM v,const SQChar * s,...){
    static SQChar temp[2048];
    va_list vl;
    va_start(vl,s);
    GGen_Vsprintf( temp,s,vl);

    va_end(vl);

    GGen::GetInstance()->ThrowMessage(temp, GGEN_ERROR);
}

GGen_Squirrel::GGen_Squirrel(){
    

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
    RegisterGlobal(&GGen_AddIntArg, _SC("GGen_AddIntArg"));
    RegisterGlobal(&GGen_AddBoolArg, _SC("GGen_AddBoolArg"));
    RegisterGlobal(&GGen_AddEnumArg, _SC("GGen_AddEnumArg"));
    RegisterGlobal(&GGen_GetArgValue, _SC("GGen_GetArgValue"));

    /* Progress */
    RegisterGlobal(&GGen_InitProgress, _SC("GGen_InitProgress"));
    RegisterGlobal(&GGen_SetProgress, _SC("GGen_SetProgress"));
    RegisterGlobal(&GGen_IncreaseProgress, _SC("GGen_IncreaseProgress"));

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

    /* Enum: GGen_Comparison_Mode */
    BindConstant(GGEN_EQUAL_TO, _SC("GGEN_EQUAL_TO"));
    BindConstant(GGEN_NOT_EQUAL_TO, _SC("GGEN_NOT_EQUAL_TO"));
    BindConstant(GGEN_LESS_THAN, _SC("GGEN_LESS_THAN"));
    BindConstant(GGEN_GREATER_THAN, _SC("GGEN_GREATER_THAN"));
    BindConstant(GGEN_LESS_THAN_OR_EQUAL_TO, _SC("GGEN_LESS_THAN_OR_EQUAL_TO"));
    BindConstant(GGEN_GREATER_THAN_OR_EQUAL_TO, _SC("GGEN_GREATER_THAN_OR_EQUAL_TO"));

    /* Enum: GGen_Outline_Mode */
    BindConstant(GGEN_INSIDE, _SC("GGEN_INSIDE"));
    BindConstant(GGEN_OUTSIDE, _SC("GGEN_OUTSIDE"));

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
        func(&GGen_Data_2D::CropValues,_T("CropValues")).
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
        func(&GGen_Data_2D::FloodSelect,_T("FloodSelect")).
        func(&GGen_Data_2D::GetMaxValueOnPath,_T("GetMaxValueOnPath")).
        func(&GGen_Data_2D::GetMinValueOnPath,_T("GetMinValueOnPath")).
        func(&GGen_Data_2D::Expand,_T("Expand")).
        func(&GGen_Data_2D::Shrink,_T("Shrink")).
        func(&GGen_Data_2D::ExpandDirection,_T("ExpandDirection")).
        func(&GGen_Data_2D::ShrinkDirection,_T("ShrinkDirection")).
        func(&GGen_Data_2D::Outline,_T("Outline")).
        func(&GGen_Data_2D::ConvexityMap,_T("ConvexityMap")).
        func(&GGen_Data_2D::Distort,_T("Distort"));

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
        func(&GGen_Path::GetMinX,_T("GetMinX")).
        func(&GGen_Path::GetMinY,_T("GetMinY")).
        func(&GGen_Path::GetMaxX,_T("GetMaxX")).
        func(&GGen_Path::GetMaxY,_T("GetMaxY")).
        func(&GGen_Path::GetAverageX,_T("GetAverageX")).
        func(&GGen_Path::GetAverageY,_T("GetAverageY")).
        func(&GGen_Path::GetCenterX,_T("GetCenterX")).
        func(&GGen_Path::GetCenterY,_T("GetCenterY")).
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

    #include "ggen_presets.h"
}

GGen_Squirrel::~GGen_Squirrel(){    
    // delete contents of all presets
    for(list<void*>::iterator it = this->presets.begin(); it != this->presets.end(); it++){
        delete *it;
    }

    // empty the preset list
    this->presets.clear();

    SquirrelVM::Shutdown();    
}


bool GGen_Squirrel::SetScript(const GGen_String& script){
    try {
        SquirrelObject sqScript = SquirrelVM::CompileBuffer(script.c_str());

        SquirrelVM::RunScript(sqScript);

        this->status = GGEN_SCRIPT_LOADED;

        return true;
    } catch (SquirrelError &) {
        return false;
    }
}

GGen_String GGen_Squirrel::GetInfo(const GGen_String& label){
    assert(this->status == GGEN_SCRIPT_LOADED || this->status == GGEN_READY_TO_GENERATE);

    GGen_Status statusBackup = this->status;

    this->status = GGEN_LOADING_MAP_INFO;

    try{
        SquirrelFunction<const SQChar*> callFunc(GGen_Const_String("GetInfo"));

        const SQChar* output = callFunc(label.c_str());

        this->status = statusBackup;

        return GGen_String(output);
    }
    catch(...){
        this->status = statusBackup;

        return NULL;
    }
}

int GGen_Squirrel::GetInfoInt(const GGen_String& label){
    assert(this->status == GGEN_SCRIPT_LOADED || this->status == GGEN_READY_TO_GENERATE);

    GGen_Status statusBackup = this->status;

    this->status = GGEN_LOADING_MAP_INFO;

    try{
        SquirrelFunction<int> callFunc(GGen_Const_String("GetInfo"));

        int value = callFunc(label.c_str());

        this->status = statusBackup;

        return value;
    }
    catch(...){
        this->status = statusBackup;

        return -1;
    }
}

int16* GGen_Squirrel::Generate(){        
    assert(this->status == GGEN_READY_TO_GENERATE);
    
    HSQUIRRELVM v = SquirrelVM::GetVMPtr();

    // remember the current state of the stack (so it can be restored later)
    int top = sq_gettop(v);

    try {
        int16* return_data;
        GGen_Data_2D* data;

        {
            this->status = GGEN_GENERATING;            

            sq_pushroottable(v);

            // name of the function being called
            sq_pushstring(v, GGen_Const_String("Generate"),-1); 
            
            // does the Generate() function exist?
            if(SQ_SUCCEEDED(sq_get(v,-2))) {
                // push "this" (since Generate() is supposed to be a global function use root table)
                sq_pushroottable(v);

                // invoke the function
                sq_call(v, 1, true, true);                            

                // the output instance
                SQUserPointer outInstance;

                // did the function return an instance of a GGen_Data_2D class?
                if(SQ_SUCCEEDED(sq_getinstanceup(v, -1, &outInstance, ClassType<GGen_Data_2D>::type()))){
                    data = (GGen_Data_2D*) outInstance;
                }
                else {
                    throw SquirrelError();
                }
            }
            else {
                GGen::GetInstance()->ThrowMessage(GGen_Const_String("\"Generate()\" function was not found in the script!"), GGEN_ERROR, -1);

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
        this->GetInfoInt(GGen_Const_String("voidCall"));

        /* Free all remaining 2D instances (those created via Clone) */
        GGen_Data_2D::FreeAllInstances();

        return return_data;        
    } 
    catch (SquirrelError &) {        
        GGen_Data_2D::FreeAllInstances();        

        this->status = GGEN_READY_TO_GENERATE;

        return NULL;
    }
    catch (GGen_ScriptAssertException &) {
        GGen_Data_2D::FreeAllInstances();

        this->status = GGEN_READY_TO_GENERATE;

        return NULL;
    }
    catch (bad_alloc){
        GGen_Data_2D::FreeAllInstances();

        this->status = GGEN_READY_TO_GENERATE;
        
        GGen::GetInstance()->ThrowMessage(GGen_Const_String("GGen_Data memory allocation failed!"), GGEN_ERROR, -1);

        return NULL;
    }
}

void GGen_Squirrel::RegisterPreset(GGen_Data_1D* preset, const GGen_String& label){
    BindVariable(preset, label.c_str(), VAR_ACCESS_READ_ONLY);

    this->presets.push_back(preset);
}

void GGen_Squirrel::RegisterPreset(GGen_Data_2D* preset, const GGen_String& label){
    BindVariable(preset, label.c_str(), VAR_ACCESS_READ_ONLY);

    this->presets.push_back(preset);
}

void GGen_Squirrel::RegisterPreset(GGen_Amplitudes* preset, const GGen_String& label){
    BindVariable(preset, label.c_str(), VAR_ACCESS_READ_ONLY);

    this->presets.push_back(preset);
}


