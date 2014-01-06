tree grammar GeoGenScriptDecls;

options 
{
    tokenVocab	    = GeoGenScript;
    language	    = C;
    ASTLabelType	= pANTLR3_BASE_TREE;
}

scope BlockScope
{
	int breakCodeBlockLevel;
	int continueCodeBlockLevel;
}

@includes {
	#include <memory>

	#include "../../GeoGen.hpp"
	using namespace std;
	using namespace geogen;
	using namespace geogen::compiler;
	
	void handleTreeWalkerError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames);
}


@context {
	geogen::compiler::CompiledScript* compiledScript;
	
	pANTLR3_BASE_TREE_ADAPTOR	adaptor;
	pANTLR3_VECTOR_FACTORY		vectors;
	
	bool isInFunction = false;
	bool isInLoop = false;
	int codeBlockLevel = 0;
}


@apifuncs {
	RECOGNIZER->displayRecognitionError = handleTreeWalkerError;
}

@members {
    	void handleTreeWalkerError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames) { 
    	    pANTLR3_EXCEPTION ex = recognizer->state->exception;
    	    pANTLR3_BASE_TREE currentTree = (pANTLR3_BASE_TREE)(recognizer->state->exception->token);
    	    
    	    // Char pos odlisna pro tree parser
    	    CodeLocation location(recognizer->state->exception->line, currentTree->getCharPositionInLine(currentTree));
    	    
    	    string expectedTokenName = "";
    	    if(ex->expecting == ANTLR3_TOKEN_EOF)
    	    {
    	    	expectedTokenName = "EOF";
    	    }
			else if (ex->expecting > 0)
    	    {
    	    	expectedTokenName = (char*)tokenNames[ex->expecting];
    	    }
    	    
    	    throw UnexpectedTokenException(GGE1201_UnexpectedToken, location, expectedTokenName/*, currentTokenName, currentTokenText*/);
    	    
    	    /*switch  (ex->type)
    	    {
    		case:			    
    		    throw GeoGenException(GGE1201_UnexpectedToken); 
    	    }*/
    	 }
}


script: ^(SCRIPT metadata? ^(DECLARATIONS declaration*) block) { ctx->compiledScript->GetRootCodeBlock().MoveInstructionsFrom(*$block.returnCodeBlock); delete $block.returnCodeBlock; };
        
metadata: ^('metadata' metadataKeyValueCollection) { ctx->compiledScript->SetMetadata(dynamic_cast<MetadataKeyValueCollection*>($metadataKeyValueCollection.value)); };

metadataKeyValueCollection returns [MetadataValue* value]
@init {
	MetadataKeyValueCollection* ret = new MetadataKeyValueCollection();
	$value = ret;	
} : ^(COLLECTION (metadataKeyValuePair { ret->AddItem($metadataKeyValuePair.name, $metadataKeyValuePair.value); delete $metadataKeyValuePair.name; })*);
	
metadataKeyValuePair returns [char* name, MetadataValue* value] @init{ $value = NULL; }: 
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

functionDeclaration
//scope BlockScope;
//scope { SymbolDefinitionTable<VariableDefinition>* localVariableDefinitions }
@init { ctx->isInFunction = true; /*functionDeclaration::localVariableDefinitions = new SymbolDefinitionTable<VariableDefinition>();*/}
@after { ctx->isInFunction = false; /*functionDeclaration::localVariableDefinitions = NULL;*/ }
: ^('function' name=IDENTIFIER ^(PARAMETERS formalParameters+=IDENTIFIER*) block)
{
	ScriptFunctionDefinition* decl = new ScriptFunctionDefinition((char*)$name.text->chars, $formalParameters != NULL ? $formalParameters->count : 0);

	//SymbolDefinitionTable<VariableDefinition>& varDecls = decl->GetLocalVariableDefinitions();	
	
	CodeBlock& codeBlock = decl->GetRootCodeBlock();
	if($formalParameters != NULL)
	{

	        
	        for(unsigned i = 0; i < $formalParameters->count; i++)
	        {
			pANTLR3_BASE_TREE tree = (pANTLR3_BASE_TREE)$formalParameters->elements[i].element;
			//varDecls.AddItem(new ScriptVariableDefinition(std::string((char*)tree->getText(tree)->chars)));
		        codeBlock.AddInstruction(new instructions::StoreLocalValueInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex((char*)tree->getText(tree)->chars)));	
		}
	             	
	        codeBlock.MoveInstructionsFrom(CodeBlock()); // todo: WTF?
	}
	
	codeBlock.MoveInstructionsFrom(*$block.returnCodeBlock);
	delete $block.returnCodeBlock;

	// Add null to end of each function for case it had no return. If it has, this instruction will never be reached.
	codeBlock.AddInstruction(new instructions::LoadNullInstruction());
	
	//SymbolDefinitionTable<VariableDefinition>* d = functionDeclaration::localVariableDefinitions;
	//varDecls.MoveItemsFrom(*functionDeclaration::localVariableDefinitions);

	if (!ctx->compiledScript->GetGlobalFunctionDefinitions().AddItem(decl)){
		throw SymbolRedefinitionException(GGE1306_FunctionAlreadyDefined, decl->GetName());
	}
        
        ctx->compiledScript->GetSymbolNameTable().AddName(decl->GetName());
};

block returns [CodeBlock* returnCodeBlock] 
@init { auto_ptr<CodeBlock> codeBlock(new CodeBlock()); $returnCodeBlock = NULL; }
@after { $returnCodeBlock = codeBlock.release(); }
: ^(BLOCK (statement { codeBlock.get()->MoveInstructionsFrom(*$statement.returnCodeBlock); delete $statement.returnCodeBlock; })*);

statement returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = NULL; }
:     
    BREAK 
    { 
    	if(!ctx->isInLoop)
    	{
    		throw CompilerException(GGE1301_InvalidBreak);
    	}
    
    	$returnCodeBlock = new CodeBlock();  
    	$returnCodeBlock->AddInstruction(new instructions::BreakInstruction(ctx->codeBlockLevel - $BlockScope::breakCodeBlockLevel));
    }
    | CONTINUE 
    { 
    	if(!ctx->isInLoop)
    	{
    		throw CompilerException(GGE1303_InvalidContinue);
    	}
    
    	$returnCodeBlock = new CodeBlock();  
    	$returnCodeBlock->AddInstruction(new instructions::ContinueInstruction(ctx->codeBlockLevel - $BlockScope::continueCodeBlockLevel));
    }
    | variableDeclaration { $returnCodeBlock = $variableDeclaration.returnCodeBlock; }
    | expression { $returnCodeBlock = new CodeBlock(); $returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); delete $expression.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::PopInstruction());}
    | yieldStatement{ $returnCodeBlock = $yieldStatement.returnCodeBlock; }
    | returnStatement { $returnCodeBlock = $returnStatement.returnCodeBlock; }
    | whileStatement { $returnCodeBlock = $whileStatement.returnCodeBlock; }
    | forStatement { $returnCodeBlock = $forStatement.returnCodeBlock; }
    | ifStatement { $returnCodeBlock = $ifStatement.returnCodeBlock;}
    | switchStatement { $returnCodeBlock = new CodeBlock(); }
    | block { $returnCodeBlock = $block.returnCodeBlock; };
    
variableDeclaration returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
: ^('var' IDENTIFIER expression?)
{
	char* name = (char*)$IDENTIFIER.text->chars;
	/*if(ctx->isInFunction)
	{
		functionDeclaration::localVariableDefinitions->AddItem(new VariableDefinition(name));
	}
	else 
	{*/
	//ctx->compiledScript->GetGlobalVariableDefinitions().AddItem(new VariableDefinition(name));
	//	}
};

yieldStatement returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
: ^(YIELD expression STRING?) 
{ 
	if($STRING == NULL){
		$returnCodeBlock->AddInstruction(new instructions::YieldAsMainInstruction());
	}
	else 
	{
		$returnCodeBlock->AddInstruction(new instructions::YieldAsNamedInstruction((char*)$STRING.text->chars));	
	}
};

returnStatement returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = NULL; } 
: ^(RETURN (expression { $returnCodeBlock = new CodeBlock(); $returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); delete $expression.returnCodeBlock; } )?)
{
	if($returnCodeBlock == NULL)
	{
		$returnCodeBlock = new CodeBlock();	
		$returnCodeBlock->AddInstruction(new instructions::LoadNullInstruction());
	}

	if(!ctx->isInFunction)
	{
		throw CompilerException(GGE1304_InvalidReturn);
	}

	$returnCodeBlock->AddInstruction(new instructions::BreakInstruction(ctx->codeBlockLevel));
};

whileStatement returns [CodeBlock* returnCodeBlock]
scope BlockScope;
@init { $returnCodeBlock = new CodeBlock(); ctx->codeBlockLevel++; $BlockScope::breakCodeBlockLevel = ctx->codeBlockLevel; $BlockScope::continueCodeBlockLevel = ctx->codeBlockLevel; ctx->isInLoop = true; }
@after { ctx->codeBlockLevel--; ctx->isInLoop = false;}
: ^(WHILE expression statement) 
{
	instructions::WhileInstruction* whileInstr = new instructions::WhileInstruction();
	CodeBlock& whileCodeBlock = whileInstr->GetCodeBlock();
	
	whileCodeBlock.MoveInstructionsFrom(*$expression.returnCodeBlock);
	delete $expression.returnCodeBlock;
	
	instructions::IfInstruction* ifInstr = new instructions::IfInstruction();
	ifInstr->GetIfBranchCodeBlock().AddInstruction(new instructions::BreakInstruction(1));
	whileCodeBlock.AddInstruction(ifInstr);
	
	whileCodeBlock.MoveInstructionsFrom(*$statement.returnCodeBlock);
	delete $statement.returnCodeBlock;
	
	$returnCodeBlock->AddInstruction(whileInstr);
};

forStatement returns [CodeBlock* returnCodeBlock]
scope BlockScope;
@init 
{ 
	$returnCodeBlock = new CodeBlock(); 
	CodeBlock* initExpressionCodeBlock = NULL; 
	CodeBlock* conditionExpressionCodeBlock = NULL; 
	CodeBlock* incrementExpressionCodeBlock = NULL; 
	ctx->codeBlockLevel++; 
	ctx->isInLoop = true;
	$BlockScope::breakCodeBlockLevel = ctx->codeBlockLevel; 
	$BlockScope::continueCodeBlockLevel = ctx->codeBlockLevel; 
}
@after { ctx->codeBlockLevel--; ctx->isInLoop = false; }
: ^(FOR 
	(^(INITIALIZATION_EXPRESSION initExpression { initExpressionCodeBlock = $initExpression.returnCodeBlock; } ))? 
	(^(CONDITION_EXPRESSION conditionExpression=expression { conditionExpressionCodeBlock = $conditionExpression.returnCodeBlock; } ))? 
	(^(INCREMENT_EXPRESSION incrementExpression=expression { incrementExpressionCodeBlock = $incrementExpression.returnCodeBlock; } ))? 
	statement)
{
	if(initExpressionCodeBlock != NULL)
	{
		$returnCodeBlock->MoveInstructionsFrom(*initExpressionCodeBlock);
		delete initExpressionCodeBlock;
	}
	
	instructions::WhileInstruction* whileInstr = new instructions::WhileInstruction();
	CodeBlock& whileCodeBlock = whileInstr->GetCodeBlock();
	
	if(conditionExpressionCodeBlock != NULL)
	{
		whileCodeBlock.MoveInstructionsFrom(*conditionExpressionCodeBlock);
		delete conditionExpressionCodeBlock;
		
		instructions::IfInstruction* ifInstr = new instructions::IfInstruction();
		ifInstr->GetIfBranchCodeBlock().AddInstruction(new instructions::BreakInstruction(1));
		whileCodeBlock.AddInstruction(ifInstr);
	}

	whileCodeBlock.MoveInstructionsFrom(*$statement.returnCodeBlock);
	delete $statement.returnCodeBlock;
	
	if(incrementExpressionCodeBlock != NULL)
	{
		whileCodeBlock.MoveInstructionsFrom(*incrementExpressionCodeBlock);
		delete incrementExpressionCodeBlock;
		
		$returnCodeBlock->AddInstruction(new instructions::PopInstruction());
	}
	
	$returnCodeBlock->AddInstruction(whileInstr);
};

initExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = NULL; } 
:
    variableDeclaration { $returnCodeBlock = $variableDeclaration.returnCodeBlock; }
    | expression { $returnCodeBlock = $expression.returnCodeBlock; };

ifStatement returns [CodeBlock* returnCodeBlock]
scope BlockScope;
@init 
{ 
	$returnCodeBlock = new CodeBlock(); 
}: 
^(IF expression ifBranchStatement=statement elseBranchStatement=statement) 
{
	instructions::IfInstruction* ifInstr = new instructions::IfInstruction();
	
	ifInstr->GetIfBranchCodeBlock().MoveInstructionsFrom(*$ifBranchStatement.returnCodeBlock);
	delete $ifBranchStatement.returnCodeBlock;
	
	ifInstr->GetElseBranchCodeBlock().MoveInstructionsFrom(*$elseBranchStatement.returnCodeBlock);
	delete $elseBranchStatement.returnCodeBlock;
	
	$returnCodeBlock->AddInstruction(ifInstr);
};

switchStatement: ^(SWITCH expression normalCase* defaultCase?);

normalCase: ^(CASE label ^(BLOCK statement*));
defaultCase: ^(DEFAULT ^(BLOCK statement*));

expression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
:
	^('=' lvalueExpression rvalueExpression=expression){ $returnCodeBlock->MoveInstructionsFrom(*$rvalueExpression.returnCodeBlock); delete $rvalueExpression.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("="), 1));$returnCodeBlock->MoveInstructionsFrom(*$lvalueExpression.returnCodeBlock); delete $lvalueExpression.returnCodeBlock; }
	| ^('+=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("+="), 2));}
	| ^('-=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("-="), 2));}
	| ^('*=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("*="), 2));}
	| ^('/=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("/="), 2));}
	| ^('%=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("\%="), 2));}
	| ^('<<=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<<="), 2));}
	| ^('>>=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">>="), 2));}
	| ^('&=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("&&="), 2));}
	| ^('|=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("||="), 2));}
	| ^('is' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("is"), 2));}
	| conditionalOperatorExpression { $returnCodeBlock->MoveInstructionsFrom(*$conditionalOperatorExpression.returnCodeBlock); delete $conditionalOperatorExpression.returnCodeBlock;} 
	| ^('||' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("||"), 2));}
	| ^('&&' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("&&"), 2));}
	| ^('^' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("^"), 2));}
	| ^('&' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("&"), 2));}
	| ^('==' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("=="), 2));}
	| ^('!=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("!="), 2));}
	| ^('<' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<"), 2));}
	| ^('<=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<="), 2));}
	| ^('>' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">"), 2));}
	| ^('>=' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">="), 2));}
	| ^('<<' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("<<"), 2));}
	| ^('>>' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex(">>"), 2));}
	| ^('+' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("+"), 2));}
	| ^('-' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("-"), 2));}
	| ^('*' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("*"), 2));}
	| ^('/' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("/"), 2));}
	| ^('%' e1=expression e2=expression) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("\%"), 2));}
	| ^('.' e1=expression IDENTIFIER) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::LoadMemberValueInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex((char*)$IDENTIFIER.text->chars)));}
	| callExpression { $returnCodeBlock->MoveInstructionsFrom(*$callExpression.returnCodeBlock); delete $callExpression.returnCodeBlock;} 
	| indexAccessExpression { $returnCodeBlock->MoveInstructionsFrom(*$indexAccessExpression.returnCodeBlock); delete $indexAccessExpression.returnCodeBlock;} 
	| IDENTIFIER { $returnCodeBlock->AddInstruction(new instructions::LoadGlobalValueInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex((char*)$IDENTIFIER.text->chars))); }
	//collectionLiteral |
	| coordinateLiteral
	| 'true' { $returnCodeBlock->AddInstruction(new instructions::LoadConstBooleanInstruction(true));}
	| 'false' { $returnCodeBlock->AddInstruction(new instructions::LoadConstBooleanInstruction(false));}
	| NUMBER { $returnCodeBlock->AddInstruction(new instructions::LoadConstNumberInstruction(StringToNumber((char*)$NUMBER.text->chars)));}
	| STRING { $returnCodeBlock->AddInstruction(new instructions::LoadConstStringInstruction((char*)$STRING.text->chars));};

callExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); std::vector<CodeBlock*> argumentCodeBlocks; } 
:
^('(' IDENTIFIER (expression { argumentCodeBlocks.push_back($expression.returnCodeBlock); } )*) 
{
	// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}

	$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex((char*)$IDENTIFIER.text->chars), argumentCodeBlocks.size()));
}
|	
^('(' ^('.' e1=expression IDENTIFIER) (e2=expression { argumentCodeBlocks.push_back($e2.returnCodeBlock); } )*) 
{
	// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}

	$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; 
	
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex((char*)$IDENTIFIER.text->chars), argumentCodeBlocks.size()));
}
;

indexAccessExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); std::vector<CodeBlock*> argumentCodeBlocks; } 
:
^('['e1=expression (e2=expression { argumentCodeBlocks.push_back($e2.returnCodeBlock); } )*) 
{
	// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}

	$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; 
	
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("[]"), argumentCodeBlocks.size()));
};

conditionalOperatorExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
:
^('?' conditionExpression=expression ifExpression=expression elseExpression=expression)
{
	$returnCodeBlock->MoveInstructionsFrom(*$conditionExpression.returnCodeBlock);
	delete $conditionExpression.returnCodeBlock;

	instructions::IfInstruction* ifInstruction = new instructions::IfInstruction();
	
	ifInstruction->GetIfBranchCodeBlock().MoveInstructionsFrom(*$ifExpression.returnCodeBlock);
	delete $ifExpression.returnCodeBlock;

	ifInstruction->GetElseBranchCodeBlock().MoveInstructionsFrom(*$elseExpression.returnCodeBlock);
	delete $elseExpression.returnCodeBlock;
	
	$returnCodeBlock->AddInstruction(ifInstruction);
};

lvalueExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
:
	IDENTIFIER { $returnCodeBlock->AddInstruction(new instructions::StoreGlobalValueInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex((char*)$IDENTIFIER.text->chars)));  }
	| ^('.' e1=expression IDENTIFIER) { $returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; $returnCodeBlock->AddInstruction(new instructions::StoreMemberValueInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex((char*)$IDENTIFIER.text->chars)));}
	| lvalueIndexAccessExpression { $returnCodeBlock->MoveInstructionsFrom(*$lvalueIndexAccessExpression.returnCodeBlock); delete $lvalueIndexAccessExpression.returnCodeBlock;} ;

lvalueIndexAccessExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); std::vector<CodeBlock*> argumentCodeBlocks; } 
:
^('['e1=expression (e2=expression { argumentCodeBlocks.push_back($e2.returnCodeBlock); } )*) 
{
	// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}

	$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; 
	
	// +1 for the value being written into the array
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(ctx->compiledScript->GetSymbolNameTable().GetNameIndex("[]="), argumentCodeBlocks.size() + 1));
};

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