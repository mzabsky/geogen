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

#pragma once;

#include <iostream>

#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"

#include "ggen.h"
#include "ggen_squirrel.h"

GGen_Amplitudes* ggen_std_noise;
GGen* ggen_current_object;

GGen::GGen(){
	message_callback = NULL;
	return_callback = NULL;
	progress_callback = NULL;
	
	//post_callback = NULL;

	max_progress = current_progress = 0;

	//args = NULL;
	num_args = 0;

	ggen_std_noise = new GGen_Amplitudes(4096);

	ggen_std_noise->AddAmplitude(1, 3 * 15);
	ggen_std_noise->AddAmplitude(2, 7 * 15);
	ggen_std_noise->AddAmplitude(4, 10 * 15);
	ggen_std_noise->AddAmplitude(8, 20 * 15);
	ggen_std_noise->AddAmplitude(16, 50 * 15);
	ggen_std_noise->AddAmplitude(32, 75 * 15);
	ggen_std_noise->AddAmplitude(64, 150 * 15);
	ggen_std_noise->AddAmplitude(128, 250 * 15);
	ggen_std_noise->AddAmplitude(256, 400 * 15);
	ggen_std_noise->AddAmplitude(512, 600 * 15);
	ggen_std_noise->AddAmplitude(1024, 1000 * 15);
	ggen_std_noise->AddAmplitude(2048, 1400 * 15);
	ggen_std_noise->AddAmplitude(4096, 2000 * 15);
}

GGen::~GGen(){
	delete ggen_std_noise;
}

void GGen::ThrowMessage(char* message, GGen_Message_Level level, int line, int column){
	
	if(message_callback != NULL){
		message_callback(message, level, line, column);
	}
	else{
		switch(level){
			case GGEN_MESSAGE:
				if(line != -1) cout << "GGen Message: " << message << " on line " << line << "\n" << flush;
				else cout << "GGen Message: " << message <<  "\n";
				break;
			case GGEN_NOTICE:
				if(line != -1) cout << "GGen Notice: " << message << " on line " << line << "\n" << flush;
				else cout << "GGen Notice: " << message <<  "\n";
				break;
			case GGEN_WARNING:
				if(line != -1) cout << "GGen Warning: " << message << " on line " << line << "\n" << flush;
				else cout << "GGen Warning: " << message <<  "\n";
				break;
			case GGEN_ERROR:
				if(line != -1) cout << "GGen Error: " << message << " on line " << line << "\n" << flush;
				else cout << "GGen Error: " << message <<  "\n" << flush;
				break;
		}
	}

}

void GGen::SetMessageCallback( void (*message_callback) (char* message, GGen_Message_Level, int line, int column) ){
	this->message_callback = message_callback;
}

void GGen::SetReturnCallback( void (*return_callback) (char* name, const int16* map, int width, int height) ){
	this->return_callback = return_callback;
}

void GGen::SetProgressCallback( void (*progress_callback) (int current_progress, int max_progress) ){
	this->progress_callback = progress_callback;
}

GGen_ScriptArg** GGen::LoadArgs(){
	// free current array of args
	if(args != NULL){
		for(uint8 i = 0; i < num_args; i++){
			delete args[i];
		}

		//delete [] *args;
	}

	GetInfoInt("args");

	return args;
}

void GGen::ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line, int column){

	int len = wcslen(message);

	char* buf = new char[len + 1];

	wcstombs(buf, message, len + 1); 
	
	buf[len] = '\0';

	ThrowMessage(buf, level, line, column);
}

void GGen::InitProgress(uint32 max_progress){
	ggen_current_object->max_progress = max_progress;
	
	if(ggen_current_object->progress_callback != NULL) ggen_current_object->progress_callback(0, max_progress);
}

void GGen::SetProgress(uint32 current_progress){
	GGen_Script_Assert(current_progress <= ggen_current_object->max_progress);
	
	ggen_current_object->current_progress = current_progress;

	if(ggen_current_object->progress_callback != NULL) ggen_current_object->progress_callback(current_progress, ggen_current_object->max_progress);
}

void GGen::IncreaseProgress(){
	GGen_Script_Assert(ggen_current_object->current_progress + 1 < ggen_current_object->max_progress);
	
	ggen_current_object->current_progress++;
	
	if(ggen_current_object->progress_callback != NULL) ggen_current_object->progress_callback(ggen_current_object->current_progress, ggen_current_object->max_progress);
}

void GGen::InitPresets(){
	#include "ggen_presets.h"
}