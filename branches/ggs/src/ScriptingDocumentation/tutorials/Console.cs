/// @page tutorial_console Tutorial: Using the command line interface
/// This tutorial shows how to run GeoGen from the command line.
///
/// The `geogen` executable is located in the `bin` directory in the GeoGen's main directory. Starting this program will enter the interactive console and wait for a command.
///
/// @image html tut_console_empty.png
/// 
/// Command `load` (shorthand `l`) can be used to load a script:
/// 
/// @verbatim load myScript.ggs@endverbatim
/// 
/// This command loads the script and compiles it (and prints any compilation errors if necessary). Command `run` (`r`) can then be used to execute the script. A script can be executed as many times as you wish.
/// 
/// @image html tut_console_run.png
/// 
/// Command `rr` reloads any modifications made to the last loaded script and runs it (effectively combining above two commands into one, accelerating script development).
///
///
/// @section console_script_parameters Script parameters
///
/// Following commands can be used to adjust execution of the scripts:
///
/// Command name | Cue/shorthand       | Description
/// -------------|---------------------|--------------------------------------------------------
/// Render size  | @verbatim rs [width] [height]@endverbatim | Sets render size of the map. If only one argument is passed, that number is used as both width and height. Use "auto" to let the program determine the size automatically (default value of 1000 will be used, usually).
/// Render origin | @verbatim ro [x] [y]@endverbatim | Sets position of the render origin. If no arguments are passed, displays current render origin. [0, 0] is used if not set. Use "auto" to let the program determine the size automatically (default value of 1000 will be used, usually).
/// Render scale | @verbatim rsc [scale]@endverbatim | Sets render scale (0.5 = 2x zoomed out, 2.0 = 2x zoomed in). 1 is used if not set.
/// Map size     | @verbatim ms [width] [height]@endverbatim | Sets reference size of the map. If only one argument is passed, that number is used as both width and height. Use "auto" to let the program determine the size automatically (render size will be used, usually). If the map is infinite, this will be ignored.
/// Random seed  | @verbatim rseed [seed]@endverbatim | Sets random seed. If the seed is a number, that number will be used. If it is a string, a hash of that string will be used.
/// Parameter    | @verbatim param [name] [value]@endverbatim | Sets value of a script parameter with given name. If the script doesn't support parameter with that name, it will be ignored.
///
/// All of these commands will print the current value of that attribute when invoked without any arguments.
/// 
/// Values set with all these commands will remain effective until changed once again with that command (throughout multiple execution of the script and even after loading another script).
/// 	   
/// @section console_debugger Debugging scripts
/// Instead of executing the script with `run`, `debug` (`d`) can be used to enter the script debugger.
///
/// @image html tut_console_debugger.png
/// 
/// When debugger is started, the program enters a debugger "context" (the `LOADER>>` is now replaced with `DEBUGGER>>`) which supports a completely different suite of commands. When the debugger starts, it is waiting before first instruction of the script. Command 'step' (or just hitting "enter") can be used to advance execution by one instruction. 'run' ('r') will keep advancing execution until an error is encountered or end of script is reached.
/// 
/// Variety of commands can be used to study current state of execution:
/// Command name  | Cue/shorthand          | Description
/// --------------|------------------------|---------------------------------------------------------------------
/// Object stack  | @verbatim os@endverbatim | Prints current object stack.
/// Call stack  | @verbatim cs@endverbatim | Prints current call stack.
/// Read variable | @verbatim read [name]@endverbatim | Prints value of variable with given name. Uses normal @link variables variable name resolution mechanism@endlink from current location of execution.
/// Rendering sequence | @verbatim rs@endverbatim | Prints current rendering sequence.
/// Managed objects | @verbatim mo@endverbatim | Prints list of all alive managed objects along with their IDs and reference counts.
/// 
/// The debugger can be terminated at any time using the `quit` (`q`) command or by pressing CTRL+C. The program will the go to back to the loader.
///
/// When the script finishes executing successfully it will automatically switch to the renderer debugger.
/// 
/// @section console_renderer Renderer debugger
///
/// Renderer debugger allows manual control over the rendering process. It works similar to the script debugger, except each `step` (or enter) advances the process by one rendering step instead of one instruction (`run`/`r` also works). Most other commands are different than in the loader or debugger context though.
/// 
/// @image html tut_console_renderer.png
///
/// State of the renderer can be studied using these commands:
/// Command name  | Cue/shorthand          | Description
/// --------------|------------------------|---------------------------------------------------------------------
/// Rendering sequence | @verbatim rs@endverbatim | Prints the rendering sequence.
/// Rendering sequence metadata  | @verbatim rsm@endverbatim | Prints rendering sequence metadata, including object liveness ranges and rendering bounds for each step.
/// Object table | @verbatim ot@endverbatim | Prints information about each slot in the object table.
/// Save image | @verbatim save [slot]@endverbatim | Saves object from given object table slot to the hard drive. It will be saved as a PNG file (or CSV file if it is an HeightProfile).
/// Managed objects | @verbatim mo@endverbatim | Prints list of all alive managed objects along with their IDs and reference counts.
/// Enable/disable autosave | @verbatim as [0/1]@endverbatim | Enables (if 1) or disables (if 0) automatic saving of the affected @ref HeightMap/@ref HeightProfile to the hard drive after each rendering step. These objects will be saved as they are in the object table, including areas which are outside rendering bounds of the executed rendering step (and may therefore be meaningless). 

