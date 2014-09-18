/// @page enum_types Enumeratiom types
/// Enumeration type can hold one of a predefined list of values. These types can be either predefined by the runtime(Direction) or defined in the script using the `enum` keyword:
/// @code{.cs}
/// enum KindOfFruit
/// {
///	    Apple,
///	    Orange,
///	    Pear
/// }
/// @endcode
///         
/// Each item is allowed only once, otherwise a compilation error will be generated.
/// 
/// Enum items can be accessed using the dot operator on the enum type:
/// @code{.cs}
/// var orange = KindOfFruit.Orange;
/// if (orange == KindOfFruit.Apple)
/// {
///	    // fails
/// }
/// @endcode
///         
/// Enum items can be converted to and from numeric values - each item corresponds to an integer. These integers can be assigned to individual enum items during declaration:
/// 
/// @code{.cs}
/// enum DayOfWeek
/// {
///	    Monday = 1,
///	    Tuesday = 2,
///	    Wednesday = 3,
///	    Thursday = 4,
///	    Friday = 5,
///	    Saturday = 6,
///	    Sunday = 7
/// }
/// @endcode
///         
/// If these assignments are not made manually, the integer values will be assigned automatically. The first item will be assigned zero and each other item will be assigned numeric value of the previous item plus one. Manual and automatic assignments can be combined. Multiple enum items can correspond to the same numeric value - such items will then be indistinguishable (equal to each other).
/// 
/// @code{.cs}
/// enum Combined
/// {
///	    Zero, // 0
///	    Two = 2,
///	    Three, // 3
///	    Four, // 4
///	    TwoAgain = 2,
///	    ThreeAgain // 3
/// }
///
/// if (Combined.Two == Combined.TwoAgain)
/// {
///	    // passes
/// }
/// @endcode
/// 
/// Negative integer values are allowed.
/// 
/// All enumeration types have predefined constructor `FromNumber`, which takes a numeric value and converts it to an enum value of that type. If there is no enum value with that numeric value, a runtime error is triggered.
/// 
/// Example:
/// @code{.cs}
/// enum KindOfFruit
/// {
///	    Apple = 3,
///	    Orange = 4,
///	    Pear = 5
/// }
/// 
/// var value = KindOfFruit.FromNumber(4); // value will be KindOfFruit.Orange
/// @endcode
