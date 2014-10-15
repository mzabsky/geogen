/// Random sequence which generates a number for each point in 2D space in each step of the sequence. The numbers are determined by the initial seed. This sequence has to be advanced manually.
class RandomSequence2D
{
private:
public:

    /// Creates a new 2D random sequence.
    /// @param seed (Optional) Random seed. If not provided, 0 is used. This seed is always combined with the main script seed provided in script arguments to the script.
    /// @return The sequence.
    static RandomSequence2D Create(Number seed);

    /// Advances the sequence - until the sequence is advanced, the same number will always be generated for the same coordinates.
    void Advance();

    /// Returns a number corresponding to point [@a coordinate, 0], in range <@a min, @a max>. Does not advance the sequence.
    /// @param point The point on which the random number is based.
    /// @param min (Optional) The lower bound of the range, inclusive. If not specified, 0 is used.
    /// @param max (Optional)The higher bound of the range, inclusive. If not specified, 1 is used. If @a min is specified, this must be specified too. Must be greater than @a min.
    /// @return The number.
    Number GetNumber(Point point, Number min, Number max);
};
