/// @page error_codes Error codes
/// 
/// @li @ref gge1101
/// @li @ref gge1102
/// @li @ref gge1201

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

