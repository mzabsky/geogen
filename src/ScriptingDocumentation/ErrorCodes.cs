/// @page error_codes Error codes
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


/// @page gge1101 GGE1101
/// The parser has encountered an unexpected character.
/// 
/// This error is usually caused by a character that is not supported by the parser in any context:
/// @code{.cs}
/// var variablě = 5; // GGE1101 caused by the "ě"
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




















