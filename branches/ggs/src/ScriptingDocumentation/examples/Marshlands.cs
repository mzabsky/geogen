/// @example marshlands.ggs
/// @page example_marshlands Example: Marshlands
/// This script creates wide river valley with a bottom covered almost entirely by a vast marshland.
/// 
/// @image html examples_marshlands.png
/// 
/// The map is infinite in the horizontal direction and finite in the vertical direction.
/// 
/// @snippet marshlands.ggs Metadata
///          
/// First step is to build the valley banks. The banks are built using a combination of HeightMap.Projection and HeightMap.Shift. Each of the banks is created separately, so each bank has a different curve, giving the map a more natural look. 
/// 
/// @snippet marshlands.ggs Banks
/// @image html examples_marshlands_banks.png
/// 
/// Second step is to build the riverbed, once again a shifted projection (and once again with a different curve).
/// 
/// @snippet marshlands.ggs Riverbed
///          
/// Then the canal structure of the valley floor is created. "Ridged" noise is created by applying HeightMap.Abs on HeightMap.Noise.
/// 
/// @image html examples_marshlands_ridges.png
/// 
/// Two layers of this ridged noise are combined together to make the canal pattern even more chaotic and dense.
///
/// @snippet marshlands.ggs Marsh
/// @image html examples_marshlands_marsh.png
/// 
/// All of the layers built above are then combined together.
/// @snippet marshlands.ggs Combination
///          
/// @link marshlands.ggs Full code @endlink

