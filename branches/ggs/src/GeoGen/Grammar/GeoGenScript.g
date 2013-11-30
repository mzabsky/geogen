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
options { 
	//backtrack = true; 
	language = C;
	output=AST; 
	ASTLabelType	= pANTLR3_BASE_TREE;
}


tokens { SCRIPT; COLLECTION; DECLARATIONS; BLOCK; PARAMETERS; COORDINATE; IDENTCHAIN; OPERATOR_CALL; OPERATOR_SUBSCRIPT; INITIALIZATION_EXPRESSION; CONDITION_EXPRESSION; INCREMENT_EXPRESSION;}

/*
@lexer::namespace {
    geogen_generated
}

@parser::namespace {
    geogen_generated
}

@lexer::traits {
	   class GeoGenScriptLexer;
	   class GeoGenScriptParser;
	   typedef antlr3::Traits< GeoGenScriptLexer, GeoGenScriptParser > GeoGenScriptTraits;

	typedef GeoGenScriptTraits GeoGenScriptLexerTraits;
	typedef GeoGenScriptTraits GeoGenScriptParserTraits;
}

@parser::traits {
	   class GeoGenScriptLexer;
	   class GeoGenScriptParser;
	   typedef antlr3::Traits< GeoGenScriptLexer, GeoGenScriptParser > GeoGenScriptTraits;
	   
	typedef GeoGenScriptTraits GeoGenScriptLexerTraits;
	typedef GeoGenScriptTraits GeoGenScriptParserTraits;
}

@parser::includes
{
   #include "GeoGenScriptLexer.hpp"
}*/


script: (decls+=declaration)* (metadata (stmts+=statement | decls+=declaration)* | stmts+=statement (stmts+=statement | decls+=declaration)*)? -> ^(SCRIPT metadata ^(DECLARATIONS $decls*) ^(BLOCK $stmts*));
        
metadata: 'metadata' keyValueCollection -> ^('metadata' keyValueCollection);

keyValueCollection: '{' (keyValuePair (',' keyValuePair )*)? '}' -> ^(COLLECTION keyValuePair*);

keyValuePair: 
	IDENTIFIER ':' keyValueValue -> ^(IDENTIFIER keyValueValue)
	| '@'? NUMBER ':' keyValueValue -> ^(NUMBER keyValueValue '@'?);

keyValueValue: expression | keyValueCollection;

declaration: enumDeclaration | functionDeclaration;

enumDeclaration: 'enum' IDENTIFIER '{' enumValues? '}' -> ^('enum' IDENTIFIER enumValues?);

enumValues: enumValue (',' enumValue)* -> enumValue+;

enumValue: 
    IDENTIFIER ( '=' expression)? -> ^(IDENTIFIER expression?);

functionDeclaration: 'function' IDENTIFIER '(' formalParameters? ')' block -> ^('function' IDENTIFIER ^(PARAMETERS formalParameters) block);

formalParameters: IDENTIFIER (',' IDENTIFIER)* -> IDENTIFIER*;

block: '{' statement* '}' -> ^(BLOCK statement*);

statement:     
    'break' ';' -> BREAK
    | 'continue' ';' -> CONTINUE
    | variableDeclaration ';' -> variableDeclaration
    | expression ';' -> expression
    | yieldStatement ';' -> yieldStatement
    | returnStatement ';' -> returnStatement
    | whileStatement -> whileStatement
    | forStatement -> forStatement
    | ifStatement -> ifStatement
    | switchStatement -> switchStatement
    | block -> block
    | ';' -> ;
    
variableDeclaration: 'var' IDENTIFIER ('=' expression)? -> ^('var' IDENTIFIER expression?);

yieldStatement: 
    YIELD expression ('as' STRING)? -> ^(YIELD expression STRING?);


returnStatement: 'return' expression? -> ^(RETURN expression?);

whileStatement: 'while' '(' expression ')' statement -> ^(WHILE expression statement); 

forStatement: 'for' '(' initExpression? ';' condExpression=expression? ';' updateExpression=expression? ')' statement -> ^(FOR ^(INITIALIZATION_EXPRESSION initExpression?) ^(CONDITION_EXPRESSION $condExpression?) ^(INCREMENT_EXPRESSION $updateExpression?) statement);

initExpression: 
    'var' IDENTIFIER '=' expression -> ^('var' IDENTIFIER expression)
    | expression -> expression;

ifStatement:
    ('if' '(' expression ')' statement 'else') =>  'if' '(' expression ')' ifStmt=statement 'else' elseStmt=statement -> ^(IF expression $ifStmt $elseStmt)
    | 'if' '(' expression ')' statement  -> ^(IF expression statement)
    /*('else' statement)?*/;


switchStatement:
    'switch' '(' expression ')' '{'
	normalCase*
        defaultCase?
    '}' -> ^(SWITCH expression normalCase* defaultCase?);

normalCase: 'case' label ':' statement* -> ^(CASE label ^(BLOCK statement*));
defaultCase: 'default' ':' statement* -> ^(DEFAULT ^(BLOCK statement*));

expression:
    prio14Expression ;

//prio14Operator: ;
prio14Expression: prio13Expression (('=' | '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '|=' | '^=' | 'is')^ prio13Expression)*;	

prio13Expression: prio12Expression ('?'^ prio13Expression ':' prio12Expression)*;

prio12Operator: '||';
prio12Expression: prio11Expression (prio12Operator^ prio11Expression)*;

prio11Operator: '&&';
prio11Expression: prio10Expression (prio11Operator^ prio10Expression)*;

prio10Operator: '|';
prio10Expression: prio9Expression (prio10Operator^ prio9Expression)*;

prio9Operator: '^';
prio9Expression: prio8Expression (prio9Operator^ prio8Expression)*;

prio8Operator: '&';
prio8Expression: prio7Expression (prio8Operator^ prio7Expression)*;

//prio7Operator: '==' | '!=';
prio7Expression: prio6Expression (('==' | '!=')^ prio6Expression)*;

//prio6Operator: '<' | '<=' | '>' | '>=';
prio6Expression: prio5Expression (('<' | '<=' | '>' | '>=')^ prio5Expression)*;

//prio5Operator: '<<' | '>>';
prio5Expression: prio4Expression (('<<' | '>>')^ prio4Expression)*;

//prio4Operator: '+' | '-';
prio4Expression: prio3Expression (('+' | '-')^ prio3Expression)*;

//prio3Operator: '*' | '/' | '%';
prio3Expression: prio2Expression (('*' | '/' | '%')^ prio2Expression)*;

//prio2PrefixOperator: '++' | '--' | '!' | '+' | '-';
//prio2PostfixOperator: '++' | '--';
//prio2Expression: prio2PrefixOperator* prio1Expression prio2PostfixOperator*;  
prio2Expression
	:	 prio1Expression;
//prio2Expression: prio1Expression (('++' | '--' | '!') prio1Expression)*;

//prio2Expression:	('++' | '--' | '!' | '+' | '-')? prio1Expression ('++' | '--' | '!' | '+' | '-')?;

//prio2ExpressionPost: prio1Expression ('++' | '--')+ | prio1Expression ;  


prio1Expression:
    prio0Expression (
        '.'^ IDENTIFIER
        | '('^ (expression (','! expression)*)? ')'!
        | '['^ expression (','! expression)* ']'!
    )*;

prio0Expression: 
     /*('(') => */('(' expression ')') -> expression
    | IDENTIFIER
    //collectionLiteral |
    | coordinateLiteral
    | 'true'
    | 'false'
    | NUMBER
    | STRING 
    ; // expression!

/*collectionLiteral: 
    keyValueCollection |
    unkeyedCollectionLiteral;*/

unkeyedCollectionLiteral:
    '{' (expression (',' expression)*) '}';

coordinateLiteral:
    '@'? '[' expression (',' expression)* ']' -> ^(COORDINATE expression+);

label:        
	IDENTIFIER ('.' IDENTIFIER)* ->  ^(IDENTCHAIN IDENTIFIER+)
	| 'true'
        | 'false'
	| NUMBER
	| STRING;

/*literal:
        'true' |
        'false' |
	NUMBER |
	STRING;*/

LEFT_BRACKET: '(';
RIGHT_BRACKET: ')';
LEFT_SQUARE_BRACKET: '[';
RIGHT_SQUARE_BRACKET: ']';
LEFT_CURLY_BRACKET: '{';
RIGTH_CURLY_BRACKET: '}';

TRUE_LIT: 'true';
FALSE_LIT: 'false';

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

COMMENT
    :   /*'//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |  */ //'/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

IDENTIFIER: ('a'..'z' |'A'..'Z' |'_' ) ('a'..'z' |'A'..'Z' |'_' |'0'..'9' )*;

fragment INTEGER: ('0'..'9')+ ;

NUMBER: INTEGER ('.' INTEGER)?;

STRING
    :  '"' ( ~'"')* '"'
    ;

fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
ESC_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    /*|   UNICODE_ESC
    |   OCTAL_ESC*/
    ;
/*
fragment
OCTAL_ESC
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UNICODE_ESC
    :   '\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
    ;*/

WHITESPACE: (' ' |'\t' |'\n' |'\r' )+ {$channel=HIDDEN;} ;