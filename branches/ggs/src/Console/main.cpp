#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/compiler/Compiler.hpp"
#include <iomanip>
#include <fstream>

#include <windows.h>
#include "..\GeoGen\utils\StringUtils.hpp"

#include "CommandTable.hpp"

#include "Loader.hpp"
#include "ConsoleUtils.hpp"
#include "ProgramArguments.hpp"
#include "ArgDesc.hpp"

using namespace geogen;
using namespace console;
using namespace compiler;
using namespace runtime;
using namespace random;
using namespace std;

int main(int argc, char** argv){
	ProgramArguments programArguments;
	programArguments.seed = IntToString((int)time(0));

	// initialize argument support
	ArgDesc args(argc, argv);
	//args.SetPosArgsVector(_params.script_args);

	args.AddStringArg(GG_STR('i'), GG_STR("input"), GG_STR("Input script to be executed."), GG_STR("FILE"), &programArguments.inputFile);
	args.AddStringArg(GG_STR('o'), GG_STR("output"), GG_STR("Output file, the extension determines file type of the output (*.bmp for Windows Bitmap, *.shd for GeoGen Short Height Data and *.pgm for Portable Gray Map are allowed). Set to \"../temp/out.bmp\" by default."), GG_STR("FILE"), &programArguments.outputDirectory);
	//args.AddStringArg(GG_STR('d'), GG_STR("output-directory"), GG_STR("Directory where secondary maps will be saved. Set to \"../temp/\" by default."), GG_STR("DIRECTORY"), &_params.output_directory);
	//args.AddStringArg(GG_STR('v'), GG_STR("overlay"), GG_STR("Overlay file to be mapped on the output. This file must be a Windows Bitmap file one pixel high and either 256 or 511 pixels wide."), GG_STR("FILE"), &_params.overlay_file);

	args.AddStringArg(GG_STR('s'), GG_STR("seed"), GG_STR("Pseudo-random generator seed. Maps generated with same seed, map script, arguments and generator version are always the same."), GG_STR("SEED"), &programArguments.seed);
	args.AddBoolArg(GG_STR('n'), GG_STR("noninteractive"), GG_STR("Non-interactive mode."), &programArguments.isNonInteractive);

	//args.AddBoolArg(GG_STR('a'), GG_STR("all-random"), GG_STR("All unset script arguments are generated randomly."), &_params.all_random);
	//args.AddBoolArg(GG_STR('z'), GG_STR("ignore-zero"), GG_STR("Height data range will be rescaled to fit the output file format including negative value. Zero level will probably not be preserved. Allows to fit negative values into format, which doesn't support them (Windows Bitmap)."), &_params.ignore_zero);
	//args.AddBoolArg(GG_STR('n'), GG_STR("no-rescaling"), GG_STR("The height data will not be rescaled at all. Might cause color overflows if the format's value range is lower than <-32787, 32787>."), &_params.no_rescaling);
	args.AddBoolArg(GG_STR('?'), GG_STR("help"), GG_STR("Displays this help."), &programArguments.displayHelp);
	//args.AddBoolArg(GG_STR('x'), GG_STR("syntax-check"), GG_STR("Will print OKAY if script is compilable or descibe the error found."), &_params.syntax_check_mode);
	//args.AddBoolArg(GG_STR('e'), GG_STR("simple"), GG_STR("Mode which allows all necessary data to be entered interactively. This mode is automatically activaded if no params were entered."), &_params.stupid_mode);
	//args.AddBoolArg(GG_STR('m'), GG_STR("manual"), GG_STR("Script arguments will be entered interactively."), &_params.manual_mode);
	//args.AddBoolArg(GG_STR('D'), GG_STR("disable-secondary-maps"), GG_STR("All secondary maps will be immediately discarded, ReturnAs calls will be effectively skipped."), &_params.disable_secondary_maps);
	//args.AddBoolArg(GG_STR('V'), GG_STR("overlay-as-copy"), GG_STR("Color files with overlays will be saved as copies."), &_params.overlay_as_copy);
	//args.AddIntArg(GG_STR('g'), GG_STR("grid"), GG_STR("Renders a grid onto the overlay file."), GG_STR("SIZE"), &_params.grid_size);
	//args.AddBoolArg(GG_STR('h'), GG_STR("split-range"), GG_STR("Splits the value range of a file format, which doesn't support negative values, so lower half of the range covers negaive values and upper half covers positive values. Value \"(max + 1) / 2\" will be treated as zero."), &_params.split_range);

	args.Scan();

	if (programArguments.displayHelp)
	{
		args.PrintHelpString();
		return 0;
	}

	Loader loader(Cin, Cout, programArguments);
	loader.Run();

	return 0;
}
