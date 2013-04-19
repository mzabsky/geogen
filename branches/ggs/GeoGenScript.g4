/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


/*

OTAZKY:
- Case sensitivita klicovych slov?
- Collection literaly

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

keyValuePair: IDENTIFIER ':' keyValueValue;

keyValueValue: expression | keyValueCollection;

declaration: enumDeclaration | functionDeclaration;

enumDeclaration: 'enum' IDENTIFIER '{' enumValues? '}';

enumValues: enumValue (',' enumValue)*;

enumValue: 
    IDENTIFIER ( '=' expression)?;

functionDeclaration: 'function' IDENTIFIER '(' formalParameters? ')' block;

formalParameters: IDENTIFIER (',' IDENTIFIER);

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

globalFunctionCall: IDENTIFIER '(' callArguments ')';

memberFunctionCall: IDENTIFIER '.' IDENTIFIER '(' callArguments ')';

callArguments: expression (',' expression);

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
    prio1Expression ;


prio1Operator: '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '/=';
prio1Expression: prio2Expression (prio1Operator prio2Expression)*;	

prio2Expression: prio3Expression ('?' prio3Expression ':' prio3Expression)*;

prio3Operator: '||';
prio3Expression: prio4Expression (prio3Operator prio4Expression)*;

prio4Operator: '&&';
prio4Expression: prio5Expression (prio4Operator prio5Expression)*;

prio5Operator: '|';
prio5Expression: prio6Expression (prio5Operator prio6Expression)*;

prio6Operator: '^';
prio6Expression: prio7Expression (prio6Operator prio7Expression)*;

prio7Operator: '&';
prio7Expression: prio8Expression (prio7Operator prio8Expression)*;

prio8Operator: '==' | '!=';
prio8Expression: prio9Expression (prio8Operator prio9Expression)*;

prio9Operator: '<' | '<=' | '>' | '>=';
prio9Expression: prio10Expression (prio9Operator prio10Expression)*;

prio10Operator: '<<' | '>>';
prio10Expression: prio11Expression (prio10Operator prio11Expression)*;

prio11Operator: '+' | '-';
prio11Expression: prio12Expression (prio11Operator prio12Expression)*;

prio12Operator: '*' | '/' | '%';
prio12Expression: prio13Expression (prio12Operator prio13Expression)*;

prio13Operator: '++' | '--' | '!' | '+' | '-';
prio13Expression: prio13Operator* prio14Expression prio13Operator* ;  

prio14Expression:
    prio15Expression (
        '.' prio15Expression |
        '(' (expression (',' expression)*)? ')' |
        '[' expression (',' expression)* ']'
    )*;

prio15Expression: 
    IDENTIFIER |
    //collectionLiteral |
    coordinateLiteral |
    literal |
    '(' expression ')';


collectionLiteral: 
    '{' (expression + (',' expression)?)? '}';

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