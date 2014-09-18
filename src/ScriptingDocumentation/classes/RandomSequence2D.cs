/// Random sequence that can generate coordinate-based random numbers.
class RandomSequence2D
{
private:
public:

    /// Creates a new 2D random sequence.
    /// @param x The X coordinate.
    /// @param y The Y coordinate.
    /// @return The point.
    static RandomSequence2D Create(Number randomSeed);
};
