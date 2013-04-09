/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


/*

OTAZKY:
- Case sensitivita klicovych slov?


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


PRIO1_OPERATOR: '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '/=';
prio1Expression: prio2Expression (PRIO1_OPERATOR prio2Expression)*;	

prio2Expression:
    prio3Expression '?' prio3Expression ':' prio3Expression | 
    prio3Expression;

PRIO3_OPERATOR: '||';
prio3Expression: prio4Expression (PRIO3_OPERATOR prio4Expression)*;

PRIO4_OPERATOR: '&&';
prio4Expression: prio5Expression (PRIO4_OPERATOR prio5Expression)*;

PRIO5_OPERATOR: '|';
prio5Expression: prio6Expression (PRIO5_OPERATOR prio6Expression)*;

PRIO6_OPERATOR: '^';
prio6Expression: prio7Expression (PRIO6_OPERATOR prio7Expression)*;

PRIO7_OPERATOR: '&';
prio7Expression: prio8Expression (PRIO7_OPERATOR prio8Expression)*;

PRIO8_OPERATOR: '==' | '!=';
prio8Expression: prio9Expression (PRIO8_OPERATOR prio9Expression)*;

PRIO9_OPERATOR: '<' | '<=' | '>' | '>=';
prio9Expression: prio10Expression (PRIO9_OPERATOR prio10Expression)*;

PRIO10_OPERATOR: '<<' | '>>';
prio10Expression: prio11Expression (PRIO10_OPERATOR prio11Expression)*;

PRIO11_OPERATOR: '+' | '-';
prio11Expression: prio12Expression (PRIO11_OPERATOR prio12Expression)*;

PRIO12_OPERATOR: '*' | '/' | '%';
prio12Expression: prio13Expression (PRIO12_OPERATOR prio13Expression)*;

PRIO13_OPERATOR: '++' | '--' | '!';
prio13Expression: PRIO13_OPERATOR* prio14Expression PRIO13_OPERATOR* ;  

prio14Expression:
    prio15Expression (
        '.' prio15Expression |
        '(' (expression (',' expression)*)? ')' |
        '[' (expression (',' expression)*)? ']'
    )*;

prio15Expression: 
    IDENTIFIER |
    constant |
    '(' expression ')';

label:
	IDENTIFIER |
	constant;

constant:
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