/// @example ouroboros.ggs
/// @page example_ouroboros Example: Ouroboros
/// This script creates a ring of exactly identical islands.
/// 
/// @image html examples_ouroboros.png
/// 
/// The map is finite in both the horizontal and vertical directions.
/// 
/// @snippet tepui.ggs Metadata
///          
/// The lowland and the highland (the mountaintop) are built separately. The lowland is medium-range noise with a cone-shaped peak in the middle (do that the terrain eases a bit into the cliffs). The height overflow in the center doesn't matter, it will be replaced by the tepui itself.
/// 
/// @snippet tepui.ggs Lowland
/// @image html examples_tepui_lowland.png
/// 
/// The highland is a low-range noise offset to the high altitude of the mountaintop.
/// 
/// @snippet tepui.ggs Highland
/// @image html examples_tepui_highland.png
///          
/// The mask is build similarly to the lowland, except it is turned to a monochrome image and then blurred a bit to make the cliffs somewhat less sharp. Note that each of the layers uses a noise with a different seed, so that different noise is generated each time.
/// 
/// @snippet tepui.ggs Mask
/// @image html examples_tepui_mask.png
/// 
/// The lowland and the highland are then combined together using the mask.
/// @snippet tepui.ggs Combination
///          
/// @link tepui.ggs Full code @endlink

