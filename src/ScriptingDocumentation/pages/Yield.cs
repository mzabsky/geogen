/// @page yield Yielding maps
/// `yield` statement is used to pass maps from the script. If no name is provided, the passed map will become the primary height map:
/// @code{.cs}
/// var map = HeightMap.Noise();
/// yield map;
/// @endcode
/// If a name is provided, the map will be yielded as map of that name:
///  @code{.cs}
/// var map = HeightMap.Noise();
/// yield map;
/// yield HeightMap.ConvexityMap(map) as "ConvexityMap";
/// @endcode
/// 		
/// The primary map must always be yielded during execution of the script, otherwise an runtime error will be triggered when the execution finishes. Each map can be only yielded once, otherwise an runtime error is triggered.
/// 
/// When a map is yielded, a copy of it is made by the API for further processing. The script is allowed to make arbitrary changes to a map that was already yielded.
/// 