/// Random sequence that can generate coordinate-based random number.
class RandomSequence2D
{
private:
public:

    /// Creates a point from a pair of coordinates.
    /// @param x The X coordinate.
    /// @param y The Y coordinate.
    /// @return The point.
    static RandomSequence2D Create(Coordinate x, Coordinate y);
};
