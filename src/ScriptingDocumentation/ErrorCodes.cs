/// @page error_codes Error codes
/// 
/// This page contains a reference of all error codes produced by GeoGen. 
/// 
/// @li @ref gge1101
/// @li @ref gge1102
/// @li @ref gge1201
/// @li @ref gge1301
/// @li @ref gge1303
/// @li @ref gge1304
/// @li @ref gge1306
/// @li @ref gge1307
/// @li @ref gge1308
/// @li @ref gge1309
/// @li @ref gge1310
/// @li @ref gge1311
/// @li @ref gge1312
/// @li @ref gge1401
/// @li @ref gge1402
/// @li @ref gge1403
/// @li @ref gge1405
/// @li @ref gge1406
/// @li @ref gge1407
/// @li @ref gge1408
/// @li @ref gge1409
/// @li @ref gge1410
/// @li @ref gge1411
/// @li @ref gge1412
/// @li @ref gge2101
/// @li @ref gge2102
/// @li @ref gge2103
/// @li @ref gge2104
/// @li @ref gge2106
/// @li @ref gge2107
/// @li @ref gge2108
/// @li @ref gge2201
/// @li @ref gge2202
/// @li @ref gge2301
/// @li @ref gge2302
/// @li @ref gge2303
/// @li @ref gge2304
/// @li @ref gge2305
/// @li @ref gge2306
/// @li @ref gge2307
/// @li @ref gge2308
/// @li @ref gge2309
/// @li @ref gge2501
/// @li @ref gge2502
/// @li @ref gge2503
/// @li @ref gge2601
/// @li @ref gge2602
/// @li @ref gge2603
/// @li @ref gge2604
/// @li @ref gge2701
/// @li @ref gge2702
/// @li @ref gge2703
/// @li @ref gge2704
/// @li @ref gge2705
/// @li @ref gge2706
/// @li @ref gge2707
/// @li @ref gge2708
/// @li @ref gge2709
/// @li @ref gge2710
/// @li @ref gge2711
/// @li @ref gge2712
/// @li @ref gge2713
/// @li @ref gge2714
/// @li @ref gge2801
/// @li @ref gge3000
/// @li @ref gge4000
/// @li @ref gge4001
/// @li @ref gge5000


/// @page gge1101 GGE1101
/// The parser has encountered an unexpected character.
/// 
/// This error is usually caused by a character that is not supported by the parser in any context:
/// @code{.cs}
/// var variablě = 5; // GGE1101 caused by the "ě"
/// 
/// It can also be caused by a newline missing at the end of the script.
/// @endcode

/// @page gge1102 GGE1102
/// The parser has encountered invalid escape sequence.
/// 
/// This error is usually caused by an backslash followed by character not matching any of the supported @link literals escape sequences@endlink:
/// @code{.cs}
/// var str = "Hello \x world!"; // GGE1102 caused by the "x"
/// @endcode

/// @page gge1201 GGE1201
/// The parser has encountered unexpected token.
/// 
/// This error is caused by syntax errors. Common causes include:
/// 
/// @li Missing semicolon after an statement:
/// @code{.cs}
/// var str = "Hello world!" // GGE1201
/// var str = "Another hello world!";
/// @endcode
///     
/// @li Missing ending parenthesis:
/// @code{.cs}
/// var n = ((5 + 2) * 3 / 5; // GGE1201
/// @endcode
///     
/// @li Misplaced keyword (or a keyword being used as a symbol name):
/// @code{.cs}
/// function if(x) // GGE1201
/// {
///     return x == true;
/// }
/// 
/// var y = if(false);
/// @endcode
///         
/// @li Malformed statement:
/// @code{.cs}
/// for(var i = 0; i++) // GGE1201
/// {
///     x++;
/// }
/// @endcode

/// @page gge1301 GGE1301
/// The parser has encountered a `break` statement outside of a loop.
/// 
/// @code{.cs}
/// for(var i = 0; i < 10; i++)
/// {
///     if(i == 5)
///     {
///         break;
///     }
/// }
/// 
/// break; // GGE1301
/// @endcode

/// @page gge1303 GGE1303
/// The parser has encountered a `continue` statement outside of a loop.
/// 
/// @code{.cs}
/// for(var i = 0; i < 10; i++)
/// {
///     if(i == 5)
///     {
///         continue;
///     }
/// }
/// 
/// continue; // GGE1303
/// @endcode

/// @page gge1304 GGE1304
/// The parser has encountered a `return` statement outside of a function body.
/// 
/// Note that `return` is not used to "return" maps from the script, @link yield `yield` statement@endlink is used instead for that. 
/// 
/// @code{.cs}
/// return HeightMap.Flat(); // GGE1304
/// @endcode
     
/// @page gge1306 GGE1306
/// The script has declared a function which with a name which was already used by another function (either predefined or defined previously by the script).
/// 
/// Full list of the predefined functions can be found in the @link standard_library Standard library@endlink section.
/// 
/// @code{.cs}
/// function MyFunction()
/// {
///     return 5;
/// }
/// 
/// function Sin(x) // GGE1306
/// {
///     return 0;
/// }
/// 
/// function MyFunction() // GGE1306
/// {
///     return 6;
/// }
/// @endcode

/// @page gge1307 GGE1307
/// An read only expression was used as a left-expression with a assignment operator such as =, += or ++. Only variable references and array item references are legal as left-expression.
/// 
/// @code{.cs}
/// var a = 5;
/// var b = 3;
/// var array = {1: 11, 2: 22};
/// 
/// a += 5; // OK
/// Sin(a) += 5; // GGE1307, function call is not a legal left-expression
/// array[5]++; // OK
/// a + b = 3; // GGE1307, sum is not a legal left-expression
/// @endcode

/// @page gge1308 GGE1308
/// The script attempted to define an @link enum_types enum type@endlink with a name already used another class (either predefined or script defined).
/// 
/// @code{.cs}
/// enum MyEnum { A, B }
/// enum HeightMap { A, B } // GGE1308
/// enum MyEnum { A, B } // GGE1308
/// @endcode

/// @page gge1309 GGE1309
/// The script defined an enum item with the same name as another item of the same @link enum_types enum type@endlink.
/// 
/// @code{.cs}
/// enum MyEnum
/// {
///     A,
///     B,
///     C,
///     A, // GGE1309
///     D
/// }
/// @endcode

/// @page gge1310 GGE1310
/// The script defined an enum item with a numeric value that was not an iteger.
/// 
/// @code{.cs}
/// enum MyEnum
/// {
///     A = 2,
///     B = 2.5, // GGE1310
///     C = 3
/// }
/// @endcode

/// @page gge1311 GGE1311
/// The script defined an @link enum_types enum type@endlink with a name that is reserved by the language. Currently, the only reserved name is "Type".
/// 
/// @code{.cs}
/// enum Type // GGE1311
/// {
///     A = 1,
///     B = 2,g
///     C = 3
/// }
/// @endcode

/// @page gge1312 GGE1312
/// The script didn't contain a @link yield `yield` statement@endlink returning the primary map. 
/// 
/// @code{.cs}
/// yield HeightMap.Flat() as "SomeName"; // GGE1312 if this is the only yield in the script
/// @endcode

/// @page gge1401 GGE1401
/// The script didn't contain a @link yield `yield` statement@endlink returning the primary map. 
/// 
/// @code{.cs}
/// yield HeightMap.Flat() as "SomeName"; // GGE1312 if this is the only yield in the script
/// @endcode

/// @page gge1402 GGE1402
/// The `Parameters` node of @link metadata script metadata@endlink was not key-value collection node. 
/// 
/// @code{.cs}
/// metadata
/// {
///     Parameters: 5 // GGE1402
/// }
/// @endcode

/// @page gge1403 GGE1403
/// Node of @link metadata script metadata@endlink node representing a single script parameter was not key-value collection node. 
/// 
/// @code{.cs}
/// metadata
/// {
///     Parameters: 
///     {
///         Smoothness: "Hello world!" // GGE1403
///     }
/// }
/// @endcode

/// @page gge1405 GGE1405
/// Script parameter defined in @link metadata script metadata@endlink used name reserved by the language.
/// 
/// Currently, the reserved script parameter names are :
/// @verbatim Width, Height, RenderOriginX, RenderOriginY, RenderWidth, RenderHeight @endverbatim
/// 
/// Map width and height can be limited using the dedicated @link map_size `Width` and `Height` nodes@endlink.
/// 
/// @code{.cs}
/// metadata
/// {
///     Parameters: 
///     {
///         Height: { Min: 100, Max: 200 } // GGE1405
///     }
/// }
/// @endcode

/// @page gge1406 GGE1406
/// An attribute of a script parameter defined in @link metadata script metadata@endlink was a node of a different type than expected.
/// 
/// Expected types of all parameter definition subnodes see @link script_parameters Script parmeters @endlink section.
/// 
/// @code{.cs}
/// metadata
/// {
///     Parameters:
///     {
///         ExampleParameter: { Label: 5, Min: 5, Max: 10 } // GGE1406, Label is expected to be of type String, not Number.
///     }
/// }
/// @endcode

/// @page gge1407 GGE1407
/// Script parameter defined in @link metadata script metadata@endlink had an invalid @link script_parameters numeric restriction@endlink.
/// 
/// Allowed restrictions are: `Unrestricted`, `Integers`, `PowersOf2` and `PowersOf10`.
/// 
/// @code{.cs}
/// metadata
/// {
///     Parameters:
///     {
///         ExampleParameter: { Min: 5, Max: 10, Restriction: Intgers } // GGE1407, typo in "Intgers".
///     }
/// }
/// @endcode

/// @page gge1408 GGE1408
/// The `Width` node of @link metadata script metadata@endlink was of incorrect type.
/// 
/// This node is expected to be either `Identifier` (named `Finite` or `Infinite`) or `KeyValueCollection` as specified in @link map_size Map width and height@endlink.
/// 
/// @code{.cs}
/// metadata
/// {
///     Width: "Finite" // GGE1408, String not expected
/// }
/// @endcode

/// @page gge1409 GGE1409
/// The `Height` node of @link metadata script metadata@endlink was of incorrect type.
/// 
/// This node is expected to be either `Identifier` (named `Finite` or `Infinite`) or `KeyValueCollection` as specified in @link map_size Map width and height@endlink.
/// 
/// @code{.cs}
/// metadata
/// {
///     Height: "Finite" // GGE1409, String not expected
/// }
/// @endcode

/// @page gge1410 GGE1410
/// An identifier used in @link metadata script metadata@endlink was not recognized.
/// 
/// Each context in metadata supports different identifiers - for example the @link map_size `Width` node@endlink supports `Finite` and `Infinite` while @link script_parameters `Restriction` of a script parameter@endlink supports identifiers such as `Unrestricted` and `PowersOf10`. Nodes not recognized by the parser will never trigger this error.
/// 
/// @code{.cs}
/// metadata
/// {
///     Height: HelloWorld // GGE1410, no such identifier is supported
/// }
/// @endcode

/// @page gge1411 GGE1411
/// A map size (maximum, minimum or default width or height) in @link metadata script metadata@endlink was outside of the supported range.
/// 
/// The map size must be greater than or equal to 1 and less than or equal to 4294967293. If you want the map avoid setting huge numbers as maximum map size without making the map truly infinite, you can simply specify the `Width`/`Height` node as `Finite`.
/// 
/// @code{.cs}
/// metadata
/// {
///     Height: { Max: 9999999999999999999999 } // GGE1411
/// }
/// @endcode

/// @page gge1412 GGE1412
/// A minimum map size (width or height) in @link metadata script metadata@endlink was greater than corresponding maximum map size.
/// 
/// @code{.cs}
/// metadata
/// {
///     Height: { Min: 2000, Max: 1000 } // GGE1412
/// }
/// @endcode

/// @page gge2101 GGE2101
/// An argument passed to a native function was of incorrect type (and was not @link implicit_conversions convertible@endlink to the correct type).
/// 
/// See appropriate entry in the @link standard_library standard library@endlink documentation to see which types of arguments are expected by the function.
/// 
/// @code{.cs}
/// Sin("Hello"); // GGE2101, Number expected
/// @endcode

/// @page gge2102 GGE2102
/// An operand of an operator was of incorrect type (and was not @link implicit_conversions convertible@endlink to the correct type).
/// 
/// See @link operators operators@endlink documentation to see which types of operands are expected by the operator.
/// 
/// @code{.cs}
/// var a = 6; 
/// a += "Hello"; // GGE2102, Number expected
/// @endcode

/// @page gge2103 GGE2103
/// Incorrect number of arguments was passed to a function.
/// 
/// Script defined functions always require specific number of arguments. Native functions may have optional parameters (eg. HeightMap.Flat) or even arbitrary number of arguments (eg. Print).
/// 
/// @code{.cs}
/// function MyFunction(x, y)
/// {
///     return x + y;
/// }
/// 
/// MyFunction(1, 2, 3); // GGE2103, 2 arguments expected
/// 
/// var map1 = HeightMap.Flat(); // OK, the function has one optional argument
/// var map2 = HeightMap.Flat(0.5); // OK
/// var map3 = HeightMap.Flat(0.2, 2); // GGE2103
/// @endcode

/// @page gge2104 GGE2104
/// Condition expression in an @link if `if`@endlink, @link while `while`@endlink or @link for `for`@endlink statement returned a value of incorrect type (Boolean is expected).
/// 
/// @code{.cs}
/// var array = {1, 2, 3}; 
/// if(array.Count()) // GGE2104, got Number (use "array.Count() > 0")
/// {
///     Print("Empty!");
/// }
/// @endcode

/// @page gge2106 GGE2106
/// The script attempted to access a member of @link null `null`@endlink. 
/// 
/// @code{.cs}
/// var a; // All variables are initialized to null
/// a.Add(5); // GGE2106
/// @endcode

/// @page gge2107 GGE2107
/// The script attempted to @link yield yield@endlink an object of incorrect type (HeightMap is expected). 
/// 
/// @code{.cs}
/// yield 5; // GGE2107
/// @endcode

/// @page gge2108 GGE2108
/// The script attempted to write into a read-only variable. 
/// 
/// @code{.cs}
/// var point = [100, 200];
/// point.X = 300; // GGE2108, properties of a Point are read only.
/// @endcode

/// @page gge2201 GGE2201
/// The script attempted to call an undefined function.
/// 
/// See the @link standard_library standard library@endlink documentation to see a list of all predefined functions. 
/// 
/// @code{.cs}
/// HelloWorld(); // GGE2201
/// @endcode

/// @page gge2202 GGE2202
/// The script attempted to read or write an undefined variable.
/// 
/// See the @link standard_library standard library@endlink documentation to see a list of all predefined variables/properties. 
/// 
/// A @link variables script defined variable@endlink must be declared in code executed before the variable is referenced (this applies to both globals and locals).
/// 
/// @code{.cs}
/// var a;
/// a = 1; // OK
/// b = 2; // GGE2202, b is never declared
/// c = 3; // GGE2202, c is declared after it is first referenced
/// var c;
/// 
/// function WriteD(x)
/// {
///     var l;
///     l = 4; // This always works, l is local variable in WriteD correctly declared before it is accessed
///     d = x; // d is not a local variable in WriteD, so global variable d has to be declared. Whether this fails will depend on whether global d was declared before this function was called, see below.
/// }
/// 
/// WriteD(5); // GGE2202 inside WriteD (global variable d was not declared yet)
/// 
/// global d;
/// WriteD(6); // OK, global variable d was declared correctly
/// 
/// // d is now 6
/// @endcode

/// @page gge2204 GGE2204
/// The script attempted to define a variable which was already defined in the same context.
/// 
/// See the @link variables variable hiding rules@endlink for details on how variable declarations interact.
/// 
/// @code{.cs}
/// var a = 1;
/// var a = 2; // GGE2204, a is already declared
/// 
/// global b = 3;
/// var b = 4; // OK, local b hides global b
/// 
/// var c = 5;
/// global c = 6; // OK, but local c still hides global c
/// 
/// global d = 7;
/// global d = 8; // GGE2204, global d already defined
/// 
/// var e = 9;
/// {
///     var e = 10; // OK, inner local e hides outer local e
/// }
/// 
/// var f = 11;
/// 
/// function MyFunction()
/// {
///     var f = 12; // OK, local variables across functions don't interact
/// }
/// @endcode

/// @page gge2301 GGE2301
/// The script attempted to divide a number by zero.
/// 
/// @code{.cs}
/// var a = 5 / 0; // GGE2301
/// @endcode

/// @page gge2302 GGE2302
/// A number or a result of a numeric operation was outside of the range allowed by Number.
/// 
/// Number is a doube precision floating point number as specified by the [IEEE 754](http://en.wikipedia.org/wiki/IEEE_floating_point) standard. Therefore the range of representable numbers is huge (-1.7*10<sup>308</sup> to 1.7*10<sup>308</sup>), but still finite (special number such as infinity and NaN are not allowed by GeoGen).
/// 
/// @code{.cs}
/// var a = 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 *  
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 *
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 * // GGE2302
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 *
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 *
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999; 
/// @endcode

/// @page gge2303 GGE2303
/// The script attempted to specify a height outside of the range of allowed heigts (-1 to 1).
/// 
/// This error occurs when such number is passed to a @link standard_library standard library@endlink function which expectes it to be a height.
/// 
/// @code{.cs}
/// yield HeightMap.Flat(3); GGE2303, Flat expects its argument to be a height, 3 is greater than 1
/// @endcode

/// @page gge2304 GGE2304
/// The script attempted to specify a coordinate outside of the range of allowed values (-2147483648 to 2147483647).
/// 
/// This error occurs when such number is passed to a @link standard_library standard library@endlink function which expectes it to be a coordinate or when a relative coordinate is evaluated to an absolute coordinate.
/// 
/// @code{.cs}
/// var map = HeightMap.Flat();
/// map.Move(@[10000000, 10000000]); // GGE2304, during relative coordinate evaluation
/// @endcode

/// @page gge2305 GGE2305
/// Argument of a trigonometric function was outside of its definition range.
/// 
/// This applies to Asin and Acos.
/// 
/// @code{.cs}
/// var a = Asin(6); // GGE2305
/// @endcode

/// @page gge2306 GGE2306
/// Argument of a Log function was outside of its definition range.
/// 
/// @code{.cs}
/// var a = log(-1); // GGE2306
/// @endcode

/// @page gge2307 GGE2307
/// Argument of a Sqrt function was outside of its definition range.
/// 
/// @code{.cs}
/// var a = log(-1); // GGE2307
/// @endcode

/// @page gge2308 GGE2308
/// A number or a result of a numeric operation was too small to represent with Number.
/// 
/// Number is a doube precision floating point number as specified by the [IEEE 754](http://en.wikipedia.org/wiki/IEEE_floating_point) standard. Therefore the smallest representable numbers are -1.7*10<sup>-308</sup> and 1.7*10<sup>-308</sup> (special number such as NaN are not allowed by GeoGen).
/// 
/// @code{.cs}
/// var a = 1 /  
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 /
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 / 
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 / // GGE2308
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999 /
/// 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999; 
/// @endcode

/// @page gge2309 GGE2309
/// The script attempted to specify a size (or length) outside of the range of allowed values (0 to 4294967295).
/// 
/// This error occurs when such number is passed to a @link standard_library standard library@endlink function which expectes it to be a size/length.
/// 
/// @code{.cs}
/// var map = HeightMap.RadialGradient([0, 0], 9999999999999999999999999, 1, 0); // GGE2309 in the radius argument 
/// @endcode

/// @page gge2501 GGE2501
/// Call stack became too large while executing the script.
/// 
/// This error is usually caused by runaway recursion. Maximum call stack size is 10000.
/// 
/// @code{.cs}
/// MyFunction(x)
/// {
///     return MyFunction(x); // GGE2501, after thousands of nested calls
/// }
/// @endcode

/// @page gge2502 GGE2502
/// Code block stack became too large while executing the script.
/// 
/// This error is caused by excessive nesting of code blocks. Maximum code block stack size is 10000.

/// @page gge2503 GGE2503
/// Object stack became too large while executing the script.
/// 
/// This error is caused by excessively complex expressions. Maximum object stack size is 10000.

/// @page gge2601 GGE2601
/// An operation required non-empty was called on an empty array.
/// 
/// This error is triggered by functions like Array.Pop and Array.Back.
/// 
/// @code{.cs}
/// var array = { 1, 2 };
/// array.Pop(); // OK, one item remains
/// array.Pop(); // OK, the array is now empty
/// array.Pop(); // GGE2601
/// @endcode

/// @page gge2602 GGE2602
/// Specified key was not found in an array.
/// 
/// This error is by the [] operator in read mode and functions like Array.RemoveKey.
/// 
/// @code{.cs}
/// var array = {"a": 1, "b": 2 };
/// var c = array["c"]; // GGE2602
/// array.RemoveKey("a"); // OK
/// array.RemoveKey("a"); // GGE2602, this key is already deleted
/// @endcode

/// @page gge2603 GGE2603
/// The script attempted to insert a key-value pair with `null` as its key into an array.
/// 
/// @code{.cs}
/// var array = {"a": 1, "b": 2 };
/// var c; // Variables are initialized as null
/// array[c] = 3;
/// @endcode

/// @page gge2604 GGE2604
/// Specified index was not found in the array.
/// 
/// Index into an array is a numeric identifier of position of a key-value pair in the array from first pair (index 0) to the last (index Array.Count() - 1).
/// 
/// @code{.cs}
/// var array = {"a": 1, "b": 2 };
/// array.GetKeyByIndex(1); // OK, returns 2
/// array.GetKeyByIndex(2); // GGE2604
/// @endcode

/// @page gge2701 GGE2701
/// A string format template had incorrect formating marks in it.
/// 
/// This may include completely broken marks (such as unclosed opening mark bracket or non-numeric replacement index) or wrong replacement index.
/// 
/// @code{.cs}
/// Print("Hello { Haha"); // GGE2701, unclosed formatting mark bracket
/// Print("Hello {aa} "); // GGE2701, only numbers can be contained in formatting marks
/// Print("Number is {2}", 6.14, 2.56); // GGE2701, too few arguments
/// Print("Number is {0}", 6.14); // OK
/// @endcode

/// @page gge2702 GGE2702
/// The script attempted to translate a incorrect numeric value to an @link enum_types enum value@endlink using FromNumber function of that enum type.
/// 
/// This means that the numeric value being translated doesn't correspond to any item declared by that enum.
/// 
/// @code{.cs}
/// enum DaysOfWeek
/// {
///     Monday = 1,
///     Tuesday = 2,
///     Wednesday = 3,
///     Thursday = 4,
///     Friday = 5,
///     Saturday = 6,
///     Sunday = 7
/// }
/// 
/// DaysOfWeek.FromNumber(5); // OK, returns DaysOfWeek.Friday
/// DaysOfWeek.FromNumber(8); // GGE2702, 8 doesn't correspond to any item declared by DaysOfWeek
/// @endcode

/// @page gge2703 GGE2703
/// The script attempted to use @link Coordinate relative coordinate@endlink in context to which dimension of the map the coordinate is relative to.
/// 
/// This means that relative coordinates can't be used with most functions of HeightProfile and some functions for HeightMap, like radius of HeightMap.RadialGradient. Relative coordinates can be converted to absolute coordinates using Number.FromCoordinate (which has an optional argument that specified direction of the coordinate). Point doesn't suffer from this issue, because each of the component coordinates has its direction well defined. HeightProfile.FromArray has a special optional argument only used to determine direction of any relative coordinates in the initialization array.
/// 
/// @code{.cs}
/// var map = HeightMap.Noise();
/// map.Blur(@0.1); // GGE2703, Blur applies in all directions at once.
/// 
/// var profile = HeightProfile.Slice(map, @0.3, Direction.Vertical); // OK, the direction is specified in the call
/// @endcode

/// @page gge2704 GGE2704
/// The script attempted to use @link Coordinate relative coordinate@endlink in dimension in which the map is infinite.
/// 
/// Relative coordinates can't be used in dimensions in which the map is infinite because there is no reference size to evaluate the coordinate against.
/// 
/// @code{.cs}
/// metadata
/// {
///     Height: Infinite,
///     Width: Infinite
/// }
/// 
/// var map = HeightMap.RadialGradient(@[0.5, 0.5], 500, 1, 0); // GGE2704
/// @endcode

/// @page gge2705 GGE2705
/// A maximum passed to a standard library function was less than a minimum.
/// 
/// This error applies to functions which take a minimum-maximum pair as their arguments. These include functions like Random, HeightMap.ClampHeights etc.
/// 
/// @code{.cs}
/// var map = HeightMap.RadialGradient(500, 500, 500, 1, 0);
/// map.ClampHeights(0.5, -0.5); // GGE2705, minimum is supposed to be first and maximum second
/// @endcode

/// @page gge2706 GGE2706
/// A key-value pair with a key of incorrect type was passed to HeightProfile.FromArray (Number is expected).
/// 
/// @code{.cs}
/// var profile = HeightProfile.FromArray({
///     150: 0, // OK
///     200: 1, // OK
///     "250": 0.5, // GGE2706
///     300: 0.25 // OK
/// });
/// @endcode

/// @page gge2707 GGE2707
/// A key-value pair with a value of incorrect type was passed to HeightProfile.FromArray (Number is expected).
/// 
/// @code{.cs}
/// var profile = HeightProfile.FromArray({
///     150: 0, // OK
///     200: 1, // OK
///     250: 0.5, // OK
///     300: true // GGE2707
/// });
/// @endcode

/// @page gge2708 GGE2708
/// A wave length-amplitude pair with a key (wave length) of incorrect type was passed to HeightProfile. NHeightMap.Noise (Number is expected).
/// 
/// @code{.cs}
/// var map = HeightMap.Noise({
///     128: 0.5 // OK
///     "256": 0.5 // GGE2708
/// });
/// @endcode

/// @page gge2709 GGE2709
/// A wave length-amplitude pair with a value (amplitude) of incorrect type was passed to HeightProfile.Noise HeightMap.Noise (Number is expected).
/// 
/// @code{.cs}
/// var map = HeightMap.Noise({
///     128: 0.5 // OK
///     256: "0.5" // GGE2709
/// });
/// @endcode

/// @page gge2710 GGE2710
/// A wave length-amplitude pair with a non-positive wave length (key) was passed to HeightProfile.Noise or HeightMap.Noise.
/// 
/// @code{.cs}
/// var map = HeightMap.Noise({
///     128: 0.5 // OK
///     -256: 0.5 // GGE2710
///     0: 0.52 // GGE2710
/// });
/// @endcode

/// @page gge2711 GGE2711
/// A wave length-amplitude pair with a non-positive amplitude (value) was passed to HeightProfile.Noise or HeightMap.Noise.
/// 
/// @code{.cs}
/// var map = HeightMap.Noise({
///     128: 0.5 // OK
///     256: -0.5 // GGE2711
///     512: 0 // GGE2711
/// });
/// @endcode

/// @page gge2712 GGE2712
/// An invalid transformation matrix was passed to HeightMap.Transform.
/// 
/// The transformation matrix must be [invertible](http://en.wikipedia.org/wiki/Invertible_matrix). An invertible matrix always has non-zero [determinant](http://en.wikipedia.org/wiki/Determinant), which can be calculated using following formula:
/// 
/// @verbatim determinant = a11 * a22 - a12 * a21@endverbatim
/// 
/// Non-invertible (singular) matrices usually have their rows/columns related in an unnatural way (for example two rows/columns are the same or a multiple of each other) or have a row/column composed exclusively from zeroes.
/// 
/// @code{.cs}
/// var map = HeightMap.Noise();
/// map.Transform(2, 1, 2, 1); // GGE2712
/// @endcode

/// @page gge2713 GGE2713
/// A persistence number passed to CreateNoiseLayers was ouside of the supported range (0.1 to 0.9).
/// 
/// @code{.cs}
/// var map = HeightMap.Noise(CreateNoiseLayers(1, 256, 0.95)); // GGE2713
/// @endcode

/// @page gge2714 GGE2714
/// A scale number passed to function like HeightMap.Rescale was ouside of the supported range (0.1 to 10).
/// 
/// @code{.cs}
/// var map = HeightMap.Noise();
/// map.Rescale(0.05); // GGE2714
/// @endcode

/// @page gge2801 GGE2801
/// The script finished without @link yield yielding@endlink the main height map.
/// 
/// If the script doesn't contain the main map yield at all, error @ref gge1312 is generated during compilation. This error is usually caused by the main yield being hidden in a conditional block such as @link if `if`@endlink.
/// 
/// @code{.cs}
/// if(Random(0, 2) > 1)
/// {
///     yield HeightMap.Noise();
/// }
/// 
/// // GGE2801 if a number less than 1 is generated by the Random call
/// @endcode

/// @page gge3000 GGE3000
/// Indicates incorrect usage of GeoGen API by the hosting application. Please report this as an error to the author of the hosting application.

/// @page gge4000 GGE4000
/// Indicates unrecoverable internal error in GeoGen. Please report this as an error. Provide dump file if available.

/// @page gge4001 GGE4001
/// Indicates unrecoverable internal error in GeoGen, specifically an error in the intermediate code generated by the compiler. Please report this as an error. Provide dump file if available.
/// 
/// @page gge5000 GGE5000
/// The script triggered an error explicitly using TriggerError.



























