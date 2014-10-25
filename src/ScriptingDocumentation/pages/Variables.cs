/// @page variables Variables
/// Local variables are declared explicitly with the `var` statement:
/// 
/// @code{.cs} var x;@endcode
///            
/// Value can be assigned to the local variable during declaration:
/// 
/// @code{.cs} var x = 5; @endcode
/// 
/// Global variables are declared in the same way with `global` statement:
/// 
/// @code{.cs}
/// global a;
/// global b = 5;
/// @endcode
/// 
/// If a variable is not assigned a value during its declaration, it will be declared with @ref null value.
/// 
/// All script defined variables are both readable and writable. Operator = can be used to assign a value to an variable.
/// 
/// @code{.cs} x = 5; @endcode
///            
/// Local variables can be accessed only in their scope and its nested scopes. Global variables can be accessed from anywhere (after the variable is declared). A runtime error is generated when code accesses an undeclared variable.
/// 
/// @code {.cs}
/// function f () 
/// {
///	    a = 2;
/// }
/// 
/// a = 0; // runtime error, the "a" was not declared yet
/// f(); // runtime error again
/// global a = 1;
/// f(); // passes, "a" is now declared
/// 
/// var b = 1;
/// while(b > 5) 
/// {
///	    b = b + 1; // passes, "b" is declared in higher scope
///	    var c = 8;
/// }
/// 
/// c = 5; // runtime error, "c" is not declared in this or higher scope
/// @endcode
/// 
/// Some native variables declared by the runtime may be read-only (runtime error will be generated on an attempt to write into read-only variable).
/// 
/// A global variable must not share its name with any other global variable, otherwise a runtime error will be generated upon its declaration.  A local variable must not share its name with any other local variable declared in the same scope. However, a local variable is allowed to share its name with a global variable and any number of local variables declared in higher scopes. Any references to that variable name will then refer to the closest variable declaration.
/// 
/// @code{.cs}
/// global a = 1;
/// 
/// function f()
/// {
///	    var a = 2;
///	    if (a == 2)
///     {
///		    // true, the local "a" hides the global "a"
///		    var a = 3;
///		    if (a == 3) 
///         {
///			    // true, the inner "a" is the closest declaration
///			    a = 4;
///		    }
///		    
///         if (a == 4) 
///         {
///			    // false, the inner and outer "a" are different variables
///		    }
///	    }
/// }
/// @endcode
