#include <iostream>
#include <vector>
#include <string>
#include <wchar.h>

#include "../GeoGen/String.hpp"

#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

namespace geogen
{
	namespace console
	{
		class ArgDesc{
			struct Argument{
				Char shortName;
				String longName;
				String desc;
				String paramName;
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


			std::vector<String> argsIn;
			std::vector<Argument> argsDef;
			std::vector<String>* posArgs;

		public:

			ArgDesc(int argc, char** argv){
#ifdef GEOGEN_WCHAR
				std::vector<String> argsInNarrowChar = std::vector<String>(argv, argv + argc);
				
				for(std::vector<String>::const_iterator it = argsInNarrowChar.brgin(); it != argsInNarrowChar.end(); it++)
				{
					argsIn.push_back(AnyToString(*it));
				}
#else
				// convert the unwieldy array into vector
				argsIn = std::vector<String>(argv, argv + argc);
#endif
			}

			void AddBoolArg(char shortName, String longName, String desc, bool* var){
				Argument p;
				p.shortName = shortName;
				p.longName = longName;
				p.desc = desc;
				p.type = T_BOOL;
				p.var = (void*)var;

				argsDef.push_back(p);
			}

			void AddStringArg(char shortName, String longName, String desc, String paramName, String* var){
				Argument p;
				p.shortName = shortName;
				p.longName = longName;
				p.desc = desc;
				p.paramName = paramName;
				p.type = T_STR;
				p.var = (void*)var;

				argsDef.push_back(p);
			}

			void AddIntArg(char shortName, String longName, String desc, String paramName, int* var){
				Argument p;
				p.shortName = shortName;
				p.longName = longName;
				p.paramName = paramName;
				p.desc = desc;
				p.type = T_INT;
				p.var = (void*)var;

				argsDef.push_back(p);
			}

			void SetPosArgsVector(std::vector<String>& pos_args){
				this->posArgs = &pos_args;
			}

			bool Scan(){
				NextAction next_action = UNKNOWN;
				void* next_var = NULL;
				unsigned int offset = 0;

				for (unsigned i = 1; i < argsIn.size(); i++){
					//expecting integer
					if (next_action == E_INT){
						*(int*)next_var = StringToInt(argsIn[i].c_str());

						next_action = UNKNOWN;
						continue;
					}

					//expecting string
					else if (next_action == E_STR){
						*(String*)next_var = argsIn[i];

						next_action = UNKNOWN;
						continue;
					}

					// arg named by its long name
					else if (argsIn[i][0] == '-' && argsIn[i][1] == '-'){
						String name = argsIn[i].substr(2, argsIn[i].length() - 2);

						for (unsigned j = 0; j < argsDef.size(); j++){
							if (name == argsDef[j].longName){
								if (argsDef[j].type == T_BOOL){
									*(bool*)argsDef[j].var = true;
								}
								else if (argsDef[j].type == T_INT){
									next_action = E_INT;
									next_var = argsDef[j].var;
								}
								else if (argsDef[j].type == T_STR){
									next_action = E_STR;
									next_var = argsDef[j].var;
								}
							}
						}
					}
					// arg named by its short name
					else if (argsIn[i][0] == '-'){
						Char name = argsIn[i].substr(1 + offset, argsIn[i].length() - 1 - offset).at(0);

						for (unsigned j = 0; j < argsDef.size(); j++){
							if (name == argsDef[j].shortName){
								if (argsDef[j].type == T_BOOL){
									*(bool*)argsDef[j].var = true;

									if (offset < argsIn[i].length() - 2){
										offset++;
										i--;
									}
								}
								else if (argsDef[j].type == T_INT){
									next_action = E_INT;
									next_var = argsDef[j].var;

									offset = 0;
								}
								else if (argsDef[j].type == T_STR){
									next_action = E_STR;
									next_var = argsDef[j].var;

									offset = 0;
								}
							}
						}
					}
					else{
						if (posArgs != NULL) posArgs->push_back(argsIn[i]);
					}
				}

				if (next_action != UNKNOWN) return false;

				return true;
			}

			void PrintHelpString(){
				for (unsigned i = 0; i < argsDef.size(); i++){
					Cout << "       -" << argsDef[i].shortName;
					if (argsDef[i].type != T_BOOL) Cout << " " << argsDef[i].paramName;

					Cout << ", --" << argsDef[i].longName;
					if (argsDef[i].type != T_BOOL) Cout << " " << argsDef[i].paramName;

					Cout << std::endl;

					Cout << "	      " << argsDef[i].desc << std::endl << std::endl;
				}
			}
		};
	}
}