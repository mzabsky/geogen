/*

OTAZKY:
- Case sensitivita klicovych slov?
- Collection literaly
- Jak prisna ma byt gramatika?
- Mam spravne asociativitu?

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
14    | RTL    | = += -= *= /= %= <<= >>= &= ^= |=
15    | LTR    | ,

*/


grammar GeoGenScript;

script: declaration* metadata? (statement | declaration)*;
        
metadata: ('metadata' | 'Metadata') keyValueCollection;

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
    /*globalFunctionCall ';' |
    memberFunctionCall ';' |*/
    //assignmentStatement ';' |
    //incrDecrExpression |
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

/*globalFunctionCall: IDENTIFIER '(' callArguments? ')';

memberFunctionCall: IDENTIFIER '.' IDENTIFIER '(' callArguments? ')';

callArguments: expression (',' expression)*;*/

/*
assignmentStatement: IDENTIFIER ASSIGNMENT_OPERATOR expression;

incrDecrExpression: IDENTIFIER INCRDECR_OPERATOR;
*/

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

prio14Operator: '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '/=';
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

MULTILINE_COMMENT: '/*' .*? '*/' {skip();};

LINE_COMMENT
  :  '//' ~( '\r' | '\n' )* {skip();}
  ;

IDENTIFIER: ('a'..'z' |'A'..'Z' |'_' ) ('a'..'z' |'A'..'Z' |'_' |'0'..'9' )*;

fragment INTEGER: ('0'..'9')+ ;

NUMBER: INTEGER ('.' INTEGER)?;

STRING: '"' .*? '"';

WHITESPACE: (' ' |'\t' |'\n' |'\r' )+ {skip();} ;