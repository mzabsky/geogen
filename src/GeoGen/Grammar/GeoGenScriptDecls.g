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
	
	pANTLR3_BASE_TREE_ADAPTOR	adaptor;
	pANTLR3_VECTOR_FACTORY		vectors;
}

script: ^(SCRIPT metadata? ^(DECLARATIONS declaration*) block) { ctx->compiledScript->GetRootCodeBlock().MoveInstructionsFrom(*$block.returnCodeBlock); };
        
metadata: ^('metadata' metadataKeyValueCollection) { ctx->compiledScript->SetMetadata(dynamic_cast<MetadataKeyValueCollection*>($metadataKeyValueCollection.value)); };

metadataKeyValueCollection returns [MetadataValue* value]
@init {
	MetadataKeyValueCollection* ret = new MetadataKeyValueCollection();
	$value = ret;	
} : ^(COLLECTION (metadataKeyValuePair { ret->AddItem($metadataKeyValuePair.name, $metadataKeyValuePair.value); })*);
	
metadataKeyValuePair returns [std::string name, MetadataValue* value] @init{ $value = NULL; }: 
	^(IDENTIFIER metadataKeyValueValue) { $name = (char*)$IDENTIFIER.text->chars; $value = $metadataKeyValueValue.value; }
	| ^(NUMBER metadataKeyValueValue '@'?)  { $name = (char*)$NUMBER.text->chars; $value = $metadataKeyValueValue.value; };

metadataKeyValueValue returns [MetadataValue* value] @init{ $value = NULL; }: 
	STRING  { $value = new MetadataString((char*)$STRING.text->chars); }
	| NUMBER { $value = new MetadataNumber(0); }
	| IDENTIFIER { $value = new MetadataIdentifier((char*)$IDENTIFIER.text->chars); }
	| metadataKeyValueCollection { $value = $metadataKeyValueCollection.value; };

keyValueCollection: ^(COLLECTION keyValuePair*);

keyValuePair: 
	^(IDENTIFIER keyValueValue)
	| ^(NUMBER keyValueValue '@'?);

keyValueValue: expression | keyValueCollection;

declaration: enumDeclaration | functionDeclaration;

enumDeclaration: ^('enum' IDENTIFIER enumValues?);

enumValues: enumValue+;

enumValue: ^(IDENTIFIER expression?);

functionDeclaration: ^('function' name=IDENTIFIER ^(PARAMETERS formalParameters+=IDENTIFIER*) block) {
	ScriptFunctionDefinition* decl = new ScriptFunctionDefinition((char*)$name.text->chars, $formalParameters != NULL ? $formalParameters->count : 0);
	
	if($formalParameters != NULL)
	{
	        SymbolDefinitionTable<VariableDefinition>& varDecls = decl->GetLocalVariableDefinitions();
	        CodeBlock& codeBlock = decl->GetRootCodeBlock();
	        for(int i = 0; i < $formalParameters->count; i++)
	        {
			pANTLR3_BASE_TREE tree = (pANTLR3_BASE_TREE)$formalParameters->elements[i].element;
			varDecls.AddItem(new ScriptVariableDefinition(std::string((char*)tree->getText(tree)->chars)));
		        codeBlock.AddInstruction(new instructions::StoreLocalValueInstruction(i));	
		}
	             	
	        codeBlock.MoveInstructionsFrom(CodeBlock());
	}

        ctx->compiledScript->GetGlobalFunctionDefinitions().AddItem(decl);
        ctx->compiledScript->GetSymbolNameTable().AddName(decl->GetName());
};

block returns [CodeBlock* returnCodeBlock] 
scope { CodeBlock* codeBlock; }
@init { $block::codeBlock = new CodeBlock(); $returnCodeBlock = $block::codeBlock; }
: ^(BLOCK statement*);

statement:     
    BREAK
    | CONTINUE
    | variableDeclaration
    | expression { $block::codeBlock->AddInstruction(new instructions::PopInstruction());}
    | yieldStatement
    | returnStatement
    | whileStatement
    | forStatement
    | ifStatement
    | switchStatement
    | block;
    
variableDeclaration: ^('var' IDENTIFIER expression?);

yieldStatement: ^(YIELD expression STRING?) 
{ 
	if($STRING == NULL){
		$block::codeBlock->AddInstruction(new instructions::YieldAsMainInstruction());
	}
	else {
		$block::codeBlock->AddInstruction(new instructions::YieldAsNamedInstruction((char*)$STRING.text->chars));	
	}
};

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
	^('=' expression expression)
	| ^('+=' expression expression)
	| ^('-=' expression expression)
	| ^('*=' expression expression)
	| ^('/=' expression expression)
	| ^('%=' expression expression)
	| ^('<<=' expression expression)
	| ^('>>=' expression expression)
	| ^('&=' expression expression)
	| ^('|=' expression expression)
	| ^('is' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("is"), 2));}
	| ^('?' expression  expression expression)
	| ^('||' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("||"), 2));}
	| ^('&&' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("&&"), 2));}
	| ^('^' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("^"), 2));}
	| ^('&' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("&"), 2));}
	| ^('==' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("=="), 2));}
	| ^('!=' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("!="), 2));}
	| ^('<' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<"), 2));}
	| ^('<=' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<="), 2));}
	| ^('>' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">"), 2));}
	| ^('>=' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">="), 2));}
	| ^('<<' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<<"), 2));}
	| ^('>>' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">>"), 2));}
	| ^('+' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("+"), 2));}
	| ^('-' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("-"), 2));}
	| ^('*' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("*"), 2));}
	| ^('/' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("/"), 2));}
	| ^('%' expression expression) { $block::codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("\%"), 2));}
	| ^('.' expression expression)
	| ^('(' expression expression*)
	| ^('[' expression expression*)
	| IDENTIFIER
	//collectionLiteral |
	| coordinateLiteral
	| 'true' { $block::codeBlock->AddInstruction(new instructions::LoadConstBooleanInstruction(true));}
	| 'false' { $block::codeBlock->AddInstruction(new instructions::LoadConstBooleanInstruction(false));}
	| NUMBER { $block::codeBlock->AddInstruction(new instructions::LoadConstNumberInstruction(StringToNumber((char*)$NUMBER.text->chars)));}
	| STRING { $block::codeBlock->AddInstruction(new instructions::LoadConstStringInstruction((char*)$STRING.text->chars));};


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