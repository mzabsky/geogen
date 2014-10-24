/// @example ouroboros.ggs
/// @page example_ouroboros Example: Ouroboros
/// This script creates a ring of exactly identical islands.
/// 
/// @image html examples_ouroboros.png
/// 
/// The map is finite in both the horizontal and vertical directions.
/// 
/// @snippet ouroboros.ggs Metadata
///          
/// First step is to create a template of a single island, which will be then copied several times to create the ring of islands. This template is centered at coordinate `[0, 0]`, because it will have to be rotated later and rotation is always centered at [0, 0].
/// 
/// @snippet ouroboros.ggs Template
/// @image html examples_ouroboros_template.png
/// 
/// To create each island on the ring, the template is copied, rotated to be tangent to the circle, moved to its spot and then unified into the main map.
/// 
/// @snippet ouroboros.ggs Copies
/// @image html examples_ouroboros_copies.png
///          
/// The last step is to fill in the remaining flat areas with some generic noise. To do this, a mask that is an inverse of the map itself is used (so more of the noise is used where the heights in the map are the least). The same noise is used as the one which was used for the template. There is no reason not to.
/// 
/// @snippet ouroboros.ggs Mask
/// @image html examples_ouroboros_mask.png
///          
/// @link ouroboros.ggs Full code @endlink

