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

#include <iostream>
#include <vector>
#include <string>
#include <wchar.h>

#ifdef GGEN_DLL
	#include "../../include/geogen.h"
#elif defined(GGEN_LIB)
	#include "../../include/geogen.h"
#else
	#include "../../src/ggen.h"
	#include "../../src/ggen_squirrel.h"
#endif


#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

class ArgDesc{
    struct Argument{
        GGen_Char short_name;
        GGen_String long_name;
        GGen_String desc;
        GGen_String param_name;
        char type;
        void* var;        
    };

    enum NextAction{
        UNKNOWN,
        E_INT,
        E_STR
    };

    enum ArgType{
        T_BOOL,
        T_INT,
        T_STR
    };


    std::vector<GGen_String> args_in;
    std::vector<Argument> args_def;
    std::vector<GGen_String>* pos_args;

public:

    ArgDesc(int argc, char** argv){
#ifdef GGEN_UNICODE		
		// convert the input parameters to unicode
		wchar_t** wargv = new wchar_t*[argc];

		for(int i = 0; i < argc; i++){
			wargv[i] = new wchar_t[strlen(argv[i]) + 1];

			mbstowcs(wargv[i], argv[i], strlen(argv[i]));

			wargv[i][strlen(argv[i])] = '\0';
		}

		// convert the unwieldy array into vector
        args_in = std::vector<GGen_String>(wargv, wargv + argc);

		for(int i = 0; i < argc; i++){
			delete [] wargv[i];
		}

		delete [] wargv;
#else
		// convert the unwieldy array into vector
		args_in = std::vector<GGen_String>(argv, argv + argc);
#endif
    }

    void AddBoolArg(char short_name, GGen_String long_name, GGen_String desc, bool* var){
        Argument p;
        p.short_name = short_name;
        p.long_name = long_name;
        p.desc = desc;
        p.type = T_BOOL;
        p.var = (void*) var;

        args_def.push_back(p);
    }

    void AddStringArg(char short_name, GGen_String long_name, GGen_String desc, GGen_String param_name, GGen_String* var){
        Argument p;
        p.short_name = short_name;
        p.long_name = long_name;
        p.desc = desc;
        p.param_name = param_name;
        p.type = T_STR;
        p.var = (void*) var;

        args_def.push_back(p);
    }

    void AddIntArg(char short_name, GGen_String long_name, GGen_String desc, GGen_String param_name, int* var){
        Argument p;
        p.short_name = short_name;
        p.long_name = long_name;
        p.param_name = param_name;
        p.desc = desc;
        p.type = T_INT;
        p.var = (void*) var;

        args_def.push_back(p);
    }
    
    void SetPosArgsVector(std::vector<GGen_String>& pos_args){
		this->pos_args = &pos_args;
    }

    bool Scan(){
        NextAction next_action = UNKNOWN;
        void* next_var = NULL;
        unsigned int offset = 0;

        for(unsigned i = 1; i < args_in.size(); i++){
            //expecting integer
            if(next_action == E_INT){
				*(int*) next_var = (int) GGen_Strtol(args_in[i].c_str(), NULL, 10);

                next_action = UNKNOWN;
                continue;
            }

            //expecting string
            else if(next_action == E_STR){
                *(GGen_String*) next_var = args_in[i];

                next_action = UNKNOWN;
                continue;
            }
            
            // arg named by its long name
            else if(args_in[i][0] == '-' && args_in[i][1] == '-'){
                GGen_String name = args_in[i].substr(2, args_in[i].length() - 2);
                
                for(unsigned j = 0; j < args_def.size(); j++){                
                    if(name == args_def[j].long_name){
                        if(args_def[j].type == T_BOOL){
                            *(bool*) args_def[j].var = true;
                        }
                        else if(args_def[j].type == T_INT){
                            next_action = E_INT;
                            next_var = args_def[j].var;
                        }
                        else if(args_def[j].type == T_STR){
                            next_action = E_STR;
                            next_var = args_def[j].var;
                        }
                    }
                }
            }
            // arg named by its short name
            else if(args_in[i][0] == '-'){
                GGen_Char name = args_in[i].substr(1 + offset, args_in[i].length() - 1 - offset).at(0);
                
                for(unsigned j = 0; j < args_def.size(); j++){                
                    if(name == args_def[j].short_name){
                        if(args_def[j].type == T_BOOL){
                            *(bool*) args_def[j].var = true;

                            if(offset < args_in[i].length() - 2){
                                offset++;
                                i--;
                            }
                        }
                        else if(args_def[j].type == T_INT){
                            next_action = E_INT;
                            next_var = args_def[j].var;

                            offset = 0;
                        }
                        else if(args_def[j].type == T_STR){
                            next_action = E_STR;
                            next_var = args_def[j].var;

                            offset = 0;
                        }
                    }
                }
            }
            else{
				if(pos_args != NULL) pos_args->push_back(args_in[i]);
            }
        }

        if(next_action != UNKNOWN) return false;

        return true;
    }
    
    void PrintHelpString(){
		for(unsigned i = 0; i < args_def.size(); i++){
			std::GGen_Cout << "       -" << args_def[i].short_name;
			if(args_def[i].type != T_BOOL) GGen_Cout << " " << args_def[i].param_name;
			
			std::GGen_Cout << ", --" << args_def[i].long_name;
			if(args_def[i].type != T_BOOL) GGen_Cout << " " << args_def[i].param_name;
			
			std::GGen_Cout << std::endl;
			
			std::GGen_Cout << "	      " << args_def[i].desc << std::endl << std::endl;
		}
    }
};