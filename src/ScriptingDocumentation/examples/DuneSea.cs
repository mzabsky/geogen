/// @example dunesea.ggs
/// @page example_dunesea Example: Dune sea
/// This script creates an infinite desert of dunes. It demonstrates how a simple combination of a few basic functions can produce very interesting results.
/// 
/// @image html examples_dunesea.png
/// 	   
/// The main idea is what an intersection of two layers of low frequency noise looks like. Consider following example:
/// 
/// @code{.cs}
/// yield HeightMap.Noise({ 100: 0.9 })
///     .Intersect(HeightMap.Noise({ 100: 0.9 }, 2));  
/// @endcode
/// 
/// @image html examples_dunesea_noise.png
/// 
/// All that is needed to make a great looking desert from such noise is some minor fine tuning.
/// 
/// @snippet dunesea.ggs Body
///          
/// @link dunesea.ggs Full code @endlink

