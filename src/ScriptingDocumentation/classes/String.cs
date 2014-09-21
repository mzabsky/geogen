/// Sequence of Unicode characters.
/// 
/// Strings in GGS are only used as @link Array array keys@endlink, @link yield map names@endlink and to @link Print send messages to the API@endlink. Therefore there are no string manipulation functions and operators available. Strings are immutable.
/// 
/// String objects can be created using literals:
/// @code{.cs}
/// var str = "Hello world!";
/// @endcode
/// @ingroup standard_library
class String
{
private:

    /// This fake method is here, because doxygen doesn't like classes without members.
    void FakeMethod();
public:
};
