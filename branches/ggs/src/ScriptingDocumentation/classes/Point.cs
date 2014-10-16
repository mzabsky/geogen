/// Point in two-dimensional space.
/// 
/// A point is composed from two @link Coordinate Coordinates@endlink. These coordinates can be independently either absolute or relative. The @ operator can be used on a point to make both of the contained coordinates relative:
///  
/// Example:
/// @code{.cs}
/// // Assumes max with size 2000x1000.
/// HeightMap.Gradient([500,500], 250, 1, 0); // Creates a gradient centered at point 500, 500.
/// HeightMap.Gradient(@[0.25, 0.50], 250, 1, 0); // The same.
/// HeightMap.Gradient([500, @0.50], 250, 1, 0); // Still the same.
/// @endcode
///         
/// The Point object is immutable, the X and Y accessors can only read the component coordinates. 
///         
/// @ingroup standard_library
class Point
{
private:
public:

    /// Gets the X component of the point's coordinates.
    /// @return The X coordinate.
    Number X {get;};

    /// Gets the Y component of the point's coordinates.
    /// @return The Y coordinate.
    Number Y {get;};

    /// Creates a point from a pair of coordinates.
    /// @param x The X coordinate.
    /// @param y The Y coordinate.
    /// @return The point.
    static Point Create(Coordinate x, Coordinate y);
};
