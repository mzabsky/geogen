grammar GeoGenScript;
options { 
	//backtrack = true; 
	language = C;
	output=AST; 
	ASTLabelType	= pANTLR3_BASE_TREE;
}


tokens { 
	SCRIPT; 
	COLLECTION; 
	DECLARATIONS; 
	BLOCK; 
	PARAMETERS;
	VALUES; 
	COORDINATE; 
	IDENTCHAIN; 
	OPERATOR_CALL; 
	OPERATOR_SUBSCRIPT; 
	INITIALIZATION_EXPRESSION; 
	CONDITION_EXPRESSION; 
	INCREMENT_EXPRESSION; 
	OPERATOR_INCREMENT_POST; 
	OPERATOR_INCREMENT_PRE; 
	OPERATOR_DECREMENT_POST; 
	OPERATOR_DECREMENT_PRE; 
	OPERATOR_MINUS_UN;
	OPERATOR_PLUS_UN;
}

@includes {
	#include <memory>
	#include <stdexcept>

	#include "../../GeoGen.hpp"
	using namespace std;
	using namespace geogen;
	using namespace geogen::compiler;
	
	void handleParserError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames);
}


@lexer::includes {
	#include <memory>
	#include <stdexcept>

	#include "../../GeoGen.hpp"
	using namespace std;
	using namespace geogen;
	using namespace geogen::compiler;
	
	void handleLexerError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames);
}

@lexer::apifuncs {
	RECOGNIZER->displayRecognitionError = handleLexerError;
}

@lexer::members {
	void handleLexerError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames) { 
		pANTLR3_LEXER lexer = (pANTLR3_LEXER)(recognizer->super);
		pANTLR3_EXCEPTION ex = lexer->rec->state->exception;
		throw UnexpectedCharacterException(CodeLocation(recognizer->state->exception->line, ex->charPositionInLine), ex->c); 
	} 
}

@parser::apifuncs {
	RECOGNIZER->displayRecognitionError = handleParserError;
}

@parser::members {
    	void handleParserError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames) { 
    	    pANTLR3_EXCEPTION ex = recognizer->state->exception;
    	    pANTLR3_COMMON_TOKEN currentToken = (pANTLR3_COMMON_TOKEN)(recognizer->state->exception->token);
    	    
    	    // Char pos odlisna pro tree parser
    	    CodeLocation location(recognizer->state->exception->line, recognizer->state->exception->charPositionInLine);

    	    String expectedTokenName = GG_STR("");
    	    if(ex->expecting == ANTLR3_TOKEN_EOF)
    	    {
    	    	expectedTokenName = GG_STR("EOF");
    	    }
	    else if (ex->expecting > 0)
    	    {
    	    	expectedTokenName = AnyStringToString((char*)tokenNames[ex->expecting]);
    	    }
    	    
    	    throw UnexpectedTokenException(GGE1201_UnexpectedToken, location, expectedTokenName);

    	 }
}

script: (decls+=declaration)* (metadata (stmts+=statement | decls+=declaration)* | stmts+=statement (stmts+=statement | decls+=declaration)*)? EOF -> ^(SCRIPT metadata ^(DECLARATIONS $decls*) ^(BLOCK $stmts*));
        
metadata: 'metadata' keyValueCollection -> ^('metadata' keyValueCollection);

keyValueCollection: '{' (keyValuePair (',' keyValuePair )*)? '}' -> ^(COLLECTION keyValuePair*);

keyValuePair: 
	IDENTIFIER ':' keyValueValue -> ^(IDENTIFIER keyValueValue)
	| NUMBER ':' keyValueValue -> ^(NUMBER keyValueValue);

keyValueValue: expression | keyValueCollection;

declaration: enumDeclaration | functionDeclaration;

enumDeclaration: 'enum' IDENTIFIER '{' enumValues? '}' -> ^('enum' IDENTIFIER ^(VALUES enumValues));

enumValues: values+=enumValue (',' values+=enumValue)* -> $values+;

enumValue: 
    IDENTIFIER ( '=' NUMBER)? -> ^(IDENTIFIER NUMBER?);

functionDeclaration: 'function' IDENTIFIER '(' formalParameters? ')' block -> ^('function' IDENTIFIER ^(PARAMETERS formalParameters) block);

formalParameters: IDENTIFIER (',' IDENTIFIER)* -> IDENTIFIER*;

block: '{' statement* '}' -> ^(BLOCK statement*);

statement:     
    BREAK ';'!
    | CONTINUE ';'!
    | variableDeclaration ';' -> variableDeclaration
    | globalVariableDeclaration ';' -> globalVariableDeclaration
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

globalVariableDeclaration: 'global' IDENTIFIER ('=' expression)? -> ^('global' IDENTIFIER expression?);

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
    | 'if' '(' expression ')' statement  -> ^(IF expression statement ^(BLOCK))
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

prio13Expression: prio12Expression ('?'^ prio13Expression ':'! prio12Expression)*;

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
prio3Expression: prio2ExpressionPre (('*' | '/' | '%')^ prio2ExpressionPre)*;

prio2PrefixOperator: ('++' -> OPERATOR_INCREMENT_PRE) | ('--' -> OPERATOR_DECREMENT_PRE) | ('!' -> OPERATOR_NOT) | ('+' -> OPERATOR_PLUS_UN) | ('-' -> OPERATOR_MINUS_UN) | ('@' -> OPERATOR_RELATIVE) | {false}? coordinateLiteral;
prio2PostfixOperator: ('++' -> OPERATOR_INCREMENT_POST) | ('--' -> OPERATOR_DECREMENT_POST) | {false}? coordinateLiteral;
prio2ExpressionPre: prio2ExpressionPost | prio2PrefixOperator^ prio2ExpressionPre; //((prio2PrefixOperator)^)* prio2ExpressionPost;//(('++' -> OPERATOR_INCREMENT_PRE)/*| '--'  | '!' | '+' | '-'*/)* prio1Expression (('++' -> OPERATOR_INCREMENT_POST))*;  

prio2ExpressionPost: (prio1Expression -> prio1Expression) (prio2PostfixOperator -> ^(prio2PostfixOperator $prio2ExpressionPost))*;//(('++' -> OPERATOR_INCREMENT_PRE)/*| '--'  | '!' | '+' | '-'*/)* prio1Expression (('++' -> OPERATOR_INCREMENT_POST))*;  

//prio2Expression
//	:	 prio1Expression;
//prio2Expression: prio1Expression (('++' | '--' | '!') prio1Expression)*;

//prio2Expression:	('++' | '--' | '!' | '+' | '-')? prio1Expression ('++' | '--' | '!' | '+' | '-')?;

//prio2ExpressionPost: prio1Expression ('++' | '--')+ | prio1Expression ;  


prio1Expression:
    prio0Expression (
        '.'^ IDENTIFIER
        | '('^ (expression (','! expression)*)? ')'!
        | '['^ (expression (','! expression)*)? ']'!
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
    '[' expression (',' expression)* ']' -> ^(COORDINATE expression+);

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

METADATA: 'metadata';
ENUM: 'enum';
FUNCTION: 'function';
VAR: 'var';
GLOBAL: 'global';

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
OPERATOR_RELATIVE: '@';

COMMENT
    :   '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

IDENTIFIER: ('a'..'z' |'A'..'Z' |'_' ) ('a'..'z' |'A'..'Z' |'_' |'0'..'9' )*;

fragment INTEGER: ('0'..'9')+ ;

NUMBER: INTEGER ('.' INTEGER)?;

QUOTE   :      '"';
STRING
@init{ pANTLR3_STRING unesc = GETTEXT()->factory->newRaw(GETTEXT()->factory); }
        :       QUOTE ( reg = ~('\\' | '"') { unesc->addc(unesc, reg); }
                        | esc = ESCAPED { unesc->appendS(unesc, GETTEXT()); } )* QUOTE { SETTEXT(unesc); };

fragment
ESCAPED :       '\\'
                ( '\\' { SETTEXT(GETTEXT()->factory->newStr8(GETTEXT()->factory, (pANTLR3_UINT8)"\\")); }
                | '"' { SETTEXT(GETTEXT()->factory->newStr8(GETTEXT()->factory, (pANTLR3_UINT8)"\"")); }
                | 'n' { SETTEXT(GETTEXT()->factory->newStr8(GETTEXT()->factory, (pANTLR3_UINT8)"\n")); }
                | 't' { SETTEXT(GETTEXT()->factory->newStr8(GETTEXT()->factory, (pANTLR3_UINT8)"\t")); }
                | ~('\\' | '"' | 'n' | 't') { throw InvalidEscapeSequenceException(CodeLocation(ctx->pLexer->input->getLine(ctx->pLexer->input), ctx->pLexer->input->getCharPositionInLine(ctx->pLexer->input) - 1), String(1, (char)LA(0))); }
                )
        ;


WHITESPACE: (' ' |'\t' |'\n' |'\r' )+ {$channel=HIDDEN;} ;