/// @page type_system Type system
/// GGS is dynamically typed, therefore all type checks are done at runtime using type information stored in individual objects.
/// 
/// All objects are passed by reference, however most primitive types like Number or Boolean are immutable (and therefore behave as expected).
/// 
/// All objects can be considered to implement type Object. Other than that, there is no inheritance in GGS.
///
/// @section primitive_types Primitive types
/// The concept of a primitive type doesn't hold any particular significance in GGS. Following types correspond to the usual primitive types found in other languages:
/// @li Boolean - Either true or false logical value.
/// @li Number - Double precision floating point number as specified by the [IEEE 754](http://en.wikipedia.org/wiki/IEEE_floating_point) standard.
/// @li String - A series of Unicode characters of arbitrary length. Strings are only used to as keys in arrays, to specify map names in yield statements and to pass text messages to the API. There aren't any string manipulation functions in the standard library.
/// @section implicit_conversions Implicit conversions
/// Only a single implicit conversion is supported: Number to Coordinate. A Number can therefore be used anywhere where a Coordinate can be used.
/// 
/// @section null Null
/// Null represents "no value". Passing the null into a type-checked native function or using it as an operand to most operators will result in a runtime error being generated.
/// 
/// Variables declared without being assigned a value have a null value. Null can also be assigned to any writable variable to effectively unset its value.
/// 
/// @code{.cs}
/// var a;
/// var b = null;
/// if (a == b){
///	    // This condition is true.
/// }
/// @endcode
/// Attempt to access member functions or member variables of a null value will always result in an runtime error.
