/// A coordinate in 1-dimensional space.
/// 
/// A coordinate can be either absolute or relative:
/// - @b Absolute coordinate is specific coordinate in 1D space - for example 500.  
/// - @b Relative coordinate is a proportion of the map size in one of its dimensions. For example, a relative coordinate 0.5 will evaluate to 500 on a 2000x1000 map in its vertical dimension. The dimension must be supplied when a relative coordinate is being evaluated, otherwise a runtime error will be triggered.
/// 
/// Coordinates can be created using the [] operator from a Number, but that is not necessary, because a Number will be @link implicit_conversions automatically converted @endlink to a absolute coordinate where appropriate. 
/// 
/// Example:
/// 
/// @code{.cs}
/// HeightProfile.Slice(heightMap, Direction.Vertical, 500); // 500 is automatically converted to a absolute coordinate
/// HeightProfile.Slice(heightMap, Direction.Vertical, [500]); // This is effectively the same
/// HeightProfile.Gradient(500, 800, 1, 0); // Creates a gradient from coordinate 500 to 800
/// @endcode
/// 
/// A relative coordinate can be created from an absolute coordinate or a Number using the @ operator:
/// 
/// @code{.cs}
/// // Assumes the map size is 2000x1000
/// HeightProfile.Slice(heightMap, Direction.Vertical, @0.5); // The 0.5 is automatically converted to an absolute coordinate, which is then turned into a relative coordinate, which is then evaluated by HeightMap.Slice into 500 (because it knows direction of the coordinate from its second argument).
/// HeightProfile.Slice(heightMap, Direction.Vertical, @[500]); // This works too
/// HeightProfile.Gradient(@0.5, @0.8, 1, 0); // This will trigger error, because the Gradient function doesn't have a direction specified.
/// @endcode
/// 
/// Relative coordinates can be evaluated explicitly using Number.FromCoordinate.
///
/// Two coordinates can be combined into a Point.
/// @ingroup standard_library
class Coordinate
{
private:
public:

    /// Creates an absolute coordinate from a @a number.
    /// @param number The number.
    /// @return The coordinate.
    static Number FromNumber(Number number);
};
