/// @page tutorial_first_script Tutorial: First script
/// This tutorial shows how to write your first simple GeoGen map script.
///
/// Each map script must generate at least one map: the height map itself. Therefore the simplest possible script generates a single flat and empty height map:
///
/// @code{.cs}
/// yield HeightMap.Flat();
/// @endcode
/// 
/// The HeightMap.Flat() calls a method @link HeightMap.Flat Flat@endlink of the HeightMap class. This method is a constructor, a method which doesn't require to be called on an existing instance of the class. Instead, it creates a new instance, in this case representing a new height map. As the function name suggests, it creates a completely flat height map. 
/// 
/// Each pixel of this height map will be set to the height 0. This height serves as a baseline height and is often used as sea level. 
/// 
/// The keyword `yield` then returns the height map to the hosting application, fulfilling the script's duty to return a single height map.
/// 
/// The @link HeightMap.Flat Flat@endlink function can also be used with a single numeric argument specifying a height. If provided, each pixel in the map will be filled with this height (instead of 0). The heights in GeoGen are always in range between -1 and 1. 
/// 
/// Following script creates a map filled with height 0.5:
/// 
/// @code{.cs}
/// yield HeightMap.Flat(0.5);
/// @endcode
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 