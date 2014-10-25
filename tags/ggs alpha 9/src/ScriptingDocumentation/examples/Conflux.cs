/// @example conflux.ggs
/// @page example_conflux Example: Conflux
/// This script creates a map with several rivers meeting in a central lake.
/// 
/// @image html examples_conflux.png
/// 
/// The map is finite in both the horizontal and vertical directions. This script declares two @link script_parameters script parameters@endlink - one is numeric and can configure number of rivers meeting in the conflux and the other configures size of the central lake.
/// 
/// @snippet conflux.ggs Metadata
/// 
/// The basic idea is to create a template out of lines and a circle, distort it, turn it into a three-dimensional shape with HeightMap.Distance map and then sprinkle in some noise to make the map look more natural.
/// 
/// First step to create the template is to draw lines going from center of the center of the map to points an a circle greater than the map itself (so that end of the line is never seen). Spacing of the points ont he circle is slightly randomized, to make the map less predictable.
/// 
/// A circle with radius based on the `LakeSize` script parameter is then added to the template.
/// 
/// Because the whole template is to be used with HeightMap.DistanceMap, the "base" height is 1 (any height greater than 0 would suffice) and the highlight height is any height equal to or less than 0.
/// 
/// @snippet conflux.ggs Template
/// @image html examples_conflux_template.png
/// 
/// The template is then distorted. The distortion is always continuous, guaranteed not to break the lines.  
///
/// @snippet conflux.ggs Distortion
/// @image html examples_conflux_template_distorted.png
///                            
/// The distortion however causes the lines and shapes to blur, which can be rectified by sharpening the image. HeightMap.CropHeights can be used for this purpose. Using the same number as both its @a min and @a max arguments will replace all height but that height with @a replace. In this case, all heights other than 1 are replaced with 0.
///                            
/// @snippet conflux.ggs Sharpening
/// @image html examples_conflux_template_sharpened.png
///                            
/// The template is now finished and can be used for the distance map. A small number is subtracted form the distance map to create basins of the rivers and the lake.
/// 
/// @snippet conflux.ggs DistanceMap
/// @image html examples_conflux_distancemap.png
/// 
/// The lowland and the highland are then combined together using a copy of the distance map as a mask.
/// @snippet conflux.ggs Combination
/// @image html examples_conflux_combination.png
///          
/// The final step is to apply a glaciation filter on the map, making the river banks much softer.
///          
/// @snippet conflux.ggs Glaciation
/// @image html examples_conflux.png
///          
/// @link conflux.ggs Full code @endlink

