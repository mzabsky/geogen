/// @page basic_syntax Basic syntax
/// Syntax of GGS is C-like. It is inspired by popular modern procedural programming languages like C# and JavaScript.
/// 
/// GGS code is composed out of statements. Each statement has to be terminated with a semicolon. Multiple statements can be combined into one statement using curly brackets.
///
/// @section comments Comments
/// Two types of comments are supported - line comments preceded by @p// and block comments surrounded by @p/* and @endcomment:
/// @code{.cs}
/// x = 5; // Line comment is here
/// /*
/// Block comment is here.
/// Multiple lines of comment text are supported. */ 
/// @endcode
///
/// @section keywords Keywords
/// Following keywords are reserved by the language and cannot be used as identifiers in any context:
/// 
/// @verbatim as, break, case, continue, default, else, false, if, is, for, metadata, null, return, true, var, while, yield@endverbatim
/// 
/// All keywords are case-sensitive.
///
/// @section whitespace Whitespace
/// All whitespace outside of string literals is ignored.
///
/// @section identifiers Identifiers
/// Identifiers may contains letters (uppercase and lowercase, all identifiers are case sensitive), numbers and underscores. Number must not be the first character in the identifier.
/// 
/// Only letters of English alphabet are allowed in identifiers.
/// 
/// All identifiers are case sensitive.
///
/// @section literals Literals
/// 
/// Several of the most basic types are constructed using literals:
/// 
/// Type         | Examples
/// -------------|-------------------------------
/// String       | @code{.cs} "Hello, world" @endcode
/// Number       | @code{.cs} 3.14@endcode
/// Boolean      | @code{.cs} true@endcode
/// Coordinate   | @code{.cs} [550]@endcode
/// Point        | @code{.cs}[600, 400]@endcode
/// Array	     | @code{.cs} {0, 1, "key" 5, 35.2: {"subvalue", 6: 56}} @endcode 
/// 
/// Coordinate parameters may contain any expressions (including variables and function calls) which evaluate to numbers. Relative coordinates cannot be used in dimensions in which the map is infinite.
/// 
/// Array keys can be any expressions that don't evaluate to null. Array values can be any expressions.
/// 
/// String literals can contain following escape sequences, which are interpreted as a different character by the compiler:
/// 
/// Sequence | Meaning
/// ---------|-------------------------------------
/// `\\`	 | Backslash
/// `\n`	 | Line break
/// `\"`	 | Double quote
/// `\t`	 | Horizontal tab character
/// 
/// Unescaped backslash followed by any other character will result in an compilation error.
/// 
/// String literals are allowed to contain unescaped line breaks.
