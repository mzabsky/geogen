/// Double precision floating point number as specified by the [IEEE 754](http://en.wikipedia.org/wiki/IEEE_floating_point) standard. This class is used to represent both integers and non-integers - there is no dedicated integer type. 
/// 
/// The range of allowed numbers is -1.7*10<sup>308</sup> to 1.7*10<sup>308</sup>.
/// 
/// Number supports @link operators all operators@endlink expected from a numeric type in a programming language. If operation would result in an non-number value (NaN/infinity) a runtime error will be triggered instead.
/// 
/// Number object can be created using a literal:
/// 
/// @code{.cs}
/// var a = 5.68;
/// var b = 3 + b;
/// @endcode
/// @ingroup standard_library
class Number
{
private:
public:

    /// Converts @a coordinate to a number. If the @a coordinate is relative, @a direction has to be specified. If @a coordinate is absolute, @a direction will be ignored.
    /// @param coordinate The coordinate.
    /// @param direction (optional) The direction.
    /// @return Numeric value of @a coordinate. If @a coordinate was relative, it will be converted to an absolute coordinate.
    static Number FromCoordinate(Coordinate coordinate, Direction direction);

    /// Converts an @a enumValue to its corresponding numeric value. @a Enum can be any @link enum_types enumeration type@endlink.
    /// @param enumValue The enum value.
    /// @return Numeric value of @a enumValue.
    static Number FromEnum(Enum enumValue);
};
