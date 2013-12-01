tree grammar GeoGenScriptDecls;

options 
{
    tokenVocab	    = GeoGenScript;
    language	    = C;
    ASTLabelType	= pANTLR3_BASE_TREE;
}


@includes {
	#include "../../GeoGen.hpp"
	using namespace geogen;
	using namespace geogen::compiler;
}


@context {
	geogen::compiler::CompiledScript* compiledScript;
}

script: ^(SCRIPT metadata? ^(DECLARATIONS declaration*) block);
        
metadata: ^('metadata' keyValueCollection);

keyValueCollection: ^(COLLECTION keyValuePair*);

keyValuePair: 
	^(IDENTIFIER keyValueValue)
	| ^(NUMBER keyValueValue '@'?);

keyValueValue: expression | keyValueCollection;

declaration: enumDeclaration | functionDeclaration;

enumDeclaration: ^('enum' IDENTIFIER enumValues?);

enumValues: enumValue+;

enumValue: ^(IDENTIFIER expression?);

functionDeclaration: ^('function' IDENTIFIER formalParameters block) {
	FunctionDefinition* decl = new FunctionDefinition((char*)$IDENTIFIER.text->chars);
	
	ctx->compiledScript->GetGlobalFunctionDefinitions().AddItem(decl);
	ctx->compiledScript->GetSymbolNameTable().AddName(decl->GetName());
};

formalParameters: ^(PARAMETERS IDENTIFIER*);

block: ^(BLOCK statement*);

statement:     
    BREAK
    | CONTINUE
    | variableDeclaration
    | expression
    | yieldStatement
    | returnStatement
    | whileStatement
    | forStatement
    | ifStatement
    | switchStatement
    | block;
    
variableDeclaration: ^('var' IDENTIFIER expression?);

yieldStatement: ^(YIELD expression STRING?);


returnStatement: ^(RETURN expression?);

whileStatement: ^(WHILE expression statement); 

forStatement: ^(FOR ^(INITIALIZATION_EXPRESSION initExpression?) ^(CONDITION_EXPRESSION expression?) ^(INCREMENT_EXPRESSION expression?) statement);

initExpression: 
    ^('var' IDENTIFIER expression)
    | expression;

ifStatement: ^(IF expression statement statement?);

switchStatement: ^(SWITCH expression normalCase* defaultCase?);

normalCase: ^(CASE label ^(BLOCK statement*));
defaultCase: ^(DEFAULT ^(BLOCK statement*));

expression:
	^(('=' | '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '|=' | '^=' | 'is') expression expression)
	| ^('?' expression  expression expression)
	| ^('||' expression expression)
	| ^('&&' expression expression)
	| ^('^' expression expression)
	| ^('&' expression expression)
	| ^(('==' | '!=') expression expression)
	| ^(('<' | '<=' | '>' | '>=') expression expression)
	| ^(('<<' | '>>') expression expression)
	| ^(('+' | '-') expression expression)
	| ^(('*' | '/' | '%') expression expression) 
	| ^('.' expression expression)
	| ^('(' expression expression*)
	| ^('[' expression expression*)
	| IDENTIFIER
	//collectionLiteral |
	| coordinateLiteral
	| 'true'
	| 'false'
	| NUMBER
	| STRING;


//prio14Operator: ;
/*prio14Expression: ;	

prio13Expression: ;

prio12Operator: ;
prio12Expression: ;

prio11Operator: '&&';
prio11Expression: ;

prio10Operator: '|';
prio10Expression: ;

prio9Operator: '^';
prio9Expression: ^(prio9Operator prio8Expression prio8Expression);

prio8Operator: '&';
prio8Expression: ;*/

//prio7Operator: ;
//prio7Expression: ^(('==' | '!=') prio6Expression prio6Expression);

//prio6Operator: ;
//prio6Expression: ^(('<' | '<=' | '>' | '>=') prio5Expression prio5Expression);

//prio5Operator: ;
//pio5Expression: ^(('<<' | '>>') prio4Expression prio4Expression);

//prio4Operator: ;
//pio4Expression: ^(('+' | '-') prio3Expression prio3Expression);

//prio3Operator: ;
//prio3Expression: ^(('*' | '/' | '%') prio2Expression prio2Expression);

//prio2PrefixOperator: '++' | '--' | '!' | '+' | '-';
//prio2PostfixOperator: '++' | '--';
//prio2Expression: prio2PrefixOperator* prio1Expression prio2PostfixOperator*;  
//prio2Expression/
//	:	 prio1Expression;
//prio2Expression: prio1Expression (('++' | '--' | '!') prio1Expression)*;

//prio2Expression:	('++' | '--' | '!' | '+' | '-')? prio1Expression ('++' | '--' | '!' | '+' | '-')?;

//prio2ExpressionPost: prio1Expression ('++' | '--')+ | prio1Expression ;  


/*prio1Expression:
    ^('.' prio0Expression prio0Expression)
    | ^('(' prio0Expression expression*)
    | ^('[' prio0Expression expression*);*/

/*prio0Expression:  

    | IDENTIFIER
    //collectionLiteral |
    | coordinateLiteral
    | 'true'
    | 'false'
    | NUMBER
    | STRING 
    ; // expression!
*/

/*collectionLiteral: 
    keyValueCollection |
    unkeyedCollectionLiteral;*/

unkeyedCollectionLiteral:
    '{' (expression (',' expression)*) '}';

coordinateLiteral: ^(COORDINATE expression+);

label:        
	^(IDENTCHAIN IDENTIFIER+)
	| 'true'
        | 'false'
	| NUMBER
	| STRING;