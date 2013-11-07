/*

TODO:
- Escape sequences for string literals

Operator priority (based on http://en.cppreference.com/w/cpp/language/operator_precedence):

Prio. | Assoc. | Operators
------------------------------------------------------------
1     | LTR    | . [] ()
2     | RTL    | ++ -- !
3     | LTR    | * / %
4     |        | + -
5     |        | << >> 
6     |        | < <= > >=
7     |        | == !=
8     |        | &
9     |        | ^
10    |        | |
11    |        | &&
12    |        | ||
13    |        | ?:
14    | RTL    | = += -= *= /= %= <<= >>= &= ^= |= is
15    | LTR    | ,

*/


grammar GeoGenScript;

script: declaration* metadata? (statement | declaration)*;
        
metadata: ('metadata') keyValueCollection;

keyValueCollection: '{' (keyValuePair (',' keyValuePair )*)? '}';

keyValuePair: (IDENTIFIER | ('@'? NUMBER) ) ':' keyValueValue;

keyValueValue: expression | keyValueCollection;

declaration: enumDeclaration | functionDeclaration;

enumDeclaration: 'enum' IDENTIFIER '{' enumValues? '}';

enumValues: enumValue (',' enumValue)*;

enumValue: 
    IDENTIFIER ( '=' expression)?;

functionDeclaration: 'function' IDENTIFIER '(' formalParameters? ')' block;

formalParameters: IDENTIFIER (',' IDENTIFIER)*;

block: '{' statement* '}';

statement:     
    'break' ';' |
    'continue' ';' |
    variableDeclaration ';' |
    expression ';' |
    yieldStatement ';' |
    returnStatement ';' |
    whileStatement |
    forStatement |
    ifStatement |
    switchStatement |    
    block |
    ';';
    
variableDeclaration: 'var' IDENTIFIER ('=' expression)?;

yieldStatement: 
    'yield' expression |
    'yield' expression 'as' STRING;

returnStatement: 'return' expression;

whileStatement: 'while' '(' expression ')' statement; 

forStatement: 'for' '(' initExpression? ';' expression? ';' expression? ')' statement;

initExpression: 
    'var' IDENTIFIER '=' expression |
    expression;

ifStatement:
    'if' '(' expression ')' statement
    ('else' statement)?;


switchStatement:
    'switch' '(' expression ')' '{'
        ('case' label ':' statement*)*
        ('default' ':'  statement*)? 
    '}';
    

expression:
    prio14Expression ;

prio14Operator: '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '/=' | 'is';
prio14Expression: prio13Expression (prio14Operator prio13Expression)*;	

prio13Expression: prio12Expression ('?' prio12Expression ':' prio12Expression)*;

prio12Operator: '||';
prio12Expression: prio11Expression (prio12Operator prio11Expression)*;

prio11Operator: '&&';
prio11Expression: prio10Expression (prio11Operator prio10Expression)*;

prio10Operator: '|';
prio10Expression: prio9Expression (prio10Operator prio9Expression)*;

prio9Operator: '^';
prio9Expression: prio8Expression (prio9Operator prio8Expression)*;

prio8Operator: '&';
prio8Expression: prio7Expression (prio8Operator prio7Expression)*;

prio7Operator: '==' | '!=';
prio7Expression: prio6Expression (prio7Operator prio6Expression)*;

prio6Operator: '<' | '<=' | '>' | '>=';
prio6Expression: prio5Expression (prio6Operator prio5Expression)*;

prio5Operator: '<<' | '>>';
prio5Expression: prio4Expression (prio5Operator prio4Expression)*;

prio4Operator: '+' | '-';
prio4Expression: prio3Expression (prio4Operator prio3Expression)*;

prio3Operator: '*' | '/' | '%';
prio3Expression: prio2Expression (prio3Operator prio2Expression)*;

prio2PrefixOperator: '++' | '--' | '!' | '+' | '-';
prio2PostfixOperator: '++' | '--';
prio2Expression: prio2PrefixOperator* prio1Expression prio2PostfixOperator* ;  

prio1Expression:
    prio0Expression (
        '.' prio0Expression |
        '(' (expression (',' expression)*)? ')' |
        '[' expression (',' expression)* ']'
    )*;

prio0Expression: 
    IDENTIFIER |
    collectionLiteral |
    coordinateLiteral |
    literal |
    '(' expression ')';


collectionLiteral: 
    keyValueCollection |
    unkeyedCollectionLiteral;

unkeyedCollectionLiteral:
    '{' (expression + (',' expression)*) '}';

coordinateLiteral:
    '@'? '[' expression (',' expression)* ']';

label:        
	IDENTIFIER ('.' IDENTIFIER)* |
	literal;

literal:
        'true' |
        'false' |
	NUMBER |
	STRING;

LEFT_BRACKET: '(';
RIGHT_BRACKET: ')';
LEFT_SQUARE_BRACKET: '[';
RIGHT_SQUARE_BRACKET: ']';
LEFT_CURLY_BRACKET: '{';
RIGTH_CURLY_BRACKET: '}';

TRUE: 'true';
FALSE: 'false';

COMMA: ',';
SEMICOLON: ';';
COLON: ':';
AT: '@';

METADATA: 'metadata';
ENUM: 'enum';
FUNCTION: 'function';
VAR: 'var';

RETURN: 'return';
YIELD: 'yield';
AS: 'as';

SWITCH: 'switch';
DEFAULT: 'default';
CASE: 'case';
IF: 'if';
ELSE: 'else';
FOR: 'for';
WHILE: 'while';
BREAK: 'break';
CONTINUE: 'continue';

OPERATOR_DOT: '.';
OPERATOR_INCREMENT: '++';
OPERATOR_DECREMENT: '--';
OPERATOR_NOT: '!';
OPERATOR_TIMES: '*';
OPERATOR_DIVIDE: '/';
OPERATOR_MODULO: '%';
OPERATOR_PLUS: '+';
OPERATOR_MINUS: '-';
OPERATOR_LSHIFT: '<<';
OPERATOR_RSHIFT: '>>';
OPERATOR_LESS_THAN: '<';
OPERATOR_GREATER_THAN: '>';
OPERATOR_LESS_THAN_OR_EQUAL_TO: '<=';
OPERATOR_GREATER_THAN_OR_EQUAL_TO: '>=';
OPERATOR_EQUAL_TO: '==';
OPERATOR_NOT_EQUAL_TO: '!=';
OPERATOR_BIT_AND: '&';
OPERATOR_BIT_XOR: '^';
OPERATOR_BIT_OR: '|';
OPERATOR_REL_AND: '&&';
OPERATOR_REL_OR: '||';
OPERATOR_EXPR_IF: '?';
OPERATOR_ASSIGN: '=';
OPERATOR_ASSIGN_PLUS: '+=';
OPERATOR_ASSIGN_MINUS: '-=';
OPERATOR_ASSIGN_TIMES: '*=';
OPERATOR_ASSIGN_DIVIDE: '/=';
OPERATOR_ASSIGN_MODULO: '%=';
OPERATOR_ASSIGN_LSHIFT: '<<=';
OPERATOR_ASSIGN_RSHIFT: '>>=';
OPERATOR_ASSIGN_AND: '&=';
OPERATOR_ASSIGN_XOR: '^=';
OPERATOR_ASSIGN_OR: '|=';
OPERATOR_IS: 'is';

MULTILINE_COMMENT: '/*' .*? '*/' {skip();};

LINE_COMMENT
  :  '//' ~( '\r' | '\n' )* {skip();}
  ;

IDENTIFIER: ('a'..'z' |'A'..'Z' |'_' ) ('a'..'z' |'A'..'Z' |'_' |'0'..'9' )*;

fragment INTEGER: ('0'..'9')+ ;

NUMBER: INTEGER ('.' INTEGER)?;

STRING: '"' .*? '"';

WHITESPACE: (' ' |'\t' |'\n' |'\r' )+ {skip();} ;