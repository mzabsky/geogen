/// Two-dimensional height map.
/// X coordinates go from left (negative) to right (positive) in the horizontal direction and Y coordinates go from top (negative) to bottom (positive) in the vertical direction.
/// 
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
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Noise();
    /// yield HeightMap.ConvexityMap(main, 50) as "ConvexityMap";
    /// yield main;
    /// @endcode
    ///         
    /// @image html std_2d_convexitymap.png
    /// 	   
    /// @param heightMap The height map.
    /// @param radius The radius. Must be greater than 0.
    /// @return The convexity map.
	static HeightMap ConvexityMap(HeightMap heightMap, Number radius);

    /// Creates a map representing euclidean distance of each pixel of a height map to the closest pixel with height 0 or less. 
    /// 
    /// The distance is capped at @a maximumDistance. Each pixel will have height proportional to its distance to the closest pixel with height 0 or less. Pixels with distance @a maximumDistance or greater will have height 1.
    /// 
    /// Example:
    /// @code{.cs}
    /// var noise = HeightMap.Noise(CreateNoiseLayers(64, 256));
    /// yield HeightMap.DistanceMap(noise, 250);
    /// @endcode
    ///         
    /// @image html std_2d_distancemap.png
    /// 
    /// @see [Distance transform on Wikipedia](http://en.wikipedia.org/wiki/Distance_transform)
    /// 
    /// @param heightMap The height map.
    /// @param maximumDistance The maximum distance considered. Must be greater than 0.
    /// @return The distance map.
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
    /// @param layerDefinitions (Optional) Array defining layers of the noise. If not provided, a default array is used. This array can be created using CreateNoiseLayers.
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
    /// @param firstRectanglePoint First point defining the pattern rectangle.
    /// @param secondRectanglePoint Second point defining the pattern rectangle.
    /// @return The height map.
    static HeightMap Pattern(HeightMap pattern, Point firstRectanglePoint, Point secondRectanglePoint);

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
    /// 	   
    /// RadialGradient can be combined with Multiply to create a cone with flat top:
    ///
    /// @code{.cs}
    /// yield HeightMap.RadialGradient([500, 500], 250, 1, -1).Multiply(3);
    /// @endcode
    /// 
    /// @image html std_2d_radialgradient_cone.png
    /// 	   
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
    /// @return The height map itself (for call chaining).
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
    /// @image html std_2d_clampheights.png
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
    /// @image html std_2d_combine.png
    ///        
    /// @param other The other map.
    /// @param mask The mask.
    /// @return The height map itself (for call chaining).
    HeightMap Combine(HeightMap other, HeightMap mask);

    /// Replaces all heights outside of an rectangle with @a replace.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.RadialGradient([500, 500], 400, 1, 0).Crop([350, 250], [650, 750]);
    /// @endcode
    /// 
    /// @image html std_2d_crop.png
    /// @param firstRectanglePoint First point defining the rectangle.
    /// @param secondRectanglePoint Second point defining the rectangle.
    /// @param replace (Optional) The replacement height. 0 is used if not provided.
    /// @return The height map.
    HeightMap Crop(Point firstRectanglePoint, Point secondRectanglePoint, Number replace);

    /// Replaces heights greater than @a max or less than @a min with @a replace.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().ClampHeights(-0.3, 0.3);
    /// @endcode
    /// 
    /// @image html std_2d_cropheights.png
    ///        
    /// @param min The minimum height.
    /// @param max The maximum height.
    /// @param replace (Optional) The replacement height. 0 is used if not provided.
    /// @return The height map itself (for call chaining).
    HeightMap CropHeights(Number min, Number max, Number replace);

    /// Distorts the map with random perturbations.
    /// 
    /// Example:
    /// @code{.cs}
    /// var profile = HeightProfile.Noise();
    /// var main = HeightMap.Projection(profile, Direction.Vertical);
    /// main.Rotate(DegToRad(30));
    /// main.Distort(20, 64);
    /// yield main;
    /// @endcode
    /// 
    /// @image html std_2d_distort.png
    ///        
    /// @param perturbationSize Wave length of the perturbation distance.
    /// @param maximumDistance Maximum distance by which a pixel can be moved.
    /// @param seed (Optional) Random seed. If not provided, 0 is used. This seed is always combined with the main script seed provided in script arguments to the script.
    /// @return The height map itself (for call chaining).
    HeightMap Distort(Number perturbationSize, Number maximumDistance, Number seed);

    /// Draws a 1 pixel wide line from @a start to @a end.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Flat(1);
    /// 
    /// // Draw the star from lines
    /// var angleStep = 360 / 16;
    /// for(var angle = 0; angle < 360; angle += angleStep)
    /// {
    ///     var point = [
    ///         500 + Cos(DegToRad(angle)) * 390,
    ///         500 + Sin(DegToRad(angle)) * 390];
    /// 
    ///     main.DrawLine([500, 500], point, 0);
    /// }
    /// 
    /// // Stroke the star
    /// yield HeightMap.DistanceMap(main, 100);
    /// @endcode
    /// 
    /// @image html std_2d_drawline.png
    ///        
    /// @param start One of the ending points.
    /// @param end The other ending point.
    /// @param height The height.
    /// @return The height map itself (for call chaining).
    HeightMap DrawLine(Point start, Point end, Number height);

    /// Replaces each pixel with @a height.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().Fill(0.5);
    /// @endcode
    /// 
    /// @image html std_2d_fill.png
    ///        
    /// @param height The height.
    /// @return The height map itself (for call chaining).
    HeightMap Fill(Number height);

    /// Replaces each pixel in a rectangle with @a height.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().FillRectangle([200, 200], [600, 800], 0.5);
    /// @endcode
    /// 
    /// @image html std_2d_fillrectangle.png
    ///        
    /// @param firstRectanglePoint First point defining the rectangle.
    /// @param secondRectanglePoint Second point defining the rectangle.
    /// @param height The height.
    /// @return The height map itself (for call chaining).
    HeightMap FillRectangle(Point firstRectanglePoint, Point secondRectanglePoint, Number height);

    /// Flips the map horizontally of vertically. The transformation origin is [0, 0].
    /// 
    /// Example:
    /// @code{.cs}
    /// // Create the main ridge around row 0, because flip origin is [0, 0]
    /// var main = HeightMap.Gradient([0, -300], [0, 300], 0, 1);
    /// var profile = HeightProfile.Noise({64: 0.9 });
    /// main.Shift(profile, 150, Direction.Vertical);
    ///
    /// var flipped = HeightMap.Clone(main).Flip(Direction.Vertical);
    /// main.Intersect(flipped);
    /// main.Move([0,0500]);
    /// yield main;
    /// @endcode
    /// 
    /// @image html std_2d_flip.png
    /// 
    /// @param direction The direction.
    /// @return The height map itself (for call chaining).
    HeightMap Flip(Direction direction);

    /// Exaggerates lower altitudes, making valley bases wider and peaks sharper.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Noise();
    /// yield main.Multiply(1.45).Glaciate(0.25, true);
    /// @endcode
    /// 
    /// @image html std_2d_glaciate_25.png
    ///        
    /// The same height map with @a strength 1 and @a includeNegative set to `false`.
    ///
    /// @image html std_2d_glaciate_100.png
    ///        
    /// @param strength Strength of the effect. Must be greater than or equal to 0 and less than or equal to 1.
    /// @param includeNegative True if the effect is to be applied on negative heights. If false, negative heights will be ignored. The effect is mirrored on negative heights.
    /// @return The height map itself (for call chaining).
    HeightMap Glaciate(Number strength, bool includeNegative);

    /// Sets each pixel in the to the less of the two corresponding heights in the current map and the other map.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Gradient([0, 0], [1000, 0], 0, 1);
    /// var other = HeightMap.Gradient([0, 0], [0, 1000], 0, 1);
    /// yield main.Intersect(other);
    /// @endcode
    /// 
    /// @image html std_2d_intersect.png
    ///        
    /// @param other The other map.
    /// @return The height map itself (for call chaining).
    HeightMap Intersect(HeightMap other);

    /// Flips sign of each height in the map.
    /// 
    /// Example:
    /// @code{.cs}
    /// // Transforms absolute value noise into ridges
    /// yield HeightMap.Noise().Abs().Invert().Add(1);
    /// @endcode
    /// 
    /// @image html std_2d_invert.png
    /// @return The height map itself (for call chaining).
    HeightMap Invert();

    /// Moves the height map by offset.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Noise({256:1});
    /// var copy = HeightMap.Clone(main).Move([150, 100]);
    /// yield main.Unify(copy);
    /// @endcode
    /// 
    /// @image html std_2d_move.png
    ///        
    /// @param offset The offset. The map is moved by both of the @a offset's component coordinates in the respective direction.
    /// @return The height map itself (for call chaining).
    HeightMap Move(Point offset);

    /// Multiplies the height in each pixel by a number or a corresponding pixel from another height map. 
    /// 
    /// Example with a numeric factor:
    /// @code{.cs}
    /// yield HeightMap.Noise().Multiply(0.2);
    /// @endcode
    ///       
    /// @image html std_2d_multiply_number.png
    ///         
    /// Example with map factor:
    /// @code{.cs}
    /// var main = HeightMap.Noise();
    /// var multiplier = HeightMap.RadialGradient([500, 500], 400, 1, 0.2);
    /// yield main.Multiply(multiplier);
    /// @endcode
    /// 
    /// @image html std_2d_multiply_map.png
    /// 
    /// @param factor The factor.
    /// @return A HeightMap.
	HeightMap Multiply(Number/HeightMap factor);

    /// Enlarges (with scale &gt; 1) or shrinks (with scale &lt; 1) the entire map. The scaling
    /// transformation has origin at [0, 0]. Uses linear interpolation.
    /// 
    /// Example:
    /// @code{.cs}
    /// // Create the template cone at [0, 0], otherwise its position would be scaled along with its size
    /// var template = HeightMap.RadialGradient([0, 0], 300, 1, 0);
    /// 
    /// var main = HeightMap.Flat();
    /// 
    /// var cone = HeightMap.Clone(template).Move([200, 500]);
    /// main.Unify(cone);
    /// 
    /// cone = HeightMap.Clone(template).Rescale(0.80).Multiply(0.80).Move([400, 500]);
    /// main.Unify(cone);
    /// 
    /// cone = HeightMap.Clone(template).Rescale(0.60).Multiply(0.60).Move([600, 500]);
    /// main.Unify(cone);
    /// 
    /// cone = HeightMap.Clone(template).Rescale(0.40).Multiply(0.40).Move([800, 500]);
    /// main.Unify(cone);
    /// 
    /// yield main;
    /// @endcode
    /// 
    /// @image html std_2d_rescale.png
    ///        
    /// @param horizontalScale The horizontal scale ratio. Must be greater than 0.1 and less than 10.
    /// @param verticalScale (Optional) The vertical scale ratio. If not provided, it will be the same as @a horizontalScale. Must be greater than 0.1 and less than 10.
    /// @return The height map itself (for call chaining).
    HeightMap Rescale(Number horizontalScale, Number verticalScale);

    /// Rotates the height map by @a angle in radians clockwise. The rotation origin is at [0, 0].
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Flat();
    ///
    /// // Make the rectangle centered at [0, 0], because that point is the rotation origin.
    /// main.FillRectangle([-300, -100], [600, 200], 1);
    /// main.Rotate(DegToRad(45));
    /// main.Move([500, 500]);
    /// yield main;
    /// @endcode
    /// 
    /// @image html std_2d_rotate.png
    ///        
    /// @param angle The angle in radians.
    /// @return The height map itself (for call chaining).
    HeightMap Rotate(Number angle);

    /// Shears the height map by @a shearFactor in @a direction.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Flat();
    /// main.FillRectangle([200, 400], [600, 200], 1);
    /// main.Shear(0.4, Direction.Vertical);
    /// yield main;
    /// 
    /// @endcode
    /// 
    /// @image html std_2d_shear.png
    ///        
    /// @see [Shear mapping on Wikipedia](http://en.wikipedia.org/wiki/Shear_mapping)
    ///        
    /// @param shearFactor The shear factor.
    /// @param direction The direction.
    /// @return The height map itself (for call chaining).
    HeightMap Shear(Number shearFactor, Direction direction);

    /// Shifts each row (if @a direction is vertical) or column (if @a direction is horizontal) by
    /// distance determined by the corresponding height in the profile (1 in profile means the pixels
    /// will be shifted by @a maximumDistance, -1 means the pixels will be shifted by -@a
    /// maximumDistance).
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Gradient([200, 0], [800, 0], 0, 1);
    /// var profile = HeightProfile.Noise({256: 0.7, 168: 0.3 });
    /// main.Shift(profile, 300, Direction.Horizontal);
    /// yield main;
    /// @endcode
    /// 
    /// @image html std_2d_shift.png
    ///        
    /// @param profile The shift profile.
    /// @param maximumDistance The maximum shift distance.
    /// @param direction The direction.
    /// @return The height map itself (for call chaining).
    HeightMap Shift(HeightProfile profile, Coordinate maximumDistance, Direction direction);

    /// Distributes the heights among a specified number of strata, creating a terraced terrain.
    /// 
    /// Example:
    /// @code{.cs}
    /// yield HeightMap.Noise().Stratify(12, 0.5, 0.5, true);
    /// @endcode
    /// 
    /// @image html std_2d_stratify.png
    ///        
    /// @param numberOfStrata Number of strata. Must be greater than or equal to 1 and less than or equal to 100.
    /// @param steepness (Optional) Steepness of the cliffs. Must be greater than or equal to 0 and less than or equal to 1. Greater value means the cliffs are closer to vertical and the flat surfaces are flatter. 0.5 is used if not specified.
    /// @param smoothness (Optional) Smoothness of edges of the cliffs. Must be greater than or equal to 0 and less than or equal to 1. 0.5 is used if not specified.
    /// @param includeNegative (Optional) True if the effect is to be applied on negative heights. If false, negative heights will be ignored. @c true is used if not specified.
    /// @return The height map itself (for call chaining).
    HeightMap Stratify(Number numberOfStrata, Number steepness, Number smoothness, Boolean includeNegative);

    /// Applies a linear transformation on the height map - each pixel pixel's position vector will
    /// be be multiplied by the transformation matrix composed from cells @a a11, @a a12, @a a21 and
    /// @a a22. The transformation matrix must be invertible.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Flat();
    /// main.FillRectangle([-300, -100], [600, 200], 1);
    /// 
    /// // The matrix combine 45 degree rotation, 0.4 horizontal shear and 0.5 vertical shear.
    /// main.Transform(1.06066, 0.989949, -0.207183, 0.717005);
    /// main.Move([500, 500]);
    /// yield main;
    /// @endcode
    /// 
    /// @image html std_2d_transform.png
    /// @param a11 The top-left cell of the transformation matrix.
    /// @param a12 The top-right cell of the transformation matrix.
    /// @param a21 The bottom-left cell of the transformation matrix.
    /// @param a22 The bottom-right cell of the transformation matrix.
    /// @return The height map itself (for call chaining).
    HeightMap Transform(Number a11, Number a12, Number a21, Number a22);

    /// Maps the height in each pixel from the height map to another height, using a height profile as a transformation function. Height @a minHeight will map to height in @a profile at coordinate @a starting coordinate and height @a maxHeight will map to height at coordinate @a endingCordinate. Any heights in between will map to height at appropriate coordinate in between.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.RadialGradient([500, 500], 800, 1, 0);
    /// main.TransformHeights(HeightProfile.Noise({64: 0.9}), 0, 1000);
    /// yield main;
    /// @endcode
    /// 
    /// @image html std_2d_transformheights.png
    ///        
    /// @param profile The transformation profile.
    /// @param startingCoordinate (Optional) Starting coordinate of the interval of the transformation function used for mapping. If not provided, 0 is used.
    /// @param endingCoordinate (Optional) Ending coordinate of the interval of the transformation function used for mapping. If not provided, 1000 is used.
    /// @param minHeight (Optional) Minimum height affected by the transformation. If not provided, 0 is used.
    /// @param maxHeight (Optional) Maximum height affected by the transformation. If not provided, 1 is used.
    /// @return The height map itself (for call chaining).
    HeightMap TransformHeights(HeightProfile profile, Coordinate startingCoordinate, Coordinate endingCoordinate, Number minHeight, Number maxHeight);

    /// Sets each pixel in the to the greater of the two corresponding heights in the current map and the other map.
    /// 
    /// Example:
    /// @code{.cs}
    /// var main = HeightMap.Gradient([0, 0], [1000, 0], 0, 1);
    /// var other = HeightMap.Gradient([0, 0], [0, 1000], 0, 1);
    /// yield main.Unify(other);
    /// @endcode
    /// 
    /// @image html std_2d_unify.png
    ///        
    /// @param other The other map.
    /// @return The height map itself (for call chaining).
    HeightMap Unify(HeightMap other);
};