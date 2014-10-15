/// @defgroup standard_library Standard library
/// @{

/// Returns the least of any number of numbers.
/// @code{.cs}
/// var a = Min(5, 6.16, -1.35, 69, 0.0036); // a will be -1.35
/// @endcode
/// @param n1 The first number.
/// @param n2 The second number.
/// @return The minimum number.
Number Min(Number n1, Number n2, ...);

/// Returns the greatest of any number of numbers.
/// @code{.cs}
/// var a = Max(5, 6.16, -1.35, 69, 0.0036); // a will be 69
/// @endcode
/// @param n1 The first number.
/// @param n2 The second number.
/// @return The maximum number.
Number Max(Number n1, Number n2, ...);

/// Returns value of sine of @a x radians.
/// @code{.cs}
/// var a = Sin(DegToRad(30)); // a will be 0.5
/// @endcode
/// @param x The angle in radians.
/// @return Sine of @a x.
Number Sin(Number x);

/// Returns value of cosine of @a x radians.
/// @code{.cs}
/// var a = Cos(0); // a will be 1
/// @endcode
/// @param x The angle in radians.
/// @return Cosine of @a x.
Number Cos(Number x);

/// Returns value of tangent of @a x radians.
/// @code{.cs}
/// var a = Tan(DegToRad(45)); // a will be 1
/// @endcode
/// @param x The angle in radians.
/// @return Tangent of @a x.
Number Tan(Number x);

/// Returns value of arc sine of @a x.
/// @code{.cs}
/// var a = RadToDeg(Asin(1)); // a will be 90
/// @endcode
/// @param x The angle in radians. Must be greater than -1 and less than 1.
/// @return Arc sine of @a x, expressed in radians.
Number Asin(Number x);

/// Returns value of arc cosine of @a x.
/// @code{.cs}
/// var a = RadToDeg(Acos(1)); // a will be 0
/// @endcode
/// @param x The angle in radians. Must be greater than -1 and less than 1.
/// @return Arc cosine of @a x, expressed in radians
Number Acos(Number x);

/// Returns value of arc tangent of @a x.
/// @code{.cs}
/// var a = RadToDeg(Atan(1)); // a will be 45
/// @endcode
/// @param x The angle in radians. 
/// @return Arc tangent of @a x, expressed in radians
Number Atan(Number x);

/// Returns value of arc tangent of @a y/@a x. To compute the value, the function takes into account the sign of both arguments in order to determine the quadrant (unlike Atan). At least @a x or @a y must be non-zero.
/// @code{.cs}
/// var a = RadToDeg(Atan2(1, 2)); // a will be ~26.57
/// @endcode
/// @param y Proportion of the @a y coordinate.
/// @param x Proportion of the @a x coordinate. 
/// @return Principal arc tangent of @a y/@a x, expressed in radians.
Number Atan2(Number x, Number y);

/// Converts angle of @a x degrees to radians.
/// @code{.cs}
/// var a = Sin(DegToRad(30)); // a will be 0.5
/// @endcode
/// @param x The angle in degrees. 
/// @return @a x converted to radians.
Number DegToRad(Number x);

/// Converts angle of @a x radians to degrees.
/// @code{.cs}
/// var a = RadToDeg(Asin(1)); // a will be 90
/// @endcode
/// @param x The angle in radians. 
/// @return @a x converted to degrees.
Number RadToDeg(Number x);

/// Returns the value of logarithm of @a x in @a base.
/// @code{.cs}
/// var a = Log(100, 10); // a will be 2
/// @endcode
/// @param x The x. Must be greater than zero.
/// @param base The base. Must be greater than zero.
/// @return Logarithm of @a x in @a base.
Number Log(Number x, Number base);

/// Returns the value of @a x raised to @a power.
/// @code{.cs}
/// var a = Pow(2, 3); // a will be 8
/// @endcode
/// @param x The x.
/// @param power The power.
/// @return @a x raised to @a base.
Number Pow(Number x, Number power);

/// Returns the value of square root of @a x.
/// @code{.cs}
/// var a = Sqrt(100); // a will be 10
/// @endcode
/// @param x The x. Must be greater than zero.
/// @return Square root of @a x.
Number Sqrt(Number x);

/// Returns the smallest integral number greater than @a x.
/// @code{.cs}
/// var a = Ceil(5.2); // a will be 6
/// @endcode
/// @param x The x.
/// @return @a x rounded up.
Number Ceil(Number x);

/// Returns the smallest integral number lesser than @a x.
/// @code{.cs}
/// var a = Floor(5.2); // a will be 5
/// @endcode
/// @param x The x.
/// @return @a x rounded down.
Number Floor(Number x);

/// Returns the integer closest to @a x. 0.5 is rounded up.
/// @code{.cs}
/// var a = Ceil(5.2); // a will be 5
/// @endcode
/// @param x The x.
/// @return @a x rounded to nearest integer.
Number Round(Number x);

/// Returns the absolute value of @a x.
/// @code{.cs}
/// var a = Abs(-13); // a will be 13
/// @endcode
/// @param x The x.
/// @return Absolute value of @a x.
Number Abs(Number x);

/// Sends a text @a message to the API. If any arguments are provided, any formatting marks in @a message will
/// be replaced with appropriate string representation of corresponding argument.
/// @code{.cs}
/// Print("The temperature is {0}° of {1}.", 35, "Celsius"); // Will print "The temperature is 35° of Celsius."
/// @endcode
/// @param message The message.
/// @param arg1 (Optional) The first argument.
/// @param arg2 (Optional) The second argument.
Print(String message, Object arg1, Object arg2, ...);

/// Terminates execution of the script with a text @a message. If any arguments are provided, any formatting marks in @a message will
/// be replaced with appropriate string representation of corresponding argument.
/// @code{.cs}
/// TriggerError("The number of islands must be an even number, was {0}.", Parameters.NumberOfIslands); // Will trigger error GGE5000 "The number of islands must be an even number, was 5."
/// @endcode
/// @param message The message.
/// @param arg1 (Optional) The first argument.
/// @param arg2 (Optional) The second argument.
TriggerError(String message, Object arg1, Object arg2, ...);

/// Returns a random number from specified range.
/// @code{.cs}
/// var a = Random(0, 10); // a will be eg. ~5.6841
/// a = Random(0, 10); // a will be ~0.6978
/// a = Random(0, 10); // a will be ~8.1993
/// a = Random(0, 10); // a will be ~3.1698
/// @endcode
/// @param min (Optional) The lower bound of the range, inclusive. If not specified, 0 is used.
/// @param max (Optional) The higher bound of the range, inclusive. If not specified, 1 is used. If @a min is specified, this must be specified too. Must be greater than @a min.
/// @return Random number.
Number Random(Number min, Number max);

/// @}
 