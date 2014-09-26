/// Two-dimensional height map.
/// @ingroup standard_library
class HeightMap
{
public:

    /// Creates a deep copy of another height map.
	/// @param height (Optional) The map to copy.
	/// @return The height map.
	static HeightMap Clone(HeightMap other);

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
    
	/// Creates a height map with constant @height.
	/// @param height (Optional) The height. If not provided, 0 is used.
	/// @return The height map.
	static HeightMap Flat(Number height);

    /// Creates a linear height gradient going from @a sourceHeight at @a sourcePoint to @a 
    /// destinationHeight at @a destinationPoint. Pixels outside of the gradient strip are filled with height of source or destination, whichever is closer.
    /// @param sourcePoint The source point.
    /// @param destinationPoint The destination point.
    /// @param sourceHeight Height at the source.
    /// @param destinationHeight Height at the destination.
    /// @return The height map.
	static HeightMap Gradient(Point sourcePoint, Point destinationPoint, Number sourceHeight, Number destinationHeight);

    /// Creates a height map filled with random @link http://freespace.virgin.net/hugo.elias/models/m_perlin.htm perlin noise@endlink.
    /// 
    /// The layer definitions are an array of pairs `wave length: amplitude`. Wave length and amplitude must be greater than 0. Example:
    /// 
    /// @code{.cs}
    /// yield HeightMap.Noise({1: 0.1, 8: 0.3, 512: 0.6 });
    /// @endcode
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
    /// TODO: Overshoot.
    /// 
    /// @param layerDefinitions (Optional) Array defining layers of the noise. If not provided, a default array is used..
    /// @param seed (Optional) Random seed. If not provided, 0 is used. This seed is always combined with the main script seed provided in script arguments to the script.
    /// @return The height map.
    static HeightMap Noise(Array layerDefinitions, Number seed);

    /// Creates a height map with a rectangular area of @a pattern repeating infinitely in all
    /// directions. The repeating pattern will start at point [0, 0].
    /// @param pattern The height map from which the pattern is copied.
    /// @param rectanglePosition The position of top left corner of the rectangle.
    /// @param rectangleSize Size of the rectangle (@link Point.X X@endlink is its width and @link
    /// Point.Y Y@endlink is its height).
    /// @return The height map.
    static HeightMap Pattern(HeightMap pattern, Point rectanglePosition, Point rectangleSize);

    /// Creates a height map with a @a profile projected along @a direction.
    /// @param profile The height profile.
    /// @param direction The projection direction.
    /// @return The height map.
	static HeightMap Projection(HeightProfile profile, Direction direction);

    /// Creates a radial height gradient centered at @a center with @a radius. It goes from @a centerHeight to @a rimHeight. Any pixels outside of the gradient will be filled with @a rimHeight.
    /// @param center The source point.
    /// @param radius The radius. Must not be negative.
    /// @param centerHeight Height at the source.
    /// @param rimHeight Height at the destination.
    /// @return The height map.
    static HeightMap RadialGradient(Point center, Number radius, Number centerHeight, Number rimHeight);

	HeightMap Add(Number/HeightMap addend);
};