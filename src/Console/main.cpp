#include <iomanip>
#include <fstream>

#include <GeoGen/GeoGen.hpp>

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
	args.AddStringArg(GG_STR('s'), GG_STR("seed"), GG_STR("Pseudo-random generator seed. Maps generated with same seed, map script, arguments and generator version are always the same."), GG_STR("SEED"), &programArguments.seed);
	args.AddBoolArg(GG_STR('n'), GG_STR("noninteractive"), GG_STR("Non-interactive mode."), &programArguments.isNonInteractive);
	args.AddBoolArg(GG_STR('?'), GG_STR("help"), GG_STR("Displays this help."), &programArguments.displayHelp);

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
