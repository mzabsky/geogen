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

/** 
 * @file ggen.h Core file containing the GGen base class.
 **/


/**
 * @mainpage GeoGen - API Reference
 * <p>This reference contains listings of all GeoGen objects, functions and constants available from within of map scripts.</p>
 * <p>In addition to these functions, you can use several native Squirrel functions and whole <a href="http://squirrel-lang.org/doc/sqstdlib2.html#d0e1519" target="blank">sqstdmath library</a></p>.
 * <hr>
 * <h2>Contents</h2>
 * <ul>
 *	<li><a href="globals_func.html">Global functions</a></li>
 *	<li><a href="globals_enum.html">Enumerations</a></li>
 *	<li><a href="globals_type.html">Typedefs</a></li>
 *	<li><a href="globals_defs.html">Constants</a></li>
 *	<li><a href="annotated.html">Objects</a><ul>
 *		<li>GGen_Data_1D</li>
 *		<li>GGen_Data_2D</li>
 *		<li>GGen_Amplitudes</li>
 *		<li>GGen_Point</li>
 *		<li>GGen_Path</li>
 *	</ul></li>
 * </ul>
 **/

#pragma once

#include "ggen_support.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"
#include "ggen_scriptarg.h"

class GGen;

class GGEN_EXPORT GGen{
protected: 
	static GGen* instance;

	GGen_Status status;
public:
	void (*message_callback) (const GGen_String& message, GGen_Message_Level, int line, int column);
	void (*return_callback) (const GGen_String& name, const int16* map, int width, int height);
	void (*progress_callback) (int current_progress, int max_progress);

	vector<GGen_ScriptArg> args;

	uint16 output_width, output_height;

	uint32 max_progress, current_progress;

	GGen_Size max_map_size;
	uint16 max_map_count;

	GGen();
	virtual ~GGen();

	static GGen* GetInstance();

	const GGen_Status GetStatus();

	const void ThrowMessage(const GGen_String& message, GGen_Message_Level level, int line = -1, int column = -1);
	//void ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line = -1, int column = -1);

	void SetMessageCallback( void (*message_callback) (const GGen_String& message, GGen_Message_Level, int line, int column));
	void SetReturnCallback( void (*return_callback) (const GGen_String& name, const int16* map, int width, int height) );
	void SetProgressCallback( void (*return_callback) (int current_progress, int max_progress));
	
	virtual bool SetScript(const GGen_String& script) = 0;
	virtual GGen_String GetInfo(const GGen_String& label) = 0;
	virtual int GetInfoInt(const GGen_String& label) = 0;
	virtual vector<GGen_ScriptArg>* LoadArgs();
	virtual int16* Generate() = 0;

    virtual void Reset();
	
	void SetMaxMapSize(GGen_Size size);
	void SetMaxMapCount(uint16 count);

	/* Constraint getters and progress methods must be static to be exported as globals to Squirrel */
	static GGen_Size GetMaxMapSize();
	static uint16 GetMaxMapCount();
	
	virtual void RegisterPreset(GGen_Data_1D* preset, const GGen_String& label) = 0;
	virtual void RegisterPreset(GGen_Data_2D* preset, const GGen_String& label) = 0;
	virtual void RegisterPreset(GGen_Amplitudes* preset, const GGen_String& label) = 0;

	void SetSeed(unsigned seed);
};