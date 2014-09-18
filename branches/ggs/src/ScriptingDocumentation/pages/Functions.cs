/// @page page_functions Functions
/// User functions are defined with the `function` statement. User function can have any constant number of parameters, but implicit parameters and variable number of parameters are only supported for native functions. User functions are not allowed to specify types of individual parameters. Native functions will do runtime type checks on their arguments.
/// 
/// Execution of a user function can be terminated at any time using the `return` statement. A return statement may specify a value to be returned from the function. All functions return a value - if execution of function code ends without a `return` statement or the return statement doesn't specify a return value, null is returned.
/// 
/// @code{.cs}
/// function max(a, b)
/// {
///	    if(a > b)
///     {
///		    return a;
///	    }
///     
///	    return b;
/// }
/// @endcode
/// 
/// A user function can be defined at any location in the top level block in the script body, before or after @link metadata metadata@endlink. A function may be called in code above its definitions (forward declarations are not required).
/// 
/// Function overloading is not supported - multiple functions must not share names.
