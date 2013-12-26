tree grammar GeoGenScriptDecls;

options 
{
    tokenVocab	    = GeoGenScript;
    language	    = C;
    ASTLabelType	= pANTLR3_BASE_TREE;
}

scope BlockScope
{
	CodeBlock* codeBlock;
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



script: ^(SCRIPT metadata? ^(DECLARATIONS declaration*) block) { ctx->compiledScript->GetRootCodeBlock().MoveInstructionsFrom(*$block.codeBlock); delete $block.codeBlock; };
        
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

block returns [CodeBlock* codeBlock] 
scope BlockScope;
@init { $BlockScope::codeBlock = new CodeBlock(); $codeBlock = new CodeBlock(); }
: ^(BLOCK (statement { $codeBlock->MoveInstructionsFrom(*$statement.codeBlock); delete $statement.codeBlock; })*);

statement returns [CodeBlock* codeBlock]
@init { $codeBlock = NULL; }
:     
    BREAK { $codeBlock = new CodeBlock(); }
    | CONTINUE { $codeBlock = new CodeBlock(); }
    | variableDeclaration { $codeBlock = new CodeBlock(); }
    | expression { $codeBlock = $expression.codeBlock; }
    | yieldStatement{ $codeBlock = $yieldStatement.codeBlock; }
    | returnStatement { $codeBlock = new CodeBlock(); }
    | whileStatement { $codeBlock = $whileStatement.codeBlock; }
    | forStatement { $codeBlock = new CodeBlock(); }
    | ifStatement { $codeBlock = $ifStatement.codeBlock;}
    | switchStatement { $codeBlock = new CodeBlock(); }
    | block { $codeBlock = $block.codeBlock; };
    
variableDeclaration: ^('var' IDENTIFIER expression?);

yieldStatement returns [CodeBlock* codeBlock]
@init { $codeBlock = new CodeBlock(); } 
: ^(YIELD expression STRING?) 
{ 
	if($STRING == NULL){
		$codeBlock->AddInstruction(new instructions::YieldAsMainInstruction());
	}
	else {
		$codeBlock->AddInstruction(new instructions::YieldAsNamedInstruction((char*)$STRING.text->chars));	
	}
};

returnStatement: ^(RETURN expression?);

whileStatement returns [CodeBlock* codeBlock]
scope BlockScope;
@init { $codeBlock = new CodeBlock(); }
: ^(WHILE expression statement) 
{
	instructions::WhileInstruction* whileInstr = new instructions::WhileInstruction();
	CodeBlock& whileCodeBlock = whileInstr->GetCodeBlock();
	
	whileCodeBlock.MoveInstructionsFrom(*$expression.codeBlock);
	delete $expression.codeBlock;
	
	instructions::IfInstruction* ifInstr = new instructions::IfInstruction();
	ifInstr->GetIfBranchCodeBlock().AddInstruction(new instructions::BreakInstruction(2));
	whileCodeBlock.AddInstruction(ifInstr);
	
	whileCodeBlock.MoveInstructionsFrom(*$statement.codeBlock);
	delete $statement.codeBlock;
	
	codeBlock->AddInstruction(whileInstr);
};

forStatement: ^(FOR ^(INITIALIZATION_EXPRESSION initExpression?) ^(CONDITION_EXPRESSION expression?) ^(INCREMENT_EXPRESSION expression?) statement);

initExpression: 
    ^('var' IDENTIFIER expression)
    | expression;

ifStatement returns [CodeBlock* codeBlock]
@init 
{ 
	bool hasElse = false;
	$codeBlock = new CodeBlock(); 
}: 
^(IF expression ifBranchStatement=statement elseBranchStatement=statement) 
{
	instructions::IfInstruction* ifInstr = new instructions::IfInstruction();
	
	ifInstr->GetIfBranchCodeBlock().MoveInstructionsFrom(*$ifBranchStatement.codeBlock);
	delete $ifBranchStatement.codeBlock;
	
	ifInstr->GetElseBranchCodeBlock().MoveInstructionsFrom(*$elseBranchStatement.codeBlock);
	delete $elseBranchStatement.codeBlock;
	
	codeBlock->AddInstruction(ifInstr);
};

switchStatement: ^(SWITCH expression normalCase* defaultCase?);

normalCase: ^(CASE label ^(BLOCK statement*));
defaultCase: ^(DEFAULT ^(BLOCK statement*));

expression returns [CodeBlock* codeBlock]
@init { $codeBlock = new CodeBlock(); } 
:
	^('=' e1=expression e2=expression)
	| ^('+=' e1=expression e2=expression)
	| ^('-=' e1=expression e2=expression)
	| ^('*=' e1=expression e2=expression)
	| ^('/=' e1=expression e2=expression)
	| ^('%=' e1=expression e2=expression)
	| ^('<<=' e1=expression e2=expression)
	| ^('>>=' e1=expression e2=expression)
	| ^('&=' e1=expression e2=expression)
	| ^('|=' e1=expression e2=expression)
	| ^('is' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("is"), 2));}
	| ^('?' expression  expression expression)
	| ^('||' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("||"), 2));}
	| ^('&&' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("&&"), 2));}
	| ^('^' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("^"), 2));}
	| ^('&' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("&"), 2));}
	| ^('==' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("=="), 2));}
	| ^('!=' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("!="), 2));}
	| ^('<' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<"), 2));}
	| ^('<=' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<="), 2));}
	| ^('>' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">"), 2));}
	| ^('>=' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">="), 2));}
	| ^('<<' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<<"), 2));}
	| ^('>>' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">>"), 2));}
	| ^('+' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("+"), 2));}
	| ^('-' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("-"), 2));}
	| ^('*' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("*"), 2));}
	| ^('/' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("/"), 2));}
	| ^('%' e1=expression e2=expression) { $codeBlock->MoveInstructionsFrom(*$e1.codeBlock); delete $e1.codeBlock; $codeBlock->MoveInstructionsFrom(*$e2.codeBlock); delete $e2.codeBlock; $codeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("\%"), 2));}
	| ^('.' expression expression)
	| ^('(' expression expression*)
	| ^('[' expression expression*)
	| IDENTIFIER
	//collectionLiteral |
	| coordinateLiteral
	| 'true' { $codeBlock->AddInstruction(new instructions::LoadConstBooleanInstruction(true));}
	| 'false' { $codeBlock->AddInstruction(new instructions::LoadConstBooleanInstruction(false));}
	| NUMBER { $codeBlock->AddInstruction(new instructions::LoadConstNumberInstruction(StringToNumber((char*)$NUMBER.text->chars)));}
	| STRING { $codeBlock->AddInstruction(new instructions::LoadConstStringInstruction((char*)$STRING.text->chars));};


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