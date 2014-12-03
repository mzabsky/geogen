#pragma once

/// Wraps all GeoGen features. Contains basic functions and classes used throughout the entire code-base.
namespace geogen
{
	/// Contains script parser and compiler.
	namespace compiler
	{
	}

	/// Contains definitions of the entire standard library as it is exposed to scripts.
	namespace corelib
	{
	}

	/// Contains low-level native implementations of generator objects (height maps and height profiles) and all associated map operations.
	namespace genlib
	{
	}

	/// Contains pseudo-random number generation features.
	namespace random
	{
	}

	/// Contains the renderer and all associated classes.
	namespace renderer
	{
	}

	/// Contains the virtual machine and all associated classes.
	namespace runtime
	{
	}

	/// Contains additional functions exposed to functions in testing mode.
	namespace testlib
	{
	}

	/// Contains assorted utility classes and functions used throughout the project.
	namespace utils
	{
	}


	/// @example BasicUsage.cpp 
	/// @example ErrorHandling.cpp
	/// @example ScriptParameters.cpp
	/// @example AdvancedScriptParameters.cpp
	/// @example RenderProgress.cpp
	/// @example TextMessages.cpp
	/// @example MultipleTiles.cpp


	/// @mainpage Getting started
	/// 		  
	/// The @link tutorial_basic_usage basic usage tutorial@endlink demonstrates how to include GeoGen in your project and how to generate first map using a map script. Basic knowledge of map script writing and usage of the generator is assumed.
	/// 
	/// Further tutorials:
	/// @li @link tutorial_error_handling Handling compilation and runtime errors @endlink
	/// @li @link tutorial_script_parameters Setting script parameters @endlink
	/// @li @link tutorial_render_progress Displaying render progress @endlink
	/// @li @link tutorial_text_messages Handling text messages from scripts @endlink
	/// @li @link tutorial_multiple_tiles Rendering multiple tiles of a single map @endlink

	/// @page tutorials Tutorials
	/// This page contains a list of tutorials on GeoGen API usage.
	///
	/// @li @link tutorial_basic_usage Basic usage @endlink
	/// @li @link tutorial_error_handling Handling compilation and runtime errors @endlink
	/// @li @link tutorial_script_parameters Setting script parameters @endlink
	/// @li @link tutorial_render_progress Displaying render progress @endlink
	/// @li @link tutorial_text_messages Handling text messages from scripts @endlink
	/// @li @link tutorial_multiple_tiles Rendering multiple tiles of a single map @endlink

	/// @page tutorial_basic_usage Basic usage
	/// This tutorial demonstrates how to get started with GeoGen.
	/// 
	/// First, directory GeoGen/include has to be added to your include paths. This line of code will then give you access to all of GeoGen's public symbols:
	/// @snippet BasicUsage.cpp Include
	/// 		 
	/// Following code sample demonstrates how to use the generator to render a map using a map script:
	/// @snippet BasicUsage.cpp Body
	/// @link BasicUsage.cpp Full code @endlink

	/// @page tutorial_error_handling Handling compilation and runtime errors
	/// This tutorial demonstrates how to handle compilation and runtime errors. 
	/// All errors cause an @link GeoGenException exception @endlink to be thrown and can be handled using the usual try - catch statements. Generally, the compiler::Compiler throws exceptions that are subclasses of compiler::CompilerException with @link ErrorCode error codes @endlink 1xxx and the runtime::VirtualMachine throws exceptions that are subclasses of runtime::RuntimeException with @link ErrorCode error codes @endlink 2xxx. Additionally, more generic exceptions such as InternalErrorException and ApiUsageException may be thrown by almost any call, indicating severe error in the generator itself or in the hosting application.
	/// @snippet ErrorHandling.cpp Body
	/// @link BasicUsage.cpp Full code @endlink
	/// 	 
	/// If an exception is thrown during runtime of the runtime::VirtualMachine the machine will enter @link runtime::VIRTUAL_MACHINE_STATUS_FAULTED faulted state@endlink which will block any further execution calls such as runtime::VirtualMachine::Run and runtime::VirtualMachine::Step. Information gathering calls such as runtime::VirtualMachine::GetObjectStack will still work though.

	/// @page tutorial_script_parameters Setting script parameters
	/// This tutorial demonstrates how to access and set any parameters declared by the script.
	/// 
	/// Following code sets map dimensions and a single map parameter:
	/// @snippet ScriptParameters.cpp Body
	/// @link ScriptParameters.cpp Full code @endlink
	/// 	 
	/// Each parameter can be of one of three types: runtime::BooleanScriptParameter, runtime::NumberScriptParameter and runtime::EnumScriptParameter.
	/// The actual type can be determined by calling @link runtime::ScriptParameter::GetType GetType@endlink on the base class.
	///  
	/// Next code sample is a bit more complex, iterating over all available script parameters and setting their value interactively using standard input.
	/// @snippet AdvancedScriptParameters.cpp Body
	/// @link AdvancedScriptParameters.cpp Full code @endlink
	/// 
	/// The SetValue method of each of the parameter types performs input validations and attempts to make the value valid if possible (such as number value greater than the maximum will be reduced to the maximum). It that attempt fails, default value is used.

	/// @page tutorial_render_progress Displaying render progress
	/// This tutorial demonstrates how to display render progress to the user in real-time.
	/// 
	/// @snippet RenderProgress.cpp Body
	/// @link RenderProgress.cpp Full code @endlink

	/// @page tutorial_text_messages Handling text messages from scripts
	/// This tutorial demonstrates how to handle text messages produced by the map scripts (using the Print function).
	/// 
	/// Text messages are handled using a callback of type runtime::VirtualMachine::ScriptMessageHandler. A simple implementation, which just prints the message to standard output:
	/// @snippet TextMessages.cpp Callback
	/// @link TextMessages.cpp Full code @endlink
	/// 	  
	/// The handler is then assigned to a runtime::VirtualMachine using runtime::VirtualMachine::SetScriptMessageHandler:
	/// @snippet TextMessages.cpp Body
	/// @link TextMessages.cpp Full code @endlink
	/// 	 
	/// If no custom text message handler is specified, runtime::VirtualMachine::DefaultScriptMessageHandler, which prints the message on standard output, is used. A runtime::VirtualMachine::EmptyScriptMessageHandler, which discards the message instead, is also provided.
	///  
	/// In addition to the ready-to-print formatted text, the unformatted text with the collection of formatting arguments is also provided. This can for example be used to replace the unformatted message with its localized variant and then splice the formatting arguments into it using FormatString.
	///  
	/// If you need to pass any data to the callback, you can assign any data pointer to the VM using runtime::VirtualMachine::SetCallbackData and then retrieve it in the callback using runtime::VirtualMachine::GetCallbackData.

	/// @page tutorial_multiple_tiles Rendering multiple tiles of a single map
	/// This tutorial demonstrates how to cut a single map into multiple tiles.
	/// @snippet MultipleTiles.cpp Body
	/// @link MultipleTiles.cpp Full code @endlink
	/// 	 
	/// Whether a map is infinite can be determined by calling runtime::ScriptParameters::IsMapInfinite.
	



	/// @page source_code Source code
	/// This page describes structure of source code of GeoGen.
	/// 
	/// The code is organized into following top-level projects:
	/// 
	/// - **antlr3** - C code of the [ANTLR 3 runtime](http://www.antlr3.org/). Builds as a static library.
	/// - **Console** - C++ code of the command line interface. References antlr3, GeoGen, lpng1612 and zlib128. Builds as an executable.  
	/// - **examples** - C++ code samples demonstrating usage of the GeoGen library. 
	/// - **GeoGen** - C++ code of the height map generator library itself. References antlr3. Buils as a static library.  
	/// - **GeoGen_Tests** - A suite of automated C++ tests for the generator library. References antlr3, GeoGen, lpng1612 and zlib128. Builds as an executable.  
	/// - **lpng1612** - C code of [libpng](http://www.libpng.org/pub/png/libpng.html). Builds as a static library.
	/// - <b>png++</b> - C++ code of [png++](http://www.nongnu.org/pngpp/), a C++ wrapper for libpng. Headers only (does not build).  
	/// - **ScriptingDocumentation** - Code of the scripting reference. Compiled using Doxygen.  
	/// - **zlib128** - C code of [zlib](http://www.zlib.net/). Builds as static library.  
	/// 
	/// The copy of the source code can be checked out from public online SVN repository at `http://geogen.googlecode.com/svn/branches/ggs`.

}