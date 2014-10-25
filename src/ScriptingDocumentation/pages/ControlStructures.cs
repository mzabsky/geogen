/// @page control_structures Control structures
/// The most common control structures from C-like are supported: if-else, switch, while and for. Less common statements like do-while are not implemented though. Low level concepts such as goto statement are not supported either.
/// 
/// @section if If-else
/// The `if` statement is entered when the given condition is true. It has following syntax:
/// @code{.cs}
/// if (condition) statement; 
/// @endcode
///           
/// Optionally, an else statement can be added, which is entered when the condition is not true.
/// @code{.cs}
/// if (condition) statement;
/// else statement;
/// @endcode
/// 
/// Multiple if-else blocks can be combined into an if-else if-...-else chain.
/// @code{.cs}
/// if (condition) statement;
/// else if(condition) statement;
/// else if(condition) statement;
/// ...
/// else statement;
/// @endcode
/// 
/// The condition expression has to return Boolean, otherwise a runtime error will be generated.
/// 
/// Example:
/// @code{.cs}
/// if(a >= 8)
/// {
///     Print("A is large number.");
/// }
/// else if(a >= 5)
/// {
///     Print("A is moderate number.");
/// }
/// else
/// {
///     Print("A is small number.");
/// }
/// @endcode
/// 
/// @section while While
/// The `while` statement is executed repeatedly as long as the loop condition is true. If the condition is false before the statement is first executed, the statement won't be executed at all.
/// 
/// The `while` statement has following syntax:
/// @code{.cs} 
/// while (condition) statement;
/// @endcode
///           
/// The condition expression has to return Boolean, otherwise a runtime error will be generated.
/// 
/// Example:
/// @code{.cs}
/// // Repeat until a number greater than 0.5 is generated.
/// var r = Random();
/// while(r > 0.5)
/// {
///     Print("Still lucky!");
///     r = Random();
/// }
/// @endcode
/// 
/// @section for For
/// The `for` statement first executes the initializer statement and then executes the loop statement repeatedly as long as the condition expression is true, executing the increment expression at the end of each cycle.
/// 
/// The `for` statement has following syntax:
/// @code{.cs}
/// for (initializer; condition expression; increment expression) loop statement;
/// @endcode
/// 
/// For statements are typically used to execute a code for known number of repetitions or to loop over collections of known size:
/// 
/// @code{.cs}
/// for (var i = 0; i < array.Count(); i++)
/// {
///	    array[i] = 0;
/// }
/// @endcode
/// 
/// Initializer statement may be either a variable declaration (either local or global) or any other expression (such as assignment expression referencing already declared variable). The condition expression must be an expression returning Boolean value (otherwise a runtime error will be thrown). The increment expression may be any expression.
/// 
/// Initializer, condition expression and increment expression are all optional and may be omitted.
/// 
/// The entire for loop is always enclosed in a local scope of its own, so following code is perfectly legal, according to @link variables variable hiding rules@endlink:
/// 
/// @code{.cs}
/// var i = 9;
/// for (var i = 0; i < 10; i++)
/// {
///	    // This code will execute 10 times, because the "i" declared by the for loop will hide the variable declared outside of the loop.
/// }
/// 
/// // i is still 9 here
/// @endcode
/// 
/// For loop as defined above is therefore functionally equivalent to this code using a while loop:
/// @code{.cs}
/// {
///	    Initializer statement;
///	    while(condition expression)
///     {
///		    loop statement;
///		    increment expression;
///	    }
/// }
/// @endcode
/// 
/// @section break Break
/// The `break` statement may be used to terminate a loop prematurely. 
/// 
/// Break can be used in both while and for statements. When used outside of a loop statement a runtime error will be thrown. If there are multiple nested loop statements, break will only terminate the inmost one.
/// 
/// Example:
/// 
/// @code{.cs}
/// for (int I = 0; i < 10; i++)
/// {
///	    if (i == 5)
///	    {
///		    break;
///	    }
///     
///	    // Code here will execute 5 times.
/// }
/// @endcode
/// 
/// @section continue Continue
/// The `continue` statement may be used to skip remainder a loop body and immediately continue with its next iteration (or terminate the loop if the condition expression evaluates to false).
/// 
/// Continue can be used in both while and for statements. If continue is used in a for loop, the increment expression will be evaluated before checking the loop condition. When used outside of a loop statement a runtime error will be thrown. If there are multiple nested loop statements, continue will only affect the inmost one.
/// 
/// Example:
/// 
/// @code{.cs}
/// for (int I = 0; i < 10; i++)
/// {
///	    if (i == 5)
///	    {
///		    continue;
///	    }
///	    // Code here will execute 9 times.
/// }
/// 
/// @endcode
