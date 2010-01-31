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
#include <time.h>

using namespace std;

#ifdef NO_GGEN_LIB
	#include "ggen.h"
	#include "ggen_squirrel.h"
#else
	#pragma comment(lib,"../lib/GeoGen.lib")
	#include "../include/geogen.h"
#endif

#include "EasyBMP.h"
#include "../external/ArgDesc/ArgDesc.cpp"


struct OutputFormat{
	string suffix;
	string name;
	long long int min;
	long long int max;
	bool apply_overlay;
};

#define NUM_FORMATS 3 // overlay virtual formats are not included

OutputFormat _formats[] = {
	{"bmp", "Windows Bitmap", 0, 255, true},
	{"shd", "GeoGen Short Data", -(2 << 14) + 1, (2 << 14) - 1, false},
	{"pgm", "Portable Gray Map", 0, 2 << 13, false},
	{"bmp", "Overlay", 0, 255, true},
	{"bmp", "Ext overlay", -255, 254, true}
};

struct GGen_Params{
	OutputFormat* output_format;

	string input_file;
	string output_file;
	string output_directory;
	string overlay_file;
	
	int random_seed;
	
	bool all_random;
	bool no_rescaling;
	bool ignore_zero;
	bool help;
	bool syntax_check_mode;
	bool param_list_mode;
	bool stupid_mode;
	bool manual_mode;
	bool disable_secondary_maps;
	bool overlay_as_copy;
	int grid_size;
	bool split_range;
	
	vector<std::string> script_args;
	
	GGen_Params()
		:output_format(&_formats[0]),
		input_file(""),
		output_file("../temp/out.bmp"),
		output_directory("../temp/"),
		overlay_file(""),
		random_seed(-1),
		all_random(false),
		no_rescaling(false),
		ignore_zero(false),
		help(false),
		syntax_check_mode(false),
		param_list_mode(false),
		stupid_mode(false),
		manual_mode(false),
		disable_secondary_maps(false),
		overlay_as_copy(false),
		grid_size(0),
		split_range(false)
	{}
};

GGen_Params _params;

bool Save(const int16* data, unsigned int width, unsigned int height, const char* implicit_path, const char* name = NULL, bool enable_overlay = false){
	// overlay is to be saved separately -> create its name	
	if(_params.overlay_as_copy && _params.overlay_file.length() > 0 && !enable_overlay){
		char* buf;
		if(name != NULL){
			buf = new char[strlen(name) + 9];
			buf[0] = '\0';
			buf = strcat(buf, name);
			buf = strcat(buf, "_overlay");
		}
		else {
			buf = "out_overlay";
		}
		
		Save(data, width, height, implicit_path, buf, true);
	}
	else if(!_params.overlay_as_copy && _params.overlay_file.length() > 0){
		enable_overlay = true;
	}

	OutputFormat* format = _params.output_format;

	BMP overlay;
	if(((_params.overlay_as_copy && enable_overlay) || (!_params.overlay_as_copy))  && _params.overlay_file.length() > 0){
		if(!overlay.ReadFromFile(_params.overlay_file.c_str())){
			cout << "Could  not open overlay file!\n" << flush;
			return false;
		}
		
		if(overlay.TellWidth() == 256) format = &_formats[NUM_FORMATS];
		else if(overlay.TellWidth() == 511) format = &_formats[NUM_FORMATS + 1];
		else{
			cout << "Incorrect overlay size!" << endl;
			return false;
		}
	}

	long long int format_max = format->max;
	long long int format_min = format->min;
	
	if(_params.split_range && format->min == 0){
		format_max = (format->max + 1) / 2;
		format_min = - (format->max + 1) / 2 - 1;
	}

	stringstream path_out;

	if(name == NULL){
		path_out << implicit_path;
		cout << "Saving main bitmap as \"" << implicit_path << "\" ...\n" << flush;
	}
	else if(_params.disable_secondary_maps){
		return false;
	}
	else{
		path_out << _params.output_directory << name << "." << format->suffix;
		cout << "Saving map \"" << name << "\" as \"" << path_out.str() <<"\"...\n" << flush;
	}
	
	// is scaling wanted?
	if(_params.no_rescaling == false){
		int16* new_data = new int16[width * height];
		
		// calculate the extremes
		int32 max = - 2 << 14;
		int32 min = 2  << 14;
		for(uint32 i = 0; i < width * height; i++){
			if(data[i] > max) max = data[i];
			if(data[i] < min) min = data[i];
		}
		
		/* Calculate scaling ratio */
		double ratio = 0;

		if (max <= 0){
			ratio = (double) format_min / (double) min;
		} else if (min >= 0){
			ratio = (double) format_max / (double) max;
		} else {
			ratio = 
				(double) format_max / (double) max < (double) format_min / (double) min ? 
				(double) format_max / (double) max :
				(double) format_min / (double) min;
		}

		// if max == min, then whole map is black, scaling would be useless
		if(max - min > 0){
			if(_params.ignore_zero) max = max - min;
			
			for(uint32 i = 0; i < width * height; i++){
				if(_params.ignore_zero) new_data[i] = (int16) format_min + (data[i] - min) * (int16) format_max / max;
				else if(format_min < 0 || (format_min == 0 && data[i] > 0)) new_data[i] = (int16) ((double) data[i] * ratio);
				else new_data[i] = 0;
			}
		
			if(_params.split_range && format->min == 0){
				for(unsigned int i = 0; i < width * height; i++){
					new_data[i] = new_data[i] += (int16) ((format->max + 1) / 2);
				}
			}
			
			data = new_data;
		}
	}

	if(format->suffix == "bmp"){
		BMP output;

		output.SetBitDepth(32);

		output.SetSize(width, height);

		if(_params.overlay_file == "" || !enable_overlay){
			for(unsigned int i = 0; i < height; i++){
				for(unsigned int j = 0; j < width; j++){
					output(j,i)->Red = output(j,i)->Green = output(j,i)->Blue = (ebmpBYTE) data[j + width * i];
				}		
			}
		}
		else{
			for(unsigned int i = 0; i < height; i++){
				for(unsigned int j = 0; j < width; j++){
					if(_params.grid_size > 1 && (j % _params.grid_size == 0 || i % _params.grid_size == 0)){
						output(j,i)->Red = 128;
						output(j,i)->Green = 128;
						output(j,i)->Blue = 128;					
						
						continue;
					}
				
					int xInOverlay = data[j + width * i];
					
					if(overlay.TellWidth() == 511) xInOverlay += 255;
				
					output(j,i)->Red = overlay(xInOverlay ,0)->Red;
					output(j,i)->Green = overlay(xInOverlay ,0)->Green;
					output(j,i)->Blue = overlay(xInOverlay ,0)->Blue;
				}		
			}
		}

		output.WriteToFile(path_out.str().c_str());
	}
	else if(format->suffix == "shd"){
		int iWidth = width;
		int iHeight = height;

		ofstream out(path_out.str().c_str(), ios_base::out | ios_base::binary | ios::ate);
		
		if(out.bad()){
			cout << "Could not write " << path_out.str() << "!\n" << flush;
		}
		else{
			out.write((char*) &iWidth, sizeof(iWidth));
			out.write((char*) &iHeight, sizeof(iHeight));
			
			out.write((char*) data, 2 * width * height);
		}
		
		out.close();
	}
	else if(format->suffix == "pgm"){
		ofstream out(path_out.str().c_str(), ios_base::out);
		if(out.bad()){
			cout << "Could not write " << path_out.str() << "!\n" << flush;
		}
		else{
			out << "P2" << endl;
			out << width << " " << height << endl << _params.output_format->max << endl;
			
			for(unsigned y = 0; y < height; y++){
				for(unsigned x = 0; x < width; x++){
					out << data[x + width * y];
					
					if(x < width - 1) out << " ";
				}
				if(y < height - 1) out << endl;
			}
		}		
	}

	if(name != NULL) cout << "Executing...\n" << flush;

	return true;
}


template <class T>
T random(T min, T max){
	double random = (double)rand() / (double) RAND_MAX;
	T output = min + (T) (random * (double)(max - min));
	return output;
}

void ReturnHandler(char* name, const int16* map, int width, int height){
	Save(map, width, height, "", name); 
}

void ProgressHandler(int current_progress, int max_progress){
	cout << (int) (( (double) current_progress / (double) max_progress) * 100) <<  "% Done...\n" << flush;
}

int main(int argc,char * argv[]){
	// initialize argument support
	ArgDesc args(argc, argv);
	args.SetPosArgsVector(_params.script_args);
	
	args.AddStringArg('i', "input", "Input squirrel script to be executed.", "FILE", &_params.input_file); 
	args.AddStringArg('o', "output", "Output file, the extension determines file type of the output (*.bmp for Windows Bitmap, *.shd for GeoGen Short Height Data and *.pgm for Portable Gray Map are allowed). Set to \"../temp/out.bmp\" by default.", "FILE", &_params.output_file);
	args.AddStringArg('d', "output-directory", "Directory where secondary maps will be saved. Set to \"../temp/\" by default.", "DIRECTORY", &_params.output_directory);
	args.AddStringArg('v', "overlay", "Overlay file to be mapped on the output. This file must be a Windows Bitmap file one pixel high and 256 pixels wide.", "FILE", &_params.overlay_file);
	
	args.AddIntArg('s', "seed", "Pseudo-random generator seed. Maps generated with same seed, map script, arguments and generator version are always the same.", "SEED", &_params.random_seed);
	
	args.AddBoolArg('a', "all-random", "All unset script arguments are generated randomly.", &_params.all_random);
	args.AddBoolArg('z', "ignore-zero", "Height data range will be rescaled to fit the output file format including negative value. Zero level will probably not be preserved. Allows to fit negative values into format, which doesn't support them (Windows Bitmap).", &_params.ignore_zero);
	args.AddBoolArg('n', "no-rescaling", "The height data will not be rescaled at all. Might cause color overflows if the format's value range is lower than <-32787, 32787>.", &_params.no_rescaling);
	args.AddBoolArg('?', "help", "Displays this help.", &_params.help);
	args.AddBoolArg('x', "syntax-check", "Will print OKAY if script is compilable or descibe the error found.", &_params.syntax_check_mode);
	args.AddBoolArg('p', "param-list", "Lists the script's parameters in machine-readable format.", &_params.param_list_mode);
	args.AddBoolArg('e', "simple", "Mode which allows all necessary data to be entered interactively. This mode is automatically activaded if no params were entered.", &_params.stupid_mode);
	args.AddBoolArg('m', "manual", "Script arguments will be entered interactively.", &_params.manual_mode);
	args.AddBoolArg('D', "disable-secondary-maps", "All secondary maps will be immediately discarded, ReturnAs calls will be effectively skipped.", &_params.disable_secondary_maps);
	args.AddBoolArg('V', "overlay-as-copy", "Color files with overlays will be saved as copies.", &_params.overlay_as_copy);
	args.AddIntArg( 'g', "grid", "Renders a grid onto the overlay file.", "SIZE", &_params.grid_size);
	args.AddBoolArg('h', "split-range", "Splits the value range of a file format, which doesn't support negative values, so lower half of the range covers negaive values and upper half covers positive values. Value \"(max + 1) / 2\" will be treated as zero.", &_params.split_range);
	
	
	// read the arguments
	args.Scan();

	// display help?
	if(_params.help){
		cout << "GeoGen - procedural heightmap generator.\n"
			 << "\n"
			 << "Usage:\n"
			 << "       geogen -i path_to_script -o path_to_output [options] [script_arguments]\n"
			 << "\n"
			 << "Example:\n"
			 << "       geogen -i \"../examples/basic.nut\" -o \"../../my_folder/map.bmp\" -zD 2000 2000\n"
			 << "\n"
			 << "Option list:"
			 << "\n";
			 
		args.PrintHelpString();
		
		cout << flush;
		return 0;
	}
	
	cout << "Initializing...\n" << flush;

	// no arguments->perhaps the executable was launched directly from window manager->engage stupid mode
	if(argc == 1){
		_params.stupid_mode = true;
	}

	// no input file -> ask the user
	if(_params.input_file == ""){
		cout << "Please enter path to a script file: ";
		cin >> _params.input_file;
	}

	// let the window manager user know where the output goes
	if(_params.stupid_mode){
		cout << "Output will be saved as ./out.bmp\n";
	}
	
	for(int i = 0; i < NUM_FORMATS; i++){
		if(_params.output_file.length() < 5) continue;
		else if(_params.output_file.substr(_params.output_file.length() - 3, 3) == _formats[i].suffix){
			_params.output_format = &_formats[i];
			break;
		}
	}	
	
	// load the script from file
	ifstream in_stream;
	in_stream.open(_params.input_file.c_str());

	if(!in_stream.is_open()){
		cout << "Could not open the script file!\n" << flush;
		if(_params.stupid_mode) system("pause");
		return -1;
	}

	// read the file line by line
	string str,strTotal;
	getline(in_stream,str);
	while ( in_stream ) {
	   strTotal += "\n" + str;
	   getline(in_stream,str);
	}

	in_stream.close();

	// create the primary GeoGen object (use Squirrel script interface)
	GGen_Squirrel* ggen = new GGen_Squirrel();

	cout << "Compiling...\n" << flush;

	ggen->SetReturnCallback(ReturnHandler);
	ggen->SetProgressCallback(ProgressHandler);

	// pump the script into the engine and compile it
	if(!ggen->SetScript(strTotal.c_str())){
		cout << "Compilation failed!\n" << flush;
		delete ggen;
		if(_params.stupid_mode) system("pause");
		return -1;
	}
	else if(_params.syntax_check_mode){
		cout << "OKAY\n" << flush;
		
		delete ggen;
		return 0;
	}

	cout << "Loading map info...\n" << flush;

	// fetch the list of arguments from the script file
	if(ggen->LoadArgs() == NULL){
		cout << "Could not retrieve map information!\n" << flush;
		delete ggen;
		if(_params.stupid_mode) system("pause");
		return -1;		
	}

	// make sure the random seed is prepared
	if(_params.random_seed == -1){
		// no seed was entered -> random random seed
		_params.random_seed = (int) time(0);
	}

	// let the std c++ generator use our seed
	srand(_params.random_seed);

	// param list mode
	if(_params.param_list_mode){
		cout << "PARAMS\n" << flush;
		for(uint8 i = 0; i < ggen->num_args; i++){
			GGen_ScriptArg* a = ggen->args[i];
			
			char type = 'N';
			
			switch(a->type){
				case GGEN_BOOL : type = 'B'; break;
				case GGEN_INT : type = 'I'; break;
				case GGEN_ENUM : type = 'E'; break;
			}
			
			cout << a->label << ";" <<  a->description << ";" << type << ";" << a->default_value << ";" << a->min_value << ";" << a->max_value << ";" << a->step_size << ";" << a->num_options << ";";
		
			if(a->type == GGEN_ENUM){
				for(int j = 0; j < a->num_options; j++){
					if(j > 0) cout << ",";
					cout << a->options[j];
				}
			}
			
			cout << "\n" << flush;
		}
		
		cout << "END\n" << flush;
				
		delete ggen;
		return 0;
	}
	// manual/stupid mode
	else if(_params.manual_mode || _params.stupid_mode){
		cout << "	Please set map parameters:\n";
		
		// loop through all the map arguments 
		for(uint8 i = 0; i < ggen->num_args; i++){
			GGen_ScriptArg* a = ggen->args[i];

			char* buf = new char[16];

			cout << "	" << ggen->args[i]->label << " (";
			
			if(ggen->args[i]->type == GGEN_INT) cout << "integer in range " << ggen->args[i]->min_value << "-" << ggen->args[i]->max_value << "): ";
			else if(ggen->args[i]->type == GGEN_BOOL) cout << "0 = No, 1 = Yes): ";
			else if(ggen->args[i]->type == GGEN_ENUM) {
				for(int j = 0; j < ggen->args[i]->num_options; j++){
					if(j > 0) cout << ", ";
					cout << j << " = " << ggen->args[i]->options[j];
				}
				cout << "): ";
			}
			
			cin >> buf;

			// use default (or random if in allrandom mode) value if fist char of the input is not number
			if(buf[0] >= '0' && buf[0] <= '9') {
				if(_params.all_random) ggen->args[i]->SetValue(random(ggen->args[i]->min_value, ggen->args[i]->max_value));
				else ggen->args[i]->SetValue(atoi(buf));
			}
			
			delete buf;
		}		
	}
	// auto mode
	else{
		// loop through all the map arguments 
		for(uint8 i = 0; i < ggen->num_args; i++){

			if(i < _params.script_args.size() && _params.script_args[i] != "r" && _params.script_args[i] != "d"){
				ggen->args[i]->SetValue(atoi(_params.script_args[i].c_str()));
			}

			// should the value be generated randomly?
			else if(_params.all_random || (i < _params.script_args.size() && _params.script_args[i] == "r")){
				ggen->args[i]->SetValue(random(ggen->args[i]->min_value, ggen->args[i]->max_value));
			}
		}
	}

	clock_t start = clock();
	
	cout << "Executing with seed " << _params.random_seed << "...\n" << flush;

	// execute the main part of the script
	int16* data = ggen->Generate();

	if(data == NULL){
		cout << "Map generation failed!\n" << flush;
		delete ggen;
		if(_params.stupid_mode) system("pause");
		return -1;		
	}
	
	assert(data != NULL);

	// flush the bitmap
	Save(data, ggen->output_width, ggen->output_height, _params.output_file.c_str());

	cout << "Cleanup...\n" << flush;

	delete ggen;

	double seconds = (double) (clock() - start) / (double) CLOCKS_PER_SEC;

	cout << "Finished after " << seconds << " seconds!\n" << flush;

	if(_params.stupid_mode) system("pause");

	return 0;
}