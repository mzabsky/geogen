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

// system headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
#include <math.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <time.h>

///#include <vld.h>

using namespace std;

// hide "use our better secured function" warning
#pragma warning(disable: 4996)

// GGen inclusion mode (dynamically linked library / statically linked library / inline)
#ifdef GGEN_DLL
	#ifdef _DEBUG
		#pragma comment(lib,"../bin/GeoGenD.lib")
	#else
		#pragma comment(lib,"../bin/GeoGen.lib")
	#endif

	#include "../include/geogen.h"
#elif defined(GGEN_LIB)
	#ifdef _DEBUG
		#pragma comment(lib,"../lib/GeoGenD.lib")
	#else
		#pragma comment(lib,"../lib/GeoGen.lib")
	#endif

	#include "../include/geogen.h"
#else
	#include "ggen.h"
	#include "ggen_squirrel.h"
#endif

#include "../external/EasyBMP/EasyBMP.h"
#include "../external/ArgDesc/ArgDesc.cpp"

struct OutputFormat{
	GGen_String suffix;
	string name;
	long long int min;
	long long int max;
	bool apply_overlay;
};

#define NUM_FORMATS 3 // overlay virtual formats are not included

OutputFormat _formats[] = {
	{GGen_Const_String("bmp"), "Windows Bitmap", 0, 255, true},
	{GGen_Const_String("shd"), "GeoGen Short Data", -(2 << 14) + 1, (2 << 14) - 1, false},
	{GGen_Const_String("pgm"), "Portable Gray Map", 0, 2 << 13, false},
	{GGen_Const_String("bmp"), "Overlay", 0, 255, true},
	{GGen_Const_String("bmp"), "Ext overlay", -255, 254, true}
};

struct GGen_Params{
	OutputFormat* output_format;

	GGen_String input_file;
	GGen_String output_file;
	GGen_String output_directory;
	GGen_String overlay_file;
	
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
	
	vector<GGen_String> script_args;
	
	GGen_Params()
		:output_format(&_formats[0]),
		input_file(GGen_Const_String("")),
		output_file(GGen_Const_String("../temp/out.bmp")),
		output_directory(GGen_Const_String("../temp/")),
		overlay_file(GGen_Const_String("")),
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

bool Save(const short* data, unsigned int width, unsigned int height, const GGen_String* implicit_path, const GGen_String* name = NULL, bool enable_overlay = false){
	bool deleteData = false;

	// overlay is to be saved separately -> create its name	
	if(_params.overlay_as_copy && _params.overlay_file.length() > 0 && !enable_overlay){
		GGen_String buf;
		if(name != NULL){
			buf = GGen_String(*name) + GGen_Const_String("_overlay");
		}
		else {
			buf = GGen_Const_String("out_overlay");
		}
		
		Save(data, width, height, implicit_path, &buf, true);
	}
	else if(!_params.overlay_as_copy && _params.overlay_file.length() > 0){
		enable_overlay = true;
	}

	OutputFormat* format = _params.output_format;

	BMP overlay;
	if(((_params.overlay_as_copy && enable_overlay) || (!_params.overlay_as_copy))  && _params.overlay_file.length() > 0){
#ifdef GGEN_UNICODE
		char* buf = new char[_params.overlay_file.length() + 1];
		wcstombs(buf, _params.overlay_file.c_str(), _params.overlay_file.length());
		buf[_params.overlay_file.length()] = '\0';
#else
		const char* buf = _params.overlay_file.c_str();
#endif

		if(!overlay.ReadFromFile(buf)){
			cout << "Could  not open overlay file!\n" << flush;
			return false;
		}

#ifdef GGEN_UNICODE
		delete [] buf;
#endif		

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
		format_max = (format->max + 1) / 2 - 1;
		format_min = - (format->max + 1) / 2 + 1;
	}

	GGen_String path_out;

	if(name == NULL){
		path_out = *implicit_path;
		GGen_Cout << GGen_Const_String("Saving main bitmap as \"") << *implicit_path << GGen_Const_String("\" ...\n") << flush;
	}
	else if(_params.disable_secondary_maps){
		return false;
	}
	else{

		GGen_String compatible_directory_name(_params.output_directory.length(), GGen_Const_String(' '));
		copy(_params.output_directory.begin(), _params.output_directory.end(), compatible_directory_name.begin());

		path_out += compatible_directory_name + GGen_Const_String("/") + *name + GGen_Const_String(".") + format->suffix;
		GGen_Cout << GGen_Const_String("Saving map \"") << *name << GGen_Const_String("\" as \"") << path_out << GGen_Const_String("\"...\n") << flush;
	}
	
	// is scaling wanted?
	if(_params.no_rescaling == false){
		short* new_data = new short[width * height];
		
		// calculate the extremes
		int max = - 2 << 14;
		int min = 2  << 14;
		for(unsigned i = 0; i < width * height; i++){
			if(data[i] > max) max = data[i];
			if(data[i] < min) min = data[i];
		}
		
		// Calculate scaling ratio
		double ratio = 0;

		if (max <= 0){
			ratio = (double) format_min / (double) min;
		} else if (min >= 0){
			ratio = (double) format_max / (double) max;
		} else {
			ratio = 
				((double) format_max / (double) max < (double) format_min / (double) min) || format_min == 0 ? 
				(double) format_max / (double) max :
				(double) format_min / (double) min;
		}

		// if max == min, then whole map is black, scaling would be useless
		if(max - min > 0){
			if(_params.ignore_zero) max = max - min;
			
			for(unsigned i = 0; i < width * height; i++){
				if(_params.ignore_zero) new_data[i] = (short) format_min + (data[i] - min) * (short) format_max / max;
				else if(format_min < 0 || (format_min == 0 && data[i] > 0)) new_data[i] = (short) ((double) data[i] * ratio);
				else new_data[i] = 0;
			}
		
			if(_params.split_range && format->min == 0){
				for(unsigned int i = 0; i < width * height; i++){
					new_data[i] = new_data[i] += (short) ((format->max + 1) / 2);
				}
			}
			
			data = new_data;
			deleteData = true;
		}
	}

#ifdef GGEN_UNICODE
		unsigned len = path_out.length();

		char* path_out_cstr = new char[len + 1];

		wcstombs(path_out_cstr, path_out.c_str(), len);
		path_out_cstr[len] = '\0';
#else
		const char* path_out_cstr = path_out.c_str();
#endif


		if(format->suffix == GGen_Const_String("bmp")){
			BMP output;

			output.SetBitDepth(32);

			output.SetSize(width, height);

			if(_params.overlay_file == GGen_Const_String("") || !enable_overlay){
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

		output.WriteToFile(path_out_cstr);
	}
	else if(format->suffix == GGen_Const_String("shd")){
		int iWidth = width;
		int iHeight = height;

		ofstream out(path_out_cstr, ios_base::out | ios_base::binary | ios::ate);
		
		if(out.bad()){
			GGen_Cout << GGen_Const_String("Could not write ") << path_out << GGen_Const_String("!\n") << flush;
		}
		else{
			out.write((char*) &iWidth, sizeof(iWidth));
			out.write((char*) &iHeight, sizeof(iHeight));
			
			out.write((char*) data, 2 * width * height);
		}
		
		out.close();
	}
	else if(format->suffix == GGen_Const_String("pgm")){
		ofstream out(path_out_cstr, ios_base::out);
		if(out.bad()){
			GGen_Cout << GGen_Const_String("Could not write ") << path_out << GGen_Const_String("!\n") << flush;
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

			out.close();
		}		
	}

#ifdef GGEN_UNICODE
	delete [] path_out_cstr;
#endif

	if(deleteData) delete [] data;

	if(name != NULL) cout << "Executing...\n" << flush;

	return true;
}


template <class T>
T random(T min, T max){
	double random = (double)rand() / (double) RAND_MAX;
	T output = min + (T) (random * (double)(max - min));
	return output;
}

void ReturnHandler(const GGen_String& name, const short* map, int width, int height){
	Save(map, width, height, NULL, &name); 
}

void ProgressHandler(int current_progress, int max_progress){
	cout << (int) (( (double) current_progress / (double) max_progress) * 100) <<  "% Done...\n" << flush;
}

int main(int argc,char * argv[]){
	// initialize argument support
	ArgDesc args(argc, argv);
	args.SetPosArgsVector(_params.script_args);
	
	args.AddStringArg(GGen_Const_String('i'), GGen_Const_String("input"), GGen_Const_String("Input squirrel script to be executed."), GGen_Const_String("FILE"), &_params.input_file); 
	args.AddStringArg(GGen_Const_String('o'), GGen_Const_String("output"), GGen_Const_String("Output file, the extension determines file type of the output (*.bmp for Windows Bitmap, *.shd for GeoGen Short Height Data and *.pgm for Portable Gray Map are allowed). Set to \"../temp/out.bmp\" by default."), GGen_Const_String("FILE"), &_params.output_file);
	args.AddStringArg(GGen_Const_String('d'), GGen_Const_String("output-directory"), GGen_Const_String("Directory where secondary maps will be saved. Set to \"../temp/\" by default."), GGen_Const_String("DIRECTORY"), &_params.output_directory);
	args.AddStringArg(GGen_Const_String('v'), GGen_Const_String("overlay"), GGen_Const_String("Overlay file to be mapped on the output. This file must be a Windows Bitmap file one pixel high and either 256 or 511 pixels wide."), GGen_Const_String("FILE"), &_params.overlay_file);
	
	args.AddIntArg(GGen_Const_String('s'), GGen_Const_String("seed"), GGen_Const_String("Pseudo-random generator seed. Maps generated with same seed, map script, arguments and generator version are always the same."), GGen_Const_String("SEED"), &_params.random_seed);
	
	args.AddBoolArg(GGen_Const_String('a'), GGen_Const_String("all-random"), GGen_Const_String("All unset script arguments are generated randomly."), &_params.all_random);
	args.AddBoolArg(GGen_Const_String('z'), GGen_Const_String("ignore-zero"), GGen_Const_String("Height data range will be rescaled to fit the output file format including negative value. Zero level will probably not be preserved. Allows to fit negative values into format, which doesn't support them (Windows Bitmap)."), &_params.ignore_zero);
	args.AddBoolArg(GGen_Const_String('n'), GGen_Const_String("no-rescaling"), GGen_Const_String("The height data will not be rescaled at all. Might cause color overflows if the format's value range is lower than <-32787, 32787>."), &_params.no_rescaling);
	args.AddBoolArg(GGen_Const_String('?'), GGen_Const_String("help"), GGen_Const_String("Displays this help."), &_params.help);
	args.AddBoolArg(GGen_Const_String('x'), GGen_Const_String("syntax-check"), GGen_Const_String("Will print OKAY if script is compilable or descibe the error found."), &_params.syntax_check_mode);
	args.AddBoolArg(GGen_Const_String('p'), GGen_Const_String("param-list"), GGen_Const_String("Lists the script's parameters in machine-readable format."), &_params.param_list_mode);
	args.AddBoolArg(GGen_Const_String('e'), GGen_Const_String("simple"), GGen_Const_String("Mode which allows all necessary data to be entered interactively. This mode is automatically activaded if no params were entered."), &_params.stupid_mode);
	args.AddBoolArg(GGen_Const_String('m'), GGen_Const_String("manual"), GGen_Const_String("Script arguments will be entered interactively."), &_params.manual_mode);
	args.AddBoolArg(GGen_Const_String('D'), GGen_Const_String("disable-secondary-maps"), GGen_Const_String("All secondary maps will be immediately discarded, ReturnAs calls will be effectively skipped."), &_params.disable_secondary_maps);
	args.AddBoolArg(GGen_Const_String('V'), GGen_Const_String("overlay-as-copy"), GGen_Const_String("Color files with overlays will be saved as copies."), &_params.overlay_as_copy);
	args.AddIntArg( GGen_Const_String('g'), GGen_Const_String("grid"), GGen_Const_String("Renders a grid onto the overlay file."), GGen_Const_String("SIZE"), &_params.grid_size);
	args.AddBoolArg(GGen_Const_String('h'), GGen_Const_String("split-range"), GGen_Const_String("Splits the value range of a file format, which doesn't support negative values, so lower half of the range covers negaive values and upper half covers positive values. Value \"(max + 1) / 2\" will be treated as zero."), &_params.split_range);
	
	
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
	if(_params.input_file == GGen_Const_String("")){
		cout << "Please enter path to a script file: ";
		GGen_Cin >> _params.input_file;
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

#ifdef GGEN_UNICODE
	unsigned len_in = _params.input_file.length();

	char* path_in_cstr = new char[len_in + 1];

	wcstombs(path_in_cstr, _params.input_file.c_str(), len_in);
	path_in_cstr[len_in] = '\0';
#else
	const char* path_in_cstr = _params.input_file.c_str();
#endif

	// load the script from file
	ifstream in_stream;
	//in_stream.imbue(utf8_locale);
	in_stream.open(path_in_cstr);
	
#ifdef GGEN_UNICODE
	delete [] path_in_cstr;
#endif

	if(!in_stream.is_open()){
		cout << "Could not open the script file!\n" << flush;
		if(_params.stupid_mode) system("pause");
		return -1;
	}

	// read the file line by line
	string str, strTotal;
	getline(in_stream,str);
	while ( in_stream ) {
	   strTotal += '\n' + str;
	   getline(in_stream,str);
	}

	in_stream.close();

	
#ifdef GGEN_UNICODE
	// convert the script  to unicode
	GGen_Char* preparedScript = new GGen_Char[strlen(strTotal.c_str()) + 1];

	mbstowcs(preparedScript, strTotal.c_str(), strlen(strTotal.c_str()));

	preparedScript[strlen(strTotal.c_str())] = GGen_Const_String('\0');
#else
	const GGen_Char* preparedScript = strTotal.c_str();
#endif

	// create the primary GeoGen object (use Squirrel script interface)
	GGen_Squirrel* ggen = new GGen_Squirrel();

	cout << "Compiling...\n" << flush;

	ggen->SetReturnCallback(ReturnHandler);
	ggen->SetProgressCallback(ProgressHandler);

	// pump the script into the engine and compile it
	if(!ggen->SetScript(GGen_String(preparedScript))){
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
	vector<GGen_ScriptArg>* script_args = ggen->LoadArgs();
	
	if(script_args == NULL){
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
		for(unsigned i = 0; i < script_args->size(); i++){
			GGen_ScriptArg* current_arg = &(*script_args)[i];
			
			GGen_Char type = GGen_Const_String('N');
			
			switch(current_arg->type){
				case GGEN_BOOL : type = GGen_Const_String('B'); break;
				case GGEN_INT : type = GGen_Const_String('I'); break;
				case GGEN_ENUM : type = GGen_Const_String('E'); break;
			}
			
			GGen_Cout << 
				current_arg->label << GGen_Const_String(";") <<  
				current_arg->description << GGen_Const_String(";") << 
				type << GGen_Const_String(";") << 
				current_arg->default_value << GGen_Const_String(";") << 
				current_arg->min_value << GGen_Const_String(";") << 
				current_arg->max_value << GGen_Const_String(";") << 
				current_arg->step_size << GGen_Const_String(";") << 
				current_arg->options.size() << GGen_Const_String(";");
		
			if(current_arg->type == GGEN_ENUM){
				for(unsigned j = 0; j < current_arg->options.size(); j++){
					if(j > 0) cout << ",";
					GGen_Cout << current_arg->options[j];
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
		for(unsigned i = 0; i < script_args->size(); i++){
			GGen_ScriptArg* current_arg = &(*script_args)[i];

			char* buf = new char[16];

			GGen_Cout << GGen_Const_String("	") << current_arg->label << GGen_Const_String(" (");
			
			if(current_arg->type == GGEN_INT) cout << "integer in range " << current_arg->min_value << "-" << current_arg->max_value << "): ";
			else if(current_arg->type == GGEN_BOOL) cout << "0 = No, 1 = Yes): ";
			else if(current_arg->type == GGEN_ENUM) {
				for(unsigned j = 0; j < current_arg->options.size(); j++){
					if(j > 0) cout << ", ";
					GGen_Cout << j << GGen_Const_String(" = ") << current_arg->options[j];
				}
				cout << "): ";
			}
			
			cin >> buf;

			// use default (or random if in allrandom mode) value if first char of the input is not a  number
			if(buf[0] >= '0' && buf[0] <= '9') {
				if(_params.all_random) current_arg->SetValue(random(current_arg->min_value, current_arg->max_value));
				else current_arg->SetValue(atoi(buf));
			}
			
			delete buf;
		}		
	}
	// auto mode
	else{
		// loop through all the map arguments 
		for(unsigned i = 0; i < script_args->size(); i++){
			GGen_ScriptArg* current_arg = &(*script_args)[i];

			if(i < _params.script_args.size() && _params.script_args[i] != GGen_Const_String("r") && _params.script_args[i] != GGen_Const_String("d")){
				current_arg->SetValue((int) GGen_Strtol(_params.script_args[i].c_str(), NULL, 10));
			}

			// should the value be generated randomly?
			else if(_params.all_random || (i < _params.script_args.size() && _params.script_args[i] == GGen_Const_String("r"))){
				current_arg->SetValue(random(current_arg->min_value, current_arg->max_value));
			}
		}
	}

	clock_t start = clock();
	
	cout << "Executing with seed " << _params.random_seed << "...\n" << flush;

	// execute the main part of the script
	short* data = ggen->Generate();

	if(data == NULL){
		cout << "Map generation failed!\n" << flush;
		delete ggen;
		if(_params.stupid_mode) system("pause");
		return -1;		
	}
	
	assert(data != NULL);

	GGen_String compatible_file_name(_params.output_file.length(), GGen_Const_String(' '));
	copy(_params.output_file.begin(), _params.output_file.end(), compatible_file_name.begin());

	// flush the bitmap
	Save(data, ggen->output_width, ggen->output_height, &compatible_file_name);
	
	delete [] data;

	cout << "Cleanup...\n" << flush;

	delete ggen;

	double seconds = (double) (clock() - start) / (double) CLOCKS_PER_SEC;

	cout << "Finished after " << seconds << " seconds!\n" << flush;

	if(_params.stupid_mode) system("pause");

	//GGen_Data_2D::NumInstances();

	return 0;
}