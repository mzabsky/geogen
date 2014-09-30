/// One-dimensional height profile.
/// Coordinates go from left (negative) to right (positive).
/// 
/// @ingroup standard_library
class HeightProfile
{
public:

    /// Creates a deep copy of another height profile.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightProfile.RadialGradient([400, 500], 500, 1, 0);
    /// var copy = HeightProfile.Clone(main);
    /// copy.Move([200, 0]);
    /// yield main.Unify(copy);
    /// @endcode
    /// 
	/// @param heightMap (Optional) The map to copy.
	/// @return The height profile.
	static HeightProfile Clone(HeightProfile heightProfile);
    
	/// Creates a height profile with constant @a height.
	/// @param height (Optional) The height. If not provided, 0 is used.
	/// @return The height profile.
	static HeightProfile Flat(Number height);

    /// Creates a linear height gradient going from @a sourceHeight at @a sourcePoint to @a 
    /// destinationHeight at @a destinationPoint. Pixels outside of the gradient strip are filled with height of source or destination, whichever is closer.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightProfile.Gradient(200, 700, -1, 1);
    /// @endcode
    ///         
    /// @image html std_2d_gradient.png
    /// 
    /// @param sourcePoint The source point.
    /// @param destinationPoint The destination point.
    /// @param sourceHeight Height at the source.
    /// @param destinationHeight Height at the destination.
    /// @return The height profile.
	static HeightProfile Gradient(Coordinate sourcePoint, Coordinate destinationPoint, Number sourceHeight, Number destinationHeight);

    /// Creates a height profile filled with random perlin noise.
    /// 
    /// The layer definitions are an array of pairs `wave length: amplitude`. Wave length and amplitude must be greater than 0. Example:
    /// 
    /// @code{.cs}
    /// yield HeightProfile.Noise({1: 0.1, 8: 0.3, 512: 0.6 });
    /// @endcode
    /// @image html std_1d_noise_custom.png    
    /// 
    /// The default value for @a layerDefinitions parameter is:
    /// @code{.cs}
    /// {
    ///     {1: 0.001953125},
    ///     {2: 0.00390625}, 
    ///     {4: 0.0078125}, 
    ///     {8: 0.015625}, 
    ///     {16: 0.03125}, 
    ///     {32: 0.0625},
    ///     {64: 0.125},
    ///     {128: 0.25},
    ///     {256: 0.5}
    /// }
    /// @endcode
    /// 
    /// Which produces noise like this:    
    /// @image html std_1d_noise_standard.png  
    /// 
    /// TODO: Overshoots.
    /// 
    /// @see [Perlin noise](http://freespace.virgin.net/hugo.elias/models/m_perlin.htm)
    /// 
    /// @param layerDefinitions (Optional) Array defining layers of the noise. If not provided, a default array is used.
    /// @param seed (Optional) Random seed. If not provided, 0 is used. This seed is always combined with the main script seed provided in script arguments to the script.
    /// @return The height profile.
    static HeightProfile Noise(Array layerDefinitions, Number seed);

    /// Creates a height profile with an interval of @a pattern repeating infinitely in both directions. The repeating pattern will start at coordinate 0.
    /// 
    /// Example:
    /// @code{.cs}
    /// var pattern = HeightProfile.RadialGradient([100, 100], 200, 1, -1);
    /// yield HeightProfile.Pattern(pattern, [0, 0], [200, 200]);
    /// @endcode
    /// 
    /// @image html std_1d_pattern.png
    /// 
    /// @param pattern The height profile from which the pattern is copied.
    /// @param intervalStart The position of start of the interval.
    /// @param intervalLength Length of the interval.
    /// @return The height profile.
    static HeightProfile Pattern(HeightProfile pattern, Interval intervalStart, Interval intervalLength);

    /// Creates a height profile from a single row (if @a direction is horizontal) or column (if @a direction is vertical) of pixels of an height map.
    /// 
    /// Example:
    /// @code{.cs}
    /// var profile = HeightMap.Noise();
    /// yield HeightProfile.Projection(profile, Direction.Vertical);
    /// @endcode
    /// 
    /// @image html std_1d_slice.png
    /// 
    /// @param profile The height profile.
    /// @param coordinate Coordiante of the row/column.
    /// @param direction The projection direction.
    /// @return The height profile.
	static HeightProfile Slice(HeightMap heightMap, Number coordinate, Direction direction);
    
    /// Replaces height in each pixel with its absolute value (makes negative heights positive).
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightProfile.Noise().Abs();
    /// @endcode
    /// 
    /// @image html std_1d_abs.png
    /// @return The height profile itself (for call chaining).
    HeightProfile Abs();

    /// Adds a number or a corresponding pixel from another height profile to each pixel in the height
    /// profile. If @a mask is provided, the addition in each pixel will be modulated by the mask.
    /// Heights less than 0 from the mask are considered to be 0.
    /// 
    /// Example with a numeric addend without a mask (forcing the heights at the tip of the cone to overflow):
    /// @code{.cs}
    /// yield HeightProfile.RadialGradient([500, 500], 400, 1, -1).Add(0.5);
    /// @endcode
    ///       
    /// @image html std_2d_add_number.png
    ///         
    /// Example with profile addend:
    /// @code{.cs}
    /// var rectangle = HeightProfile.Flat().FillRectangle([600, 400], [200, 200], 0.5);
    /// yield HeightProfile.RadialGradient([500, 500], 400, 0.7, -1).Add(rectangle);
    /// @endcode
    /// 
    /// @image html std_2d_add_map.png
    /// 
    /// Example with profile addend and a mask:    
    /// @code{.cs}
    /// var main = HeightProfile.RadialGradient([500, 500], 450, -0.5, 0.75);
    /// var noise = HeightProfile.Noise();
    /// var mask = HeightProfile.Clone(main).ClampHeights(0.1, 1); 
    /// yield main.Add(noise, mask); // Less of the noise will be added in the center, where there are lower heights in the mask
    /// @endcode
    /// 
    /// @image html std_2d_add_map_mask.png
    /// 
    /// @param addend The addend.
    /// @param mask (Optional) The mask.
    /// @return The height profile itself (for call chaining).
	HeightProfile Add(Number/HeightProfile addend, HeightProfile mask);

    /// Blurs the height profile with box blur algorithm with kernel of given radius.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightProfile.RadialGradient([500, 500], 450, 1, -1);
    /// main.Unify(HeightProfile.Gradient([0, 0], [1000, 0], -1, 1));
    /// yield main.Blur(50);
    /// @endcode
    /// 
    /// @image html std_2d_blur.png
    ///        
    /// @see [Box blur on Wikipedia](http://en.wikipedia.org/wiki/Box_blur)
    /// @param radius The blur kernel size.
    /// @return The height profile itself (for call chaining).
    HeightProfile Blur(Number radius);

    /// Replaces heights greater than @a max with @a max and less than @a min with @a min.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightProfile.Noise().ClampHeights(-0.3, 0.3);
    /// @endcode
    /// 
    /// @image html std_2d_clampheights.png.
    ///        
    /// @param min The minimum height.
    /// @param max The maximum height.
    /// @return The height profile itself (for call chaining).
    HeightProfile ClampHeights(Number min, Number max);

    /// Replaces heights in each pixel with a height blended from that height and corresponding height from another height profile. The blending will be done according to the height in the mask - 1 will mean 100% of the current height profile's height will be used, 0 will mean 100% of the height from the other height profile will be used. Any heights in between will cause the two heights to be blended linearly. Heights less than 0 from the mask are considered to be 0.
    /// 
    /// Example:
    /// @code{.cs}
    /// var roughNoise = HeightProfile.Noise();
    /// var smoothNoise = HeightProfile.Noise({256: 0.9}, 1); // Use different seed.
    /// var mask = HeightProfile.Gradient([300, 0], [700, 0], 0, 1).Blur(20); // Blur to remove hard edges on ends of the gradient
    /// yield roughNoise.Combine(smoothNoise, mask);
    /// @endcode
    /// 
    /// @image html std_1d_combine.png.
    ///        
    /// @param other The other profile.
    /// @param mask The mask.
    /// @return The height profile itself (for call chaining).
    HeightProfile Combine(HeightProfile other, HeightProfile mask);

    /// Replaces all heights outside of an interval with @a replace.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightProfile.RadialGradient([500, 500], 400, 1, 0).Crop([350, 250], [300, 500]);
    /// @endcode
    /// 
    /// @image html std_2d_crop.png.
    /// 
    /// @param intervalStart The position of start of the interval.
    /// @param intervalLength Length of the interval.
    /// @param replace (Optional) The replacement height. 0 is used if not provided.
    /// @return The height profile.
    HeightProfile Crop(Interval intervalStart, Interval intervalLength, Number replace);

    /// Replaces heights greater than @a max or less than @a min with @a replace.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightProfile.Noise().ClampHeights(-0.3, 0.3);
    /// @endcode
    /// 
    /// @image html std_2d_cropheights.png.
    ///        
    /// @param min The minimum height.
    /// @param max The maximum height.
    /// @param replace (Optional) The replacement height. 0 is used if not provided.
    /// @return The height profile itself (for call chaining).
    HeightProfile CropHeights(Number min, Number max, Number replace);

    /// Replaces each pixel with @a height.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightProfile.Noise().Fill(0.5);
    /// @endcode
    /// 
    /// @image html std_2d_fill.png.
    ///        
    /// @param height The height.
    /// @return The height profile itself (for call chaining).
    HeightProfile Fill(Number height);

    /// Replaces each pixel in an interval with @a height.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightProfile.Noise().FillInterval([200, 200], [400, 600], 0.5);
    /// @endcode
    /// 
    /// @image html std_2d_fillrectangle.png.
    /// 
    /// @param intervalStart The position of start of the interval.
    /// @param intervalLength Length of the interval.
    /// @param height The height.
    /// @return The height profile itself (for call chaining).
    HeightProfile FillInterval(Interval intervalStart, Interval intervalLength, Number height);

    /// Sets each pixel in the to the less of the two corresponding heights in the current profile and the other profile.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightProfile.Gradient([0, 0], [1000, 0], 0, 1);
    /// var other = HeightProfile.Gradient([0, 0], [0, 1000], 0, 1);
    /// yield main.Intersect(other);
    /// @endcode
    /// 
    /// @image html std_2d_intersect.png.
    ///        
    /// @param other The other map.
    /// @return The height profile itself (for call chaining).
    HeightProfile Intersect(HeightProfile other);

    /// Flips sign of each height in the profile.
    /// 
    /// Example:
    /// @code{.cs}
    /// // Transforms absolute value noise into ridges
    /// yield HeightProfile.Noise().Abs().Invert().Add(1);
    /// @endcode
    /// 
    /// @image html std_2d_invert.png
    /// @return The height profile itself (for call chaining).
    HeightProfile Invert();

    /// Moves the height profile by offset.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightProfile.Noise({256:1});
    /// var copy = HeightProfile.Clone(main).Move([150, 100]);
    /// yield main.Unify(copy);
    /// @endcode
    /// 
    /// @image html std_2d_move.png.
    ///        
    /// @param offset The offset. The map is @a offset to the right.
    /// @return The height profile itself (for call chaining).
    HeightProfile Move(Point offset);

    /// Multiplies the height in each pixel by a number or a corresponding pixel from another height profile. 
    /// 
    /// Example with a numeric factor:
    /// @code{.cs}
    /// yield HeightProfile.Noise().Multiply(0.2);
    /// @endcode
    ///       
    /// @image html std_2d_multiply_number.png
    ///         
    /// Example with map factor:
    /// @code{.cs}
    /// var main = HeightProfile.Noise();
    /// var multiplier = HeightProfile.RadialGradient([500, 500], 400, 1, 0.2);
    /// yield main.Multiply(multiplier);
    /// @endcode
    /// 
    /// @image html std_2d_multiply_map.png
    /// 
    /// @param factor The factor.
    /// @return A HeightProfile.
	HeightProfile Multiply(Number/HeightProfile factor);


    /// Sets each pixel in the to the greater of the two corresponding heights in the current profile and the other profile.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightProfile.Gradient([0, 0], [1000, 0], 0, 1);
    /// var other = HeightProfile.Gradient([0, 0], [0, 1000], 0, 1);
    /// yield main.Unify(other);
    /// @endcode
    /// 
    /// @image html std_2d_unify.png.
    ///        
    /// @param other The other profile.
    /// @return The height profile itself (for call chaining).
    HeightProfile Unify(HeightProfile other);
};