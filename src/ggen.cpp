/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

// system and SDL headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
//#include <SDL/SDL.h> 
#include <math.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include "ggen_support.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"

#include "ggen.h"
#include "ggen_squirrel.h"

GGen_Amplitudes* ggen_std_noise;

GGen::GGen(){
	callback = NULL;

	ggen_std_noise = new GGen_Amplitudes(16384);

	ggen_std_noise->AddAmplitude(1, 3);
	ggen_std_noise->AddAmplitude(2, 7);
	ggen_std_noise->AddAmplitude(4, 10);
	ggen_std_noise->AddAmplitude(8, 20);
	ggen_std_noise->AddAmplitude(16, 50);
	ggen_std_noise->AddAmplitude(32, 75);
	ggen_std_noise->AddAmplitude(64, 150);
	ggen_std_noise->AddAmplitude(128, 250);
	ggen_std_noise->AddAmplitude(256, 400);
	ggen_std_noise->AddAmplitude(512, 600);
	ggen_std_noise->AddAmplitude(1024, 1000);
	ggen_std_noise->AddAmplitude(2048, 1400);
	ggen_std_noise->AddAmplitude(4096, 2000);
	ggen_std_noise->AddAmplitude(8192, 2900);
	ggen_std_noise->AddAmplitude(16384, 4000);
}

GGen::~GGen(){
	delete ggen_std_noise;
}

void GGen::ThrowMessage(char* message, GGen_Message_Level level, int line, int column){
	//message = "asdasda"; 
	
	if(callback != NULL){
		callback(message, level, line, column);
	}
	else{

		switch(level){
			case GGEN_MESSAGE:
				cout << "GGen Message: " << message << " on line " << line << "\n";
				break;
			case GGEN_NOTICE:
				cout << "GGen Notice: " << message << " on line " << line << "\n";
				break;
			case GGEN_WARNING:
				cout << "GGen Warning: " << message << " on line " << line << "\n";
				break;
			case GGEN_ERROR:
				cout << "GGen Error: " << message << " on line " << line << "\n";
				break;
		}

		//assert(buf != NULL);

		

		//delete buf;
	}
}

void GGen::ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line, int column){

	int len = wcslen(message);

	char* buf = new char[len + 1];

	wcstombs(buf, message, len + 1); 
	
	buf[len] = '\0';

	ThrowMessage(buf, level);
}

void GGen::SetMessageCallback( void (*callback) (char* message, GGen_Message_Level, int line, int column) ){
	this->callback = callback;
}


static void printFunc(HSQUIRRELVM v,const SQChar * s,...) {
  /*static SQChar temp[2048];
  va_list vl;
  va_start(vl,s);
  scvsprintf( temp,s,vl);
  SCPUTS(temp);
  va_end(vl);*/
} // pri