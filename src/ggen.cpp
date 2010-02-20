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

GGen* GGen::instance = NULL;

GGen::GGen(){
	assert(GGen::instance == NULL);

	GGen::instance = this;

	/* Default map constraints to max values (given int is unsigned, -1 overflows to its max value) */
	this->max_height = -1;
	this->max_width = -1;
	this->max_map_count = -1;

	this->message_callback = NULL;
	this->return_callback = NULL;
	this->progress_callback = NULL;

	this->max_progress = this->current_progress = 0;

	this->num_args = 0;
}

GGen::~GGen(){
	GGen::instance = NULL;
}

GGen* GGen::GetInstance(){
	return GGen::instance;
}

void GGen::ThrowMessage(char* message, GGen_Message_Level level, int line, int column){
	
	if(message_callback != NULL){
		this->message_callback(message, level, line, column);
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
		
		num_args = 0;
	}

	if(GetInfoInt("args") == -1) return NULL;

	return args;
}

void GGen::ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line, int column){

	int len = wcslen(message);

	char* buf = new char[len + 1];

	wcstombs(buf, message, len + 1); 
	
	buf[len] = '\0';

	ThrowMessage(buf, level, line, column);
	
	delete [] buf;
}

void GGen::SetMaxWidth(GGen_Size width){
	this->max_width = width;
}

void GGen::SetMaxHeight(GGen_Size height){
	this->max_height = height;
}

void GGen::SetMaxMapCount(uint16 count){
	this->max_map_count = count;
}

GGen_Size GGen::GetMaxWidth(){
	return GGen::GetInstance()->max_width;
}

GGen_Size GGen::GetMaxHeight(){
	return GGen::GetInstance()->max_height;
}

uint16 GGen::GetMaxMapCount(){
	return GGen::GetInstance()->max_map_count;
}


void GGen::InitProgress(uint32 max_progress){
	GGen::GetInstance()->max_progress = max_progress;
	
	if(GGen::GetInstance()->progress_callback != NULL) GGen::GetInstance()->progress_callback(0, max_progress);
}

void GGen::SetProgress(uint32 current_progress){
	GGen_Script_Assert(current_progress <= GGen::GetInstance()->max_progress);
	
	GGen::GetInstance()->current_progress = current_progress;

	if(GGen::GetInstance()->progress_callback != NULL) GGen::GetInstance()->progress_callback(current_progress, GGen::GetInstance()->max_progress);
}

void GGen::IncreaseProgress(){
	GGen_Script_Assert(GGen::GetInstance()->current_progress + 1 < GGen::GetInstance()->max_progress);
	
	GGen::GetInstance()->current_progress++;
	
	if(GGen::GetInstance()->progress_callback != NULL) GGen::GetInstance()->progress_callback(GGen::GetInstance()->current_progress, GGen::GetInstance()->max_progress);
}