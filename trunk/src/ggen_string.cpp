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

#include "sqplus.h"


#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen.h"

char* GGen_ToCString(const wchar_t* string){
	char* out_buf = new char[wcslen(string) + 1];

	wcstombs(out_buf, string, wcslen(string));

	out_buf[wcslen(string)] = '\0';

	return out_buf;
}

char* GGen_ToCString(const char* string){
	char* out_buf = new char[strlen(string) + 1];

	strcpy(out_buf, string);

	return out_buf;
}

char* GGen_ToCString(const SqPlus::sq_std_string &string){
	char* buf = new char[string.length() + 1];

	wcstombs(buf, string.c_str(), string.length());

	buf[string.length()] = '\0';

	return buf;
}