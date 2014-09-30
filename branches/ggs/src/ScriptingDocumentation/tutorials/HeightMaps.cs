/// @page tutorial_heightmaps Tutorial: What are height maps?
/// Height map is a grayscale raster image representing elevation of terrain surface. 
/// 
/// Each pixel in a height map represents height of a corresponding point on the terrain surface - the brighter the pixel, the higher the terrain is in that point. Following example features a height map of the region around [Lake Geneva](http://en.wikipedia.org/wiki/Lake_Geneva), Switzerland:
/// 
/// @image html tut_geneva.png
/// 
/// The image contains the height map itself (on the left) and its 3D visualization (on the left). Each pixel on the height map represents approximately square area with a side of 220 meters. The height map itself however doesn't contain any information about the scale of the image, neither does it contain any information about which color represents which altitude. In this image, black represents water level of the lake (372 m above sea level) and white represents the peak of [Mont Ruan](http://en.wikipedia.org/wiki/Mont_Ruan) (3057 m).
/// 
/// If we look at height map of the entire Earth surface, the scale is much different. One pixel represents area with side of 50 km (at the equator) and black is the sea level while white is about 9000 m above sea level.
/// 
/// @image html tut_world.png
/// 
/// Main advantages of heght maps are efficient storage and ease of use - a height map can stored using any image format such as JPG or PNG and can be viewed, created and modified using most common graphics applications like Microsoft Paint or Adobe Photoshop. 
/// 
/// Main disadvantage is that height maps are restricted to representing a single height in each point and cannot be used to represent terrain features such as caves and overhangs. More sophisticated representation such as [polygon mesh](http://en.wikipedia.org/wiki/Polygon_mesh) or [voxel array](http://en.wikipedia.org/wiki/Voxel) is required to store these. Another disadvantage is the lack of any metadata describing context of the map (such as scale as mentioned above).
/// 
/// GeoGen is capable of generating height maps representing regions ranging from small local terrain features to entire continents, the scale of the map depends solely on author of the map scrip.ttw
/// 
//// @image html tut_introduction.png
/// 
/// 
/// 
/// 
/// 
/// 
/// 
/// 
// /