/// @page script_structure Script structure
/// Each script is fully contained within a single text file, referencing or including other script files is not possible. The script is composed from script metadata, the actual script body and user function definitions.
/// 
/// Script metadata contain declarations of script parameters and declaration of map size limits. Additionally, this section contains information which can be used by the client application to present the script to its users, including script name, description and any other values, which the script can define and the application can choose to read them. Metadata section is completely optional (all metadata entries will be replaced with appropriate default values if the entry is not defined by the script).
/// 
/// Script body contains the actual code where script execution starts (equivalent of "main" method from other programming languages).
/// 
/// Example of a script with simple metadata and trivial body:
/// 
/// @code {.cs}
/// metadata 
/// {
///     Name: "Example script",
///	    Width: { Min: 128 },
///	    Height: { Min: 128 },
///     Parameters:
///     {
///	        Smoothness: { Name: "Smoothness", Description: "..."}
///     }
/// }
///
/// var map = HeightMap.Empty();
/// yield map;
/// @endcode
///         
