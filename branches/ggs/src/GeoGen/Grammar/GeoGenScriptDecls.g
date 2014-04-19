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
	#include <limits>	

	#undef max

	#include "../../GeoGen.hpp"
	using namespace std;
	using namespace geogen;
	using namespace geogen::compiler;
	using namespace geogen::runtime;	
		
	void handleTreeWalkerError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames);
}


@context {
	geogen::runtime::CompiledScript* compiledScript;
	geogen::runtime::CodeBlock* rootCodeBlock;
	
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

	void binaryOperator(pGeoGenScriptDecls ctx, pANTLR3_BASE_TREE operatorToken, CodeBlock* e1, CodeBlock* e2, CodeBlock* returnCodeBlock)
	{
		CodeLocation location(operatorToken->getLine(operatorToken), operatorToken->getCharPositionInLine(operatorToken));
		
		returnCodeBlock->MoveInstructionsFrom(*e2); 
		delete e2;
		
	
		returnCodeBlock->MoveInstructionsFrom(*e1); 
		delete e1; 
		
		returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, (char*)operatorToken->getText(operatorToken)->chars, 2));
	}
	
	void unaryOperator(pGeoGenScriptDecls ctx, pANTLR3_BASE_TREE operatorToken, std::string const& text, CodeBlock* e1, CodeBlock* returnCodeBlock)
	{
		CodeLocation location(operatorToken->getLine(operatorToken), operatorToken->getCharPositionInLine(operatorToken));
	
		returnCodeBlock->MoveInstructionsFrom(*e1); 
		delete e1; 		
		
		returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, text, 1));
	}    	     	 
}


script: ^(SCRIPT metadata? ^(DECLARATIONS declaration*) block) 
{ 
	ctx->rootCodeBlock->MoveInstructionsFrom(*$block.returnCodeBlock); 
	delete $block.returnCodeBlock; 
};
        
metadata: ^('metadata' metadataKeyValueCollection) 
{ 
	ctx->compiledScript->GetMetadata().MoveKeyValuesFrom(*dynamic_cast<MetadataKeyValueCollection*>($metadataKeyValueCollection.value));
	delete $metadataKeyValueCollection.value;
};

metadataKeyValueCollection returns [MetadataValue* value]
@init {
	MetadataKeyValueCollection* ret = new MetadataKeyValueCollection();
	$value = ret;	
} : ^(COLLECTION (metadataKeyValuePair 
	{ 
		if(!ret->AddItem($metadataKeyValuePair.name, $metadataKeyValuePair.value))
		{
			CodeLocation location($COLLECTION.line, $COLLECTION.pos);
			throw CompilerException(GGE1401_MetadataValueAlreadyDefined, location);
		}
		/*delete $metadataKeyValuePair.name; */
	})*);
	
metadataKeyValuePair returns [char* name, MetadataValue* value] @init{ $value = NULL; }: 
	^(IDENTIFIER metadataKeyValueValue) { $name = (char*)$IDENTIFIER.text->chars; $value = $metadataKeyValueValue.value; }
	| ^(NUMBER metadataKeyValueValue '@'?)  { $name = (char*)$NUMBER.text->chars; $value = $metadataKeyValueValue.value; };

metadataKeyValueValue returns [MetadataValue* value] @init{ $value = NULL; }: 
	stringLiteral  { $value = new MetadataString($stringLiteral.value); }
	| TRUE_LIT  { $value = new MetadataBoolean(true); }
	| FALSE_LIT  { $value = new MetadataBoolean(false); }
	| NUMBER { $value = new MetadataNumber(StringToNumber((char*)$NUMBER.text->chars)); }
	| IDENTIFIER { $value = new MetadataIdentifier((char*)$IDENTIFIER.text->chars); }
	| metadataKeyValueCollection { $value = $metadataKeyValueCollection.value; };

keyValueCollection: ^(COLLECTION keyValuePair*);

keyValuePair: 
	^(IDENTIFIER keyValueValue)
	| ^(NUMBER keyValueValue '@'?);

keyValueValue: expression | keyValueCollection;

declaration: enumDeclaration | functionDeclaration;

enumDeclaration: ^(ENUM IDENTIFIER enumValues)
{
	CodeLocation location($ENUM.line, $ENUM.pos);
		
	EnumTypeDefinition* decl = new EnumTypeDefinition(location, (char*)$IDENTIFIER.text->chars, $enumValues.returnEnumValues);
	
	if (!ctx->compiledScript->AddTypeDefinition(decl)){
		throw SymbolRedefinitionException(GGE1308_TypeAlreadyDefined, location, decl->GetName());
	}
};

enumValues returns [map<int, std::string> returnEnumValues]
@init { map<std::string, int> tempEnumValues; int number = -1; }
: ^(VALUES (^(IDENTIFIER (NUMBER { number = (int)StringToNumber((char*)$NUMBER.text->chars); } )?)
	{ 
		std::string valueName = (char*)$IDENTIFIER.text->chars;
	
		CodeLocation enumValueLocation($IDENTIFIER.line, $IDENTIFIER.pos);
		
		if(!IsNumberInt(number))
		{
			throw InvalidSymbolDefinitionException(GGE1310_EnumValueNotInteger, enumValueLocation, valueName);
		}
		
		
		if(!tempEnumValues.insert(std::pair<std::string, int>(valueName, NumberToInt(number))).second)
		{		
			throw SymbolRedefinitionException(GGE1309_EnumValueAlreadyDefined, enumValueLocation, valueName);
		}
		
		number = -1;
	})*)
{
	// Assign unused numbers to values which have int value -1
	for(std::map<std::string, int>::iterator it = tempEnumValues.begin(); it != tempEnumValues.end(); it++)
	{
		if(it->second > -1)
		{
			continue;
		}
	
		int min = std::numeric_limits<int>::max();
		for(std::map<std::string, int>::iterator it2 = tempEnumValues.begin(); it2 != tempEnumValues.end(); it2++)
		{
			int current = it2->second;
			if(current > -1 && current < min)
			{
				min = current;
			}		
		}
		
		if(min == std::numeric_limits<int>::max())
		{
			min = 0;
		}	
				
		min++;
		
		it->second = min;
		
		returnEnumValues.insert(std::pair<int, std::string>(min, it->first));
	}
};

//enumValue: ;

functionDeclaration
//scope BlockScope;
//scope { SymbolDefinitionTable<VariableDefinition>* localVariableDefinitions }
@init { ctx->isInFunction = true; /*functionDeclaration::localVariableDefinitions = new SymbolDefinitionTable<VariableDefinition>();*/}
@after { ctx->isInFunction = false; /*functionDeclaration::localVariableDefinitions = NULL;*/ }
: ^(FUNCTION name=IDENTIFIER ^(PARAMETERS formalParameters+=IDENTIFIER*) block)
{
	CodeLocation location($FUNCTION.line, $FUNCTION.pos);
	ScriptFunctionDefinition* decl = new ScriptFunctionDefinition((char*)$name.text->chars, location, $formalParameters != NULL ? $formalParameters->count : 0);

	//SymbolDefinitionTable<VariableDefinition>& varDecls = decl->GetLocalVariableDefinitions();	
	
	CodeBlock& codeBlock = decl->GetRootCodeBlock();
	if($formalParameters != NULL)
	{

	        
	        for(unsigned i = 0; i < $formalParameters->count; i++)
	        {
			pANTLR3_BASE_TREE tree = (pANTLR3_BASE_TREE)$formalParameters->elements[i].element;
			CodeLocation parameterLocation(tree->getLine(tree), tree->getCharPositionInLine(tree));
			//varDecls.AddItem(new ScriptVariableDefinition(std::string((char*)tree->getText(tree)->chars)));
		        codeBlock.AddInstruction(new instructions::DeclareLocalValueInstruction(location, (char*)tree->getText(tree)->chars));	
		        codeBlock.AddInstruction(new instructions::StoreScopeValueInstruction(location, (char*)tree->getText(tree)->chars));	
		        codeBlock.AddInstruction(new instructions::PopInstruction(location));	
		}
	             	
	        codeBlock.MoveInstructionsFrom(CodeBlock()); // todo: WTF?
	}
	
	codeBlock.MoveInstructionsFrom(*$block.returnCodeBlock);
	delete $block.returnCodeBlock;

	// Add null to end of each function for case it had no return. If it has, this instruction will never be reached.
	codeBlock.AddInstruction(new instructions::LoadNullInstruction(location));
	
	//SymbolDefinitionTable<VariableDefinition>* d = functionDeclaration::localVariableDefinitions;
	//varDecls.MoveItemsFrom(*functionDeclaration::localVariableDefinitions);

	if (!ctx->compiledScript->AddGlobalFunctionDefinition(decl)){
		throw SymbolRedefinitionException(GGE1306_FunctionAlreadyDefined, location, decl->GetName());
	}
        
        //ctx->compiledScript->GetSymbolNameTable().AddName(decl->GetName());
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
	CodeLocation location($BREAK.line, $BREAK.pos);
    
    	if(!ctx->isInLoop)
    	{
    		throw CompilerException(GGE1301_InvalidBreak, location);
    	}
    
    	int a = $BlockScope::breakCodeBlockLevel;
    
    	$returnCodeBlock = new CodeBlock();  
    	$returnCodeBlock->AddInstruction(new instructions::BreakInstruction(location, ctx->codeBlockLevel - $BlockScope::breakCodeBlockLevel + 1));
    }
    | CONTINUE 
    { 
    	CodeLocation location($CONTINUE.line, $CONTINUE.pos);
    	
    	if(!ctx->isInLoop)
    	{
    		throw CompilerException(GGE1303_InvalidContinue, location);
    	}
    
    	$returnCodeBlock = new CodeBlock();  
    	$returnCodeBlock->AddInstruction(new instructions::ContinueInstruction(location, ctx->codeBlockLevel - $BlockScope::continueCodeBlockLevel + 1));
    }
    | variableDeclaration { $returnCodeBlock = $variableDeclaration.returnCodeBlock; }
    | globalVariableDeclaration { $returnCodeBlock = $globalVariableDeclaration.returnCodeBlock; }
    | expression 
    { 
    	CodeLocation location(0, 0);
    
    	$returnCodeBlock = new CodeBlock(); 
    	$returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); delete $expression.returnCodeBlock; 
    	$returnCodeBlock->AddInstruction(new instructions::PopInstruction(location));
    }
    | yieldStatement{ $returnCodeBlock = $yieldStatement.returnCodeBlock; }
    | returnStatement { $returnCodeBlock = $returnStatement.returnCodeBlock; }
    | whileStatement { $returnCodeBlock = $whileStatement.returnCodeBlock; }
    | forStatement { $returnCodeBlock = $forStatement.returnCodeBlock; }
    | ifStatement { $returnCodeBlock = $ifStatement.returnCodeBlock;}
    | switchStatement { $returnCodeBlock = new CodeBlock(); }
    | blockStatement { $returnCodeBlock = $blockStatement.returnCodeBlock; };
    
blockStatement returns [CodeBlock* returnCodeBlock]
@init 
{ 
	$returnCodeBlock = new CodeBlock(); 
	ctx->codeBlockLevel++; 
}
@after { ctx->codeBlockLevel--; }: 
block
{
	CodeLocation location(0, 0);

	instructions::CallBlockInstruction* instr = new instructions::CallBlockInstruction(location);
	
	instr->GetCodeBlock().MoveInstructionsFrom(*$block.returnCodeBlock);
	delete $block.returnCodeBlock;
	
	$returnCodeBlock->AddInstruction(instr);
};
    
variableDeclaration returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); bool hadValue = false; } 
: ^(VAR IDENTIFIER (expression 
	{
		hadValue = true;
	
    		CodeLocation location($VAR.line, $VAR.pos);
    		
		$returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); 
		delete $expression.returnCodeBlock; 
		
		$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, "=", 1));
		$returnCodeBlock->AddInstruction(new instructions::DeclareLocalValueInstruction(location, (char*)$IDENTIFIER.text->chars));
		$returnCodeBlock->AddInstruction(new instructions::StoreScopeValueInstruction(location, (char*)$IDENTIFIER.text->chars));
		$returnCodeBlock->AddInstruction(new instructions::PopInstruction(location));
	})?)
{
    	CodeLocation location($VAR.line, $VAR.pos);
	if(!hadValue){
		$returnCodeBlock->AddInstruction(new instructions::DeclareLocalValueInstruction(location, (char*)$IDENTIFIER.text->chars));
	}
};

globalVariableDeclaration returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); bool hadValue = false; } 
: ^(GLOBAL IDENTIFIER (expression 
	{
		hadValue = true;
	
    		CodeLocation location($GLOBAL.line, $GLOBAL.pos);
    		
		$returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); 
		delete $expression.returnCodeBlock; 
		
		$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, "=", 1));
		$returnCodeBlock->AddInstruction(new instructions::DeclareGlobalValueInstruction(location, (char*)$IDENTIFIER.text->chars));
		$returnCodeBlock->AddInstruction(new instructions::StoreGlobalValueInstruction(location, (char*)$IDENTIFIER.text->chars));
		$returnCodeBlock->AddInstruction(new instructions::PopInstruction(location));
	})?)
{
    	CodeLocation location($GLOBAL.line, $GLOBAL.pos);
	if(!hadValue){
		$returnCodeBlock->AddInstruction(new instructions::DeclareGlobalValueInstruction(location, (char*)$IDENTIFIER.text->chars));
	}
};

yieldStatement returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
: ^(YIELD expression STRING?) 
{ 
	CodeLocation location($YIELD.line, $YIELD.pos);
	
	if($STRING == NULL){
		$returnCodeBlock->AddInstruction(new instructions::YieldAsMainInstruction(location));
	}
	else 
	{
		$returnCodeBlock->AddInstruction(new instructions::YieldAsNamedInstruction(location, (char*)$STRING.text->chars));	
	}
};

returnStatement returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = NULL; } 
: ^(RETURN (expression { $returnCodeBlock = new CodeBlock(); $returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); delete $expression.returnCodeBlock; } )?)
{
	CodeLocation location($RETURN.line, $RETURN.pos);
	
	if($returnCodeBlock == NULL)
	{
		$returnCodeBlock = new CodeBlock();	
		$returnCodeBlock->AddInstruction(new instructions::LoadNullInstruction(location));
	}

	if(!ctx->isInFunction)
	{		
		throw CompilerException(GGE1304_InvalidReturn, location);
	}

	$returnCodeBlock->AddInstruction(new instructions::BreakInstruction(location, ctx->codeBlockLevel + 1));
};

whileStatement returns [CodeBlock* returnCodeBlock]
scope BlockScope;
@init { 
	$returnCodeBlock = new CodeBlock(); 
	ctx->codeBlockLevel++; 
	$BlockScope::breakCodeBlockLevel = ctx->codeBlockLevel; 
	$BlockScope::continueCodeBlockLevel = ctx->codeBlockLevel; 
	ctx->isInLoop = true; 
}
@after { ctx->codeBlockLevel--; ctx->isInLoop = false;}
: ^(WHILE expression statement) 
{
	CodeLocation location($WHILE.line, $WHILE.pos);

	instructions::WhileInstruction* whileInstr = new instructions::WhileInstruction(location);
	CodeBlock& whileCodeBlock = whileInstr->GetCodeBlock();
	
	whileCodeBlock.MoveInstructionsFrom(*$expression.returnCodeBlock);
	delete $expression.returnCodeBlock;
	
	instructions::IfInstruction* ifInstr = new instructions::IfInstruction(location);
	ifInstr->GetElseBranchCodeBlock().AddInstruction(new instructions::BreakInstruction(location, 2));
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
	CodeLocation location($FOR.line, $FOR.pos);

	if(initExpressionCodeBlock != NULL)
	{
		$returnCodeBlock->MoveInstructionsFrom(*initExpressionCodeBlock);
		delete initExpressionCodeBlock;
	}
	
	instructions::WhileInstruction* whileInstr = new instructions::WhileInstruction(location);
	CodeBlock& whileCodeBlock = whileInstr->GetCodeBlock();
	
	if(conditionExpressionCodeBlock != NULL)
	{
		whileCodeBlock.MoveInstructionsFrom(*conditionExpressionCodeBlock);
		delete conditionExpressionCodeBlock;
		
		instructions::IfInstruction* ifInstr = new instructions::IfInstruction(location);
		ifInstr->GetElseBranchCodeBlock().AddInstruction(new instructions::BreakInstruction(location, 2));
		whileCodeBlock.AddInstruction(ifInstr);
	}

	whileCodeBlock.MoveInstructionsFrom(*$statement.returnCodeBlock);
	delete $statement.returnCodeBlock;
	
	if(incrementExpressionCodeBlock != NULL)
	{
		whileCodeBlock.MoveInstructionsFrom(*incrementExpressionCodeBlock);
		delete incrementExpressionCodeBlock;
		
		whileCodeBlock.AddInstruction(new instructions::PopInstruction(location));
	}
	
	$returnCodeBlock->AddInstruction(whileInstr);
};

initExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = NULL; } 
:
    variableDeclaration { $returnCodeBlock = $variableDeclaration.returnCodeBlock; }
    | expression { $returnCodeBlock = $expression.returnCodeBlock; };

ifStatement returns [CodeBlock* returnCodeBlock]
@init 
{ 
	$returnCodeBlock = new CodeBlock(); 
	ctx->codeBlockLevel++; 
}
@after { ctx->codeBlockLevel--; }: 
^(IF expression ifBranchStatement=statement elseBranchStatement=statement) 
{
	CodeLocation location($IF.line, $IF.pos);

	$returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock);

	instructions::IfInstruction* ifInstr = new instructions::IfInstruction(location);
	
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
	^(OPERATOR_ASSIGN lvalueExpression rvalueExpression=expression)
	{ 
		CodeLocation location($OPERATOR_ASSIGN.line, $OPERATOR_ASSIGN.pos);
	
		$returnCodeBlock->MoveInstructionsFrom(*$rvalueExpression.returnCodeBlock); 
		delete $rvalueExpression.returnCodeBlock; 
		
		$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, "=", 1));
		$returnCodeBlock->MoveInstructionsFrom(*$lvalueExpression.returnCodeBlock); 
		delete $lvalueExpression.returnCodeBlock; 
	}
	| ^(op=OPERATOR_ASSIGN_PLUS e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='-=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='*=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='/=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='%=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='<<=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='>>=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='&=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='|=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='is' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| conditionalOperatorExpression { $returnCodeBlock->MoveInstructionsFrom(*$conditionalOperatorExpression.returnCodeBlock); delete $conditionalOperatorExpression.returnCodeBlock;} 
	| ^(op='||' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='&&' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='^' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='&' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='|' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='==' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='!=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='<' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='<=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='>' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='>=' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='<<' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='>>' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='+' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='-' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='*' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='/' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op='%' e1=expression e2=expression) { binaryOperator(ctx, $op, $e1.returnCodeBlock, $e2.returnCodeBlock, $returnCodeBlock);}
	| ^(op=OPERATOR_INCREMENT_PRE e1=expression) { unaryOperator(ctx, $op, "++pre", $e1.returnCodeBlock, $returnCodeBlock);}	
	| ^(op=OPERATOR_INCREMENT_POST e1=expression) { unaryOperator(ctx, $op, "++post", $e1.returnCodeBlock, $returnCodeBlock);}		
	| ^(op=OPERATOR_DECREMENT_PRE e1=expression) { unaryOperator(ctx, $op, "--pre", $e1.returnCodeBlock, $returnCodeBlock);}	
	| ^(op=OPERATOR_DECREMENT_POST e1=expression) { unaryOperator(ctx, $op, "--post", $e1.returnCodeBlock, $returnCodeBlock);}				
	| ^(op=OPERATOR_PLUS_UN e1=expression) { unaryOperator(ctx, $op, "+un", $e1.returnCodeBlock, $returnCodeBlock);}				
	| ^(op=OPERATOR_MINUS_UN e1=expression) { unaryOperator(ctx, $op, "-un", $e1.returnCodeBlock, $returnCodeBlock);}					
	| ^(op=OPERATOR_NOT e1=expression) { unaryOperator(ctx, $op, "!", $e1.returnCodeBlock, $returnCodeBlock);}						
	| ^(op=OPERATOR_RELATIVE e1=expression) { unaryOperator(ctx, $op, "@", $e1.returnCodeBlock, $returnCodeBlock);}							
	| ^(OPERATOR_DOT e1=expression IDENTIFIER) 
	{ 
		CodeLocation location($OPERATOR_DOT.line, $OPERATOR_DOT.pos);
	
		$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); 
		delete $e1.returnCodeBlock; 
		
		$returnCodeBlock->AddInstruction(new instructions::LoadMemberValueInstruction(location, (char*)$IDENTIFIER.text->chars));
	}
	| callExpression { $returnCodeBlock->MoveInstructionsFrom(*$callExpression.returnCodeBlock); delete $callExpression.returnCodeBlock;} 
	| indexAccessExpression { $returnCodeBlock->MoveInstructionsFrom(*$indexAccessExpression.returnCodeBlock); delete $indexAccessExpression.returnCodeBlock;} 
	| IDENTIFIER 
	{ 
		CodeLocation location($IDENTIFIER.line, $IDENTIFIER.pos);
		$returnCodeBlock->AddInstruction(new instructions::LoadScopeValueInstruction(location, (char*)$IDENTIFIER.text->chars)); 
	}
	//collectionLiteral |
	| TRUE_LIT
	{ 
		CodeLocation location($TRUE_LIT.line, $TRUE_LIT.pos);
		$returnCodeBlock->AddInstruction(new instructions::LoadConstBooleanInstruction(location, true));	
	}
	| FALSE_LIT 
	{ 
		CodeLocation location($FALSE_LIT.line, $FALSE_LIT.pos);
		$returnCodeBlock->AddInstruction(new instructions::LoadConstBooleanInstruction(location, false));
	}
	| NUMBER 
	{ 
		CodeLocation location($NUMBER.line, $NUMBER.pos);	
		$returnCodeBlock->AddInstruction(new instructions::LoadConstNumberInstruction(location, StringToNumber((char*)$NUMBER.text->chars)));
	}
	| STRING 
	{ 
		CodeLocation location($STRING.line, $STRING.pos);	
		$returnCodeBlock->AddInstruction(new instructions::LoadConstStringInstruction(location, (char*)$STRING.text->chars));
	}
	| coordinateExpression { $returnCodeBlock->MoveInstructionsFrom(*$coordinateExpression.returnCodeBlock); delete $coordinateExpression.returnCodeBlock;} 
	;
	
coordinateExpression returns[CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); std::vector<CodeBlock*> argumentCodeBlocks; } 
:
^(COORDINATE (expression { argumentCodeBlocks.push_back($expression.returnCodeBlock); } )*) 
{
	CodeLocation location($COORDINATE.line, $COORDINATE.pos);
		// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}
	
	if(argumentCodeBlocks.size() < 1 || argumentCodeBlocks.size() > 2)
	{
		throw InternalErrorException("Incorrect number of arguments in coordinate literal.");
	}
	
	if(argumentCodeBlocks.size() == 1){
		$returnCodeBlock->AddInstruction(new instructions::LoadScopeValueInstruction(location, "Coordinate"));	
	}
	else if(argumentCodeBlocks.size() == 2){
		$returnCodeBlock->AddInstruction(new instructions::LoadScopeValueInstruction(location, "Point"));	
	}
	
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, "Create", argumentCodeBlocks.size()));	
};

callExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); std::vector<CodeBlock*> argumentCodeBlocks; } 
:
^('(' IDENTIFIER (expression { argumentCodeBlocks.push_back($expression.returnCodeBlock); } )*) 
{
	CodeLocation location($IDENTIFIER.line, $IDENTIFIER.pos);

	// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}

	$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, (char*)$IDENTIFIER.text->chars, argumentCodeBlocks.size()));
}
|	
^('(' ^('.' e1=expression IDENTIFIER) (e2=expression { argumentCodeBlocks.push_back($e2.returnCodeBlock); } )*) 
{
	CodeLocation location($IDENTIFIER.line, $IDENTIFIER.pos);

	// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}

	$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; 
	
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, (char*)$IDENTIFIER.text->chars, argumentCodeBlocks.size()));
}
;

indexAccessExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); std::vector<CodeBlock*> argumentCodeBlocks; } 
:
^(LEFT_SQUARE_BRACKET e1=expression (e2=expression { argumentCodeBlocks.push_back($e2.returnCodeBlock); } )*) 
{
	CodeLocation location($LEFT_SQUARE_BRACKET.line, $LEFT_SQUARE_BRACKET.pos);

	// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}

	$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; 
	
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, "[]", argumentCodeBlocks.size()));
};

conditionalOperatorExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
:
^(OPERATOR_EXPR_IF conditionExpression=expression ifExpression=expression elseExpression=expression)
{
	CodeLocation location($OPERATOR_EXPR_IF.line, $OPERATOR_EXPR_IF.pos);

	$returnCodeBlock->MoveInstructionsFrom(*$conditionExpression.returnCodeBlock);
	delete $conditionExpression.returnCodeBlock;

	instructions::IfInstruction* ifInstruction = new instructions::IfInstruction(location);
	
	ifInstruction->GetIfBranchCodeBlock().MoveInstructionsFrom(*$ifExpression.returnCodeBlock);
	delete $ifExpression.returnCodeBlock;

	ifInstruction->GetElseBranchCodeBlock().MoveInstructionsFrom(*$elseExpression.returnCodeBlock);
	delete $elseExpression.returnCodeBlock;
	
	$returnCodeBlock->AddInstruction(ifInstruction);
};


lvalueExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
:
	IDENTIFIER 
	{ 
		CodeLocation location($IDENTIFIER.line, $IDENTIFIER.pos);
		$returnCodeBlock->AddInstruction(new instructions::StoreScopeValueInstruction(location, (char*)$IDENTIFIER.text->chars));  
	}
	| ^(OPERATOR_DOT e1=expression IDENTIFIER) 
	{
		CodeLocation location($OPERATOR_DOT.line, $OPERATOR_DOT.pos);
	
		$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); 
		delete $e1.returnCodeBlock; 
		
		$returnCodeBlock->AddInstruction(new instructions::StoreMemberValueInstruction(location, (char*)$IDENTIFIER.text->chars));
	}
	| lvalueIndexAccessExpression 
	{ 
		$returnCodeBlock->MoveInstructionsFrom(*$lvalueIndexAccessExpression.returnCodeBlock); 
		delete $lvalueIndexAccessExpression.returnCodeBlock; 
	} ; 

lvalueIndexAccessExpression returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); std::vector<CodeBlock*> argumentCodeBlocks; } 
: 
^(LEFT_SQUARE_BRACKET e1=expression (e2=expression { argumentCodeBlocks.push_back($e2.returnCodeBlock); } )*) 
{
	CodeLocation location($LEFT_SQUARE_BRACKET.line, $LEFT_SQUARE_BRACKET.pos);

	// The arguments are stored on the stack in reverse order.
	for(int i = argumentCodeBlocks.size() - 1; i >= 0; i--)
	{
		$returnCodeBlock->MoveInstructionsFrom(*argumentCodeBlocks[i]); delete argumentCodeBlocks[i]; 
	}

	$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock; 
	
	// +1 for the value being written into the array
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, "[]=", argumentCodeBlocks.size() + 1));
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

label:        
	^(IDENTCHAIN IDENTIFIER+)
	| 'true'
        | 'false'
	| NUMBER
	| STRING;
	
stringLiteral returns [std::string value]:
	STRING
{
	std::string str ((char*)$STRING.text->chars);
	$value = str.substr(1, str.length() - 2);
};