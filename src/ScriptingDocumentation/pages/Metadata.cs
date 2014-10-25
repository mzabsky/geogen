/// @page metadata Script metadata
/// Script metadata can provide arbitrary information to the script host. Some of these information is assigned special meaning by the compiler (such as script parameter definitions etc.).
/// 
/// Script metadata is declared with the `metadata` keyword:
/// @code{.cs}
/// metadata
/// {
///	    Name: "A nice name"
/// }
/// @endcode
/// Script metadata is declared using a tree-like structure of nodes. The node types are:
/// Type       | Description                  | Example                       |
/// -----------|------------------------------|-------------------------------|
/// Number	   | Number literal.	          | @code9.2@endcode              |
/// String	   | String literal.	          | @code"hello world!"@endcode   |
/// Boolean	   | Boolean literal.	          | @code true @endcode           |
/// Identifier | Symbol name (the script host will determine how interpret it). Not enclosed in quotes and must follow rules for identifiers. |  @code Infinite @endcode |
/// List	   | List of arbitrary nodes. The items may be considered to be ordered by the script host.	| @code{"a", "b", "c"}@endcode |
/// Key-value collection | Comma-separated list of key-value pairs enclosed in curly brackets, where the key is an identifier and the value is an arbitrary node. The pairs are not ordered (the script host will always see them in alphabetical order).	| @code{Key1: "a", Key2: 5}@endcode |
/// 
/// The root node must always be a key-value collection. An empty collection (`{}`) is considered to be a key-value collection, not a list.
/// 
/// Example:
/// @code{.cs}
/// metadata 
/// {
///	    Name: "Metadata example",
///	    Author: "Isaac Newton",
///	    Tags: {"Pretty", "Cool"},
///	    Width: {Min: 100, Max: 1000},
///	    Height: {Min: 100, Max: 1000}
/// }
/// @endcode
/// The metadata block must be located before the actual script body. Enum and function definitions may be located before script metadata. The script may only contain a single metadata block. The script is not required to have a metadata block at all. The host will ignore any unknown nodes.
/// 
/// @section map_size Map width and height
/// Script metadata is used to specify the range of map sizes supported by the map script.
/// 
/// Map dimensions are specified by nodes `Width` and `Height` located directly under the metadata root. These nodes can be either an identifier named `Infinite` (meaning that the map will be infinite in that direction) or `Finite` (the map will be finite in that direction). A key-value collection with following sub-nodes can also be provided (automatically making the map finite):
/// 
/// Name        | Type          | Description                                                                                                       |
/// ------------|---------------|-------------------------------------------------------------------------------------------------------------------|
/// Default     | Number        | Default size of the map in given dimension. The runtime will choose a default value if not specified.             |
/// Min         | Number        | Minimum size of the map in given dimension. Must be an integer greater than 0. If not specified, 1 will be used.  |
/// Max	        | Number        | Maximum size of the map in given dimension. If not specified, the maximum size supported by the generator will be used. |
/// 
/// If a `Width`/`Height` node is not present at all, the map will be considered to be infinite in that dimension by default.
/// 
/// Example:
/// @code{.cs}
/// metadata 
/// {
///     Width: Infinite,
///	    Height: {Min: 100, Max: 1000, Default: 500}
/// }
/// @endcode
///      
/// @section script_parameters Script parameters
/// Script can use metadata to declare parameters, which can be configured by the script host. All script parameters are contained in the `Parameters` node located under the root node. If the `Parameters` node is not present the script will not have any parameters.
/// 
/// Each parameter is declared with a key-value collection node under the `Parameters` node. The parameter’s node can have following sub-nodes:
/// 
/// Name	      | Type            | Description
/// --------------|-----------------|-----------------------------------------------------------------------------------------------
/// Label         | String          | Short human-friendly name of the parameter to be displayed in the hosting application.
/// Description   | String          | Longer human-friendly description of the parameter to be displayed in the hosting application.
/// Type          | Identifier      | Specifies type of the parameter, can be either Number, Boolean or a reference to an enum type (either defined by the API or by the script).
/// Default	      | Number/Boolean/Identifier | Default value of the parameter. Its type must correspond to value of the `Type` node. If the `Type` of the parameter is an enum, the `Default` is an identifier referencing one of the enum's values.
/// Min           | Number          | Minimum value of the parameter. Only applies to parameters of type Number.
/// Max	          | Number          | Maximum value of the parameter. Only applies to parameters of type Number.
/// Restriction   | Identifier      | Restriction applied on values of the parameter. Only applies to parameters of type Number. See below for list of allowed values. If not specified, `Unrestricted` is used. Min and Max must match this restriction.
/// 
/// Allowed values for `Restriction` are:
/// 
/// Name         | Description
/// -------------|-------------------------------------------------------------------------
/// Unrestricted | No restriction is applied on the values.
/// Integers     | Only integers are allowed.
/// PowersOf2    | Only powers of two are allowed.
/// PowersOf10   | Only powers of 10 are allowed.
/// 
/// If `Type` is not specified, the compiler will attempt to infer the parameter's type from the other nodes. The inference rules are:
/// 1. If the parameter has `Default`, determine the type based on its type (only applies to Boolean and Number).
/// 2. If the parameter has `Min`, `Max` or `Restriction`, the parameter's type is Number.  
/// 
/// Example:
/// 
/// @code{.cs}
/// enum IslandSizes
/// {
///	    Small,
///	    Medium,
///	    Large
/// }
///
/// metadata
/// {
///	    Name: "Archipelago"
///     Width: {Min: 100, Max: 1000, Default: 500},
///	    Height: {Min: 100, Max: 1000, Default: 500},
///	    Params:
///     {
///	        NumberOfIslands: { Min: 1, Max: 10, Default: 5, Restriction: Integers },
///	        IslandSize: { Type: IslandSizes, Default: Medium },
///	        VolcanoesEnabled: { Default: false }
///	    }
/// }
/// @endcode
/// 
/// Following parameter names are reserved and must not be used:
/// @verbatim Width, Height, RenderOriginX, RenderOriginY, RenderWidth, RenderHeight @endverbatim
/// 
/// @section accessing_parameters Accessing script parameters from code
/// All script parameters, including map dimensions and additional render parameters can be accesses via the Parameters global object.
/// This object has following predefined properties in addition to any user parameters declared by the script:
/// 
/// Name	         | Type         | Description
/// -----------------|--------------|--------------------------------------------------------------------------
/// MapWidth         | Number       | Width of the map. Only defined if the map is finite in the horizontal direction.
/// MapHeight        | Number       | Height of the map. Only defined if the map is finite in the vertical direction.
/// RenderOriginX    | Number       | X coordinate of the render origin.
/// RenderOriginY    | Number       | Y coordinate of the render origin.
/// RenderWidth      | Number       | Width of the render (after scaling).
/// RenderHeight     | Number       | Height of the render (after scaling).
/// RenderScale      | Number       | Render scale (1.0 = 1:1, 0.5 = 2x zoomed out, 2.0 = 2x zoomed in).
/// 
/// (doplnit odkaz na popis principu renderování) a na princip škálování
/// Example
