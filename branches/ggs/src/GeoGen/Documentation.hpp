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
}

/// @mainpage
/// This is documentation of GeoGen's public API.
/// @section API usage
/// All public symbols are declared in the header GeoGen.hpp. 
/// 
/// A simple code snippet which uses a map script to generate a height map then looks like this:
/// @snippet BasicUsage.cpp Body
/// Further reading:
///	@li @link ErrorHandling.cpp Handling compilation and runtime errors @endlink
///	@li @link ScriptParameters.cpp Setting script parameters @endlink
/// @li @link RenderProgress.cpp Displaying render progress @endlink
///	@li @link MultipleTiles.cpp Rendering multiple tiles of a single map @endlink

/// @example BasicUsage.cpp
/// This example demonstrates the simplest usage scenario.

/// @example ErrorHandling.cpp
/// This example demonstrates how handle compilation and runtime errors. All error cause an @link geogen::GeoGenException exception @endlink to be thrown and can be handled using the usual try-catch statements.

/// @example ScriptParameters.cpp
/// This example demonstrates how to access any parameters declared by the script.

/// @example RenderProgress.cpp
/// This example demonstrates how display render progress to the user.

/// @example MultipleTiles.cpp
/// This example demonstrates how slice a map into multiple tiles.

