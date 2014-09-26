/// @page tutorial_combining_maps Tutorial: Combining maps
/// This tutorial shows how to combine multiple maps into a real height map.
///
/// One of the most common ways co create complex maps in GeoGen is to compose them from two basic layers: a programatically defined shape layer and randomized noise layer. These two layers can be simply added together for the desired result. In this example, we will create a map with a single island on it:
/// 
/// TODO: Image
/// 
/// The shape layer contains a single cone created with @link HeightMap.RadialGradient radial gradient@endlink @link HeightMap.Blur blurred@endlink to avoid any hard edges or spikes. 
/// 
/// Code:
/// 
/// @code{.cs}
/// var main = HeightMap.RadialGradient([500, 500], 250, 0.5, -0.5); // Gradient cone centered at 500, 500
/// main.Blur(30); // Blur with radius of 30 px to avoid hard edges
/// 
/// var noise = HeightMap.Noise(); // Noise with default settings
/// noise.Multiply(0.5); // Noise with default settings generates heights approx. in range <-1, 1>
/// 
/// main.Add(noise);
/// yield main;
/// @endcode
/// 
/// In scenarios where multiple layers are being combined we have to pay attention to make sure the heights added together stay in the allowed value range. Therefore the shape layer is restricted to <-0.5, 0.5> and the noise layer is multiplied to make sure it stays in that range too.
/// 
/// Other functions that can be used to create shape layers include HeightMap.Gradient, HeightMap.Projection and HeightMap.Pattern.
/// 
/// Other combination functions include HeightMap.Combine, HeightMap.Intersect, HeightMap.Multiply and HeightMap.Unify.
/// 
/// 
/// 
/// 
/// 