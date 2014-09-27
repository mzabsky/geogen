/// Two-dimensional height map.
/// @ingroup standard_library
class HeightMap
{
public:

    /// Creates a deep copy of another height map.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.RadialGradient([400, 500], 500, 1, 0);
    /// var copy = HeightMap.Clone(main);
    /// copy.Move([200, 0]);
    /// yield main.Unify(copy);
    /// @endcode           
    /// 
    /// @image html std_2d_clone.png
    /// 
	/// @param heightMap (Optional) The map to copy.
	/// @return The height map.
	static HeightMap Clone(HeightMap heightMap);

    /// Creates a map representing convexity of local areas (with @a radius) in another height map.
    /// Positive heights in the convexity map indicate convex areas and negative heights indicate
    /// concave areas.
    /// @param heightMap The height map.
    /// @param radius The radius. Must be greater than 0.
    /// @return The convexity map.
	static HeightMap ConvexityMap(HeightMap heightMap, Number radius);

    /// Creates a map representing euclidean distance of each pixel to the closest pixel height 0 or less. The distance is capped at @a maximumDistance.
    /// @param heightMap The height map.
    /// @param maximumDistance The maximum distance considered. Must be greater than 0.
    /// @return The convexity map.
	static HeightMap DistanceMap(HeightMap heightMap, Number maximumDistance);
    
	/// Creates a height map with constant @a height.
	/// @param height (Optional) The height. If not provided, 0 is used.
	/// @return The height map.
	static HeightMap Flat(Number height);

    /// Creates a linear height gradient going from @a sourceHeight at @a sourcePoint to @a 
    /// destinationHeight at @a destinationPoint. Pixels outside of the gradient strip are filled with height of source or destination, whichever is closer.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Gradient([200, 200], [700, 700], -1, 1);
    /// @endcode
    ///         
    /// @image html std_2d_gradient.png
    /// 
    /// @param sourcePoint The source point.
    /// @param destinationPoint The destination point.
    /// @param sourceHeight Height at the source.
    /// @param destinationHeight Height at the destination.
    /// @return The height map.
	static HeightMap Gradient(Point sourcePoint, Point destinationPoint, Number sourceHeight, Number destinationHeight);

    /// Creates a height map filled with random perlin noise.
    /// 
    /// The layer definitions are an array of pairs `wave length: amplitude`. Wave length and amplitude must be greater than 0. Example:
    /// 
    /// @code{.cs}
    /// yield HeightMap.Noise({1: 0.1, 8: 0.3, 512: 0.6 });
    /// @endcode
    /// @image html std_2d_noise_custom.png    
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
    /// @image html std_2d_noise_standard.png  
    /// 
    /// TODO: Overshoots.
    /// 
    /// @see [Perlin noise](http://freespace.virgin.net/hugo.elias/models/m_perlin.htm)
    /// 
    /// @param layerDefinitions (Optional) Array defining layers of the noise. If not provided, a default array is used..
    /// @param seed (Optional) Random seed. If not provided, 0 is used. This seed is always combined with the main script seed provided in script arguments to the script.
    /// @return The height map.
    static HeightMap Noise(Array layerDefinitions, Number seed);

    /// Creates a height map with a rectangular area of @a pattern repeating infinitely in all
    /// directions. The repeating pattern will start at point [0, 0].
    /// 
    /// Example:
    /// @code{.cs}
    /// var pattern = HeightMap.RadialGradient([100, 100], 200, 1, -1);
    /// yield HeightMap.Pattern(pattern, [0, 0], [200, 200]);
    /// @endcode
    /// 
    /// @image html std_2d_pattern.png
    /// 
    /// @param pattern The height map from which the pattern is copied.
    /// @param rectanglePosition The position of top left corner of the rectangle.
    /// @param rectangleSize Size of the rectangle (@link Point.X X@endlink is its width and @link
    /// Point.Y Y@endlink is its height).
    /// @return The height map.
    static HeightMap Pattern(HeightMap pattern, Point rectanglePosition, Point rectangleSize);

    /// Creates a height map with a @a profile projected along @a direction.
    /// 
    /// Example:
    /// @code{.cs}
    /// var profile = HeightProfile.Noise();
    /// yield HeightMap.Projection(profile, Direction.Vertical);
    /// @endcode
    /// 
    /// @image html std_2d_projection.png
    /// 
    /// @param profile The height profile.
    /// @param direction The projection direction.
    /// @return The height map.
	static HeightMap Projection(HeightProfile profile, Direction direction);

    /// Creates a radial height gradient centered at @a center with @a radius. It goes from @a centerHeight to @a rimHeight. Any pixels outside of the gradient will be filled with @a rimHeight.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.RadialGradient([400, 400], 500, 1, 0);
    /// @endcode
    /// 
    /// @image html std_2d_radial_gradient.png
    /// @param center The source point.
    /// @param radius The radius. Must not be negative.
    /// @param centerHeight Height at the source.
    /// @param rimHeight Height at the destination.
    /// @return The height map.
    static HeightMap RadialGradient(Point center, Number radius, Number centerHeight, Number rimHeight);

    
    /// Replaces height in each pixel with its absolute value (makes negative heights positive).
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().Abs();
    /// @endcode
    /// 
    /// @image html std_2d_abs.png
    /// @return The height map itself (for call chaining).
    HeightMap Abs();

    /// Adds a number or a corresponding pixel from another height map to each pixel in the height
    /// map. If @a mask is provided, the addition in each pixel will be modulated by the mask.
    /// Heights less than 0 from the mask are considered to be 0.
    /// 
    /// Example with a numeric addend without a mask (forcing the heights at the tip of the cone to overflow):
    /// @code{.cs}
    /// yield HeightMap.RadialGradient([500, 500], 400, 1, -1).Add(0.5);
    /// @endcode
    ///       
    /// @image html std_2d_add_number.png
    ///         
    /// Example with map addend:
    /// @code{.cs}
    /// var rectangle = HeightMap.Flat().FillRectangle([600, 400], [200, 200], 0.5);
    /// yield HeightMap.RadialGradient([500, 500], 400, 0.7, -1).Add(rectangle);
    /// @endcode
    /// 
    /// @image html std_2d_add_map.png
    /// 
    /// Example with map addend and a mask:    
    /// @code{.cs}
    /// var main = HeightMap.RadialGradient([500, 500], 450, -0.5, 0.75);
    /// var noise = HeightMap.Noise();
    /// var mask = HeightMap.Clone(main).ClampHeights(0.1, 1); 
    /// yield main.Add(noise, mask); // Less of the noise will be added in the center, where there are lower heights in the mask
    /// @endcode
    /// 
    /// @image html std_2d_add_map_mask.png
    /// 
    /// @param addend The addend.
    /// @param mask (Optional) The mask.
    /// @return A HeightMap.
	HeightMap Add(Number/HeightMap addend, HeightMap mask);

    /// Blurs the height map with box blur algorithm with kernel of given radius.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.RadialGradient([500, 500], 450, 1, -1);
    /// main.Unify(HeightMap.Gradient([0, 0], [1000, 0], -1, 1));
    /// yield main.Blur(50);
    /// @endcode
    /// 
    /// @image html std_2d_blur.png
    ///        
    /// @see [Box blur on Wikipedia](http://en.wikipedia.org/wiki/Box_blur)
    /// @param radius The blur kernel size.
    /// @return The height map itself (for call chaining).
    HeightMap Blur(Number radius);

    /// Replaces heights greater than @a max with @a max and less than @a min with @a min.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().ClampHeights(-0.3, 0.3);
    /// @endcode
    /// 
    /// @image html std_2d_clampheights.png.
    ///        
    /// @param min The minimum height.
    /// @param max The maximum height.
    /// @return The height map itself (for call chaining).
    HeightMap ClampHeights(Number min, Number max);

    /// Replaces heights in each pixel with a height blended from that height and corresponding height from another height map. The blending will be done according to the height in the mask - 1 will mean 100% of the current height map's height will be used, 0 will mean 100% of the height from the other height map will be used. Any heights in between will cause the two heights to be blended linearly. Heights less than 0 from the mask are considered to be 0.
    /// 
    /// Example:
    /// @code{.cs}
    /// var roughNoise = HeightMap.Noise();
    /// var smoothNoise = HeightMap.Noise({256: 0.9}, 1); // Use different seed.
    /// var mask = HeightMap.Gradient([300, 0], [700, 0], 0, 1).Blur(20); // Blur to remove hard edges on ends of the gradient
    /// yield roughNoise.Combine(smoothNoise, mask);
    /// @endcode
    /// 
    /// @image html std_2d_combine.png.
    ///        
    /// @param other The other map.
    /// @param mask The mask.
    /// @return The height map itself (for call chaining).
    HeightMap Combine(HeightMap other, HeightMap mask);

    /// Replaces all heights outside of an rectangle with @a replace.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.RadialGradient([500, 500], 400, 1, 0).Crop([350, 250], [300, 500]);
    /// @endcode
    /// 
    /// @image html std_2d_crop.png.
    /// @param rectanglePosition The position of top left corner of the rectangle.
    /// @param rectangleSize Size of the rectangle (@link Point.X X@endlink is its width and @link
    /// Point.Y Y@endlink is its height).
    /// @param replace (Optional) The replacement height. 0 is used if not provided.
    /// @return The height map.
    HeightMap Crop(Point rectanglePosition, Pont rectangleSize, Number replace);

    /// Replaces heights greater than @a max or less than @a min with @a replace.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().ClampHeights(-0.3, 0.3);
    /// @endcode
    /// 
    /// @image html std_2d_cropheights.png.
    ///        
    /// @param min The minimum height.
    /// @param max The maximum height.
    /// @param replace (Optional) The replacement height. 0 is used if not provided.
    /// @return The height map itself (for call chaining).
    HeightMap CropHeights(Number min, Number max, Number replace);

    /// Replaces each pixel with @a height.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().Fill(0.5);
    /// @endcode
    /// 
    /// @image html std_2d_fill.png.
    ///        
    /// @param height The height.
    /// @return The height map itself (for call chaining).
    HeightMap Fill(Number height);

    /// Replaces each pixel in a rectangle with @a height.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().FillRectangle([200, 200], [400, 600], 0.5);
    /// @endcode
    /// 
    /// @image html std_2d_fillrectangle.png.
    ///        
    /// @param rectanglePosition The position of top left corner of the rectangle.
    /// @param rectangleSize Size of the rectangle (@link Point.X X@endlink is its width and @link
    /// Point.Y Y@endlink is its height).
    /// @param height The height.
    /// @return The height map itself (for call chaining).
    HeightMap FillRectangle(Point rectanglePosition, Point rectangleSize, Number height);

    /// Sets each pixel in the to the less of the two corresponding heights in the current map and the other map.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Gradient([0, 0], [1000, 0], 0, 1);
    /// var other = HeightMap.Gradient([0, 0], [0, 1000], 0, 1);
    /// yield main.Intersect(other);
    /// @endcode
    /// 
    /// @image html std_2d_intersect.png.
    ///        
    /// @param other The other map.
    /// @return The height map itself (for call chaining).
    HeightMap Intersect(HeightMap other);

    /// Sets each pixel in the to the greater of the two corresponding heights in the current map and the other map.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Gradient([0, 0], [1000, 0], 0, 1);
    /// var other = HeightMap.Gradient([0, 0], [0, 1000], 0, 1);
    /// yield main.Unify(other);
    /// @endcode
    /// 
    /// @image html std_2d_unify.png.
    ///        
    /// @param other The other map.
    /// @return The height map itself (for call chaining).
    HeightMap Unify(HeightMap other);
};