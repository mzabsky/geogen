/// @page operators Operators
/// Following table lists supported operators grouped in descending order by precedence:
/// 
/// Group      | Prec. | Ass. | Op. | Description            | Operands           | Result
/// -----------|-------|------|-----|------------------------|--------------------|--------------------
/// Primary    | 1     | LTR  | .   | Member access          | Object             | Object
/// &nbsp;     |       |      | []  | Index access           | Object, Object...  | Object
/// &nbsp;     |       |      | ()  | Function call          | Object, Object...  | Object
/// Unary      | 2     | RTL  | ++  | Increment (postfix and prefix) | Number     | Number
/// &nbsp;     |       |      | --  | Decrement (postfix and prefix) | Number     | Number
/// &nbsp;     |       |      | !   | Negation               | Number/Boolean     | Number/Boolean
/// &nbsp;     |       |      | +   | Unary plus             | Number             | Number
/// &nbsp;     |       |      | -   | Unary minus            | Number             | Number
/// &nbsp;     |       |      | @   | Relative coordinate/point | Number/Coordinate/Point | Point/Coordinate
/// Multiplicative | 3 | LTR  | *   | Multiplication         | Number, Number     | Number
/// &nbsp;     |       |      | /   | Division               | Number, Number     | Number
/// &nbsp;     |       |      | %   | Modulo (integer remainder) | Number, Number | Number
/// Additive   | 4     |      | +   | Addition               | Number, Number     | Number
/// &nbsp;     |       |      | -   | Subtraction            | Number, Number     | Number
/// Bit shift  | 5     |      | <<  | Bit shift left         | Number, Number     | Number
/// &nbsp;     |       |      | >>  | Bit shift right        | Number, Number     | Number
/// Relational | 6     |      | <   | Less than              | Number, Number     | Boolean
/// &nbsp;     |       |      | <=  | Less than or equal to  | Number, Number     | Boolean
/// &nbsp;     |       |      | >   | Greater than           | Number, Number     | Boolean
/// &nbsp;     |       |      | >=  | Greater than or equal to | Number, Number   | Boolean
/// Equality   | 7     |      | ==  | Equal to               | Object/@ref null, Object/@ref null | Boolean
/// &nbsp;     |       |      | !=  | Not equal to           | Object/@ref null, Object/@ref null | Boolean
/// Bit and    | 8     |      | &   | Bit and                | Number/Boolean, Number/Boolean | Number/Boolean
/// Bit xor    | 9     |      | ^   |Bit xor (exclusive or)  | Number/Boolean, Number/Boolean | Number/Boolean
/// Bit or     | 10    |      | \|  |Bit or (inclusive or)   | Number/Boolean, Number/Boolean | Number/Boolean
/// Logical and | 11   |      | &&  | Logical and            | Boolean, Boolean   | Boolean
/// Logical or | 12    |      | \|\| | Logical or            | Boolean, Boolean   | Boolean
/// Conditional | 13   |      | ?:  | Conditional ternary    | Boolean, Object/@ref null, Object/@ref null | Object/@ref null
/// Assignment | 14    | RTL  | =   | Assignment             | Object, Object/@ref null | Object
/// &nbsp;     |       |      | +=  | Addition assignment    | Number, Number      | Number
/// &nbsp;     |       |      | -=  | Subtraction assignment | Number, Number      | Number
/// &nbsp;     |       |      | *=  | Multiplication assignment | Number, Number   | Number
/// &nbsp;     |       |      | /=  | Division assignment    | Number, Number      | Number
/// &nbsp;     |       |      | %=  | Modulo assignment      | Number, Number      | Number
/// &nbsp;     |       |      | <<= | Bit shift left assignment | Number, Number   | Number
/// &nbsp;     |       |      | >>= | Bit shift right assignment | Number, Number  | Number
/// &nbsp;     |       |      | &=  | Bit and assignment     | Number/Boolean, Number/Boolean | Number/Boolean
/// &nbsp;     |       |      | ^=  | Bit xor assignment     | Number/Boolean, Number/Boolean | Number/Boolean
/// &nbsp;     |       |      | \|= | Bit or assignent       | Number/Boolean, Number/Boolean | Number/Boolean
/// Comma      | 15    | LTR  | ,   | Argument list          |                      | &nbsp;
/// 
/// Operators %, <<, >>, %=, <<= and >>= treat the second argument as an integer (rounding down).
/// 
/// Bit operators which return Number or Boolean will return Boolean as long as both the operands are of type Boolean, otherwise Number will be returned. Both of the operands must be of the same type (eg. `2 & true` is not legal).
/// 
/// Operator @ returns Point as long as its operand was a Point, otherwise it returns a Coordinate.
