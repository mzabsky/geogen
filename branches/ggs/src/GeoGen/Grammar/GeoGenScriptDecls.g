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
	using namespace geogen::corelib;
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
	std::vector<String> lines;
}


@apifuncs {
	RECOGNIZER->displayRecognitionError = handleTreeWalkerError;
}

@members {
       	void handleTreeWalkerError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames) { 
       	    pANTLR3_EXCEPTION ex = recognizer->state->exception;
       	    pANTLR3_BASE_TREE currentTree = (pANTLR3_BASE_TREE)(recognizer->state->exception->token);
       	    
       	    CodeLocation location(recognizer->state->exception->line, currentTree->getCharPositionInLine(currentTree));
       	    
       	    String expectedTokenName = GG_STR("");
       	    if(ex->expecting == ANTLR3_TOKEN_EOF)
       	    {
				expectedTokenName = GG_STR("EOF");
       	    }
   			else if (ex->expecting > 0)
       	    {
       	    	expectedTokenName = (Char*)tokenNames[ex->expecting];
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
		
		returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, (Char*)operatorToken->getText(operatorToken)->chars, 2));
	}
	
	void unaryOperator(pGeoGenScriptDecls ctx, pANTLR3_BASE_TREE operatorToken, string const& text, CodeBlock* e1, CodeBlock* returnCodeBlock)
	{
		CodeLocation location(operatorToken->getLine(operatorToken), operatorToken->getCharPositionInLine(operatorToken));
	
		returnCodeBlock->MoveInstructionsFrom(*e1); 
		delete e1; 		
		
		returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, AnyStringToString(text), 1));
	}    	     	 
}


script: ^(SCRIPT metadata? ^(DECLARATIONS declaration*) block) 
{ 
	ctx->rootCodeBlock->MoveInstructionsFrom(*$block.returnCodeBlock); 
	ctx->rootCodeBlock->AddInstruction(new instructions::LoadNullInstruction(CodeLocation(0, 0)));
	delete $block.returnCodeBlock; 
};
        
metadata: ^('metadata' metadataKeyValueCollection) 
{ 
	ctx->compiledScript->GetMetadata().MoveKeyValuesFrom(*dynamic_cast<MetadataKeyValueCollection*>($metadataKeyValueCollection.value));
	delete $metadataKeyValueCollection.value;
};

metadataKeyValueCollection returns [MetadataValue* value]
@init {
	MetadataKeyValueCollection* ret = NULL;
	$value = NULL;
} : ^(COLLECTION 
	{
		ret = new MetadataKeyValueCollection(CodeLocation($COLLECTION.line, $COLLECTION.pos));
		$value = ret;	
	} (metadataKeyValuePair 
	{ 
		if(!ret->AddItem($metadataKeyValuePair.name, $metadataKeyValuePair.value))
		{
			CodeLocation location($COLLECTION.line, $COLLECTION.pos);
			throw MetadataKeyRedefinitionException(location, $metadataKeyValuePair.name);
		}
		/*delete $metadataKeyValuePair.name; */
	})*);
	
metadataList returns [MetadataValue* value]
@init {
	MetadataList* ret = NULL;
	$value = NULL;
} : ^(SIMPLE_COLLECTION 
	{
		ret = new MetadataList(CodeLocation($SIMPLE_COLLECTION.line, $SIMPLE_COLLECTION.pos));
		$value = ret;	
	} (metadataKeyValueValue 
	{ 
		ret->AddItem($metadataKeyValueValue.value);
	})*);
	
metadataKeyValuePair returns [String name, MetadataValue* value] @init{ $value = NULL; }: 
	^(IDENTIFIER metadataKeyValueValue) { $name = (Char*)$IDENTIFIER.text->chars; $value = $metadataKeyValueValue.value; }
	| ^(NUMBER metadataKeyValueValue '@'?)  { $name = (Char*)$NUMBER.text->chars; $value = $metadataKeyValueValue.value; };

metadataKeyValueValue returns [MetadataValue* value] @init{ $value = NULL; }: 
	stringLiteral  { $value = new MetadataString(CodeLocation($stringLiteral.line, $stringLiteral.pos), $stringLiteral.value); }
	| TRUE_LIT  { $value = new MetadataBoolean(CodeLocation($TRUE_LIT.line, $TRUE_LIT.pos), true); }
	| FALSE_LIT  { $value = new MetadataBoolean(CodeLocation($FALSE_LIT.line, $FALSE_LIT.pos), false); }
	| NUMBER { $value = new MetadataNumber(CodeLocation($NUMBER.line, $NUMBER.pos), StringToNumber((Char*)$NUMBER.text->chars)); }
	| IDENTIFIER { $value = new MetadataIdentifier(CodeLocation($IDENTIFIER.line, $IDENTIFIER.pos), (Char*)$IDENTIFIER.text->chars); }
	| metadataKeyValueCollection { $value = $metadataKeyValueCollection.value; }
	| metadataList { $value = $metadataList.value; };

//keyValueCollection: ^(COLLECTION keyValuePair*);

/*keyValuePair: 
	^(IDENTIFIER keyValueValue)
	| ^(NUMBER keyValueValue '@'?);*/

//keyValueValue: expression | keyValueCollection;

declaration: enumDeclaration | functionDeclaration;

enumDeclaration: ^(ENUM IDENTIFIER enumValues)
{
	CodeLocation location($ENUM.line, $ENUM.pos);
		
	EnumTypeDefinition* decl = new EnumTypeDefinition((Char*)$IDENTIFIER.text->chars, $enumValues.returnEnumValues);
	
	if (!ctx->compiledScript->AddTypeDefinition(decl)){
		throw SymbolRedefinitionException(GGE1308_TypeAlreadyDefined, location, decl->GetName());
	}
};

enumValues returns [map<String, int> returnEnumValues]
@init { map<String, int> tempEnumValues; double number = 0; int nextAutoNumber = 0; bool isNumberDefined = false;}
: ^(VALUES (^(IDENTIFIER (NUMBER { number = StringToNumber((Char*)$NUMBER.text->chars); isNumberDefined = true;} )?)
	{ 
		String valueName = (Char*)$IDENTIFIER.text->chars;
	
		CodeLocation enumValueLocation($IDENTIFIER.line, $IDENTIFIER.pos);
		
		if(isNumberDefined && !IsNumberInt(number))
		{
			throw InvalidSymbolDefinitionException(GGE1310_EnumValueNotInteger, enumValueLocation, valueName);
		}
		
		if(!isNumberDefined){
			number = nextAutoNumber;
		}
		else {
			number = NumberToInt(number);
		}
		
		nextAutoNumber = number + 1;
		
		if(!returnEnumValues.insert(std::pair<String, int>(valueName, number)).second)
		{		
			throw SymbolRedefinitionException(GGE1309_EnumValueAlreadyDefined, enumValueLocation, valueName);
		}
		
		isNumberDefined = false;
		number = 0;
	})+);

//enumValue: ;

functionDeclaration
//scope BlockScope;
//scope { SymbolDefinitionTable<VariableDefinition>* localVariableDefinitions }
@init { ctx->isInFunction = true; /*functionDeclaration::localVariableDefinitions = new SymbolDefinitionTable<VariableDefinition>();*/}
@after { ctx->isInFunction = false; /*functionDeclaration::localVariableDefinitions = NULL;*/ }
: ^(FUNCTION name=IDENTIFIER ^(PARAMETERS formalParameters+=IDENTIFIER*) block)
{
	CodeLocation location($FUNCTION.line, $FUNCTION.pos);
	ScriptFunctionDefinition* decl = new ScriptFunctionDefinition((Char*)$name.text->chars, location, $formalParameters != NULL ? $formalParameters->count : 0);

	//SymbolDefinitionTable<VariableDefinition>& varDecls = decl->GetLocalVariableDefinitions();	
	
	CodeBlock& codeBlock = decl->GetRootCodeBlock();
	if($formalParameters != NULL)
	{

	        
	        for(unsigned i = 0; i < $formalParameters->count; i++)
	        {
			pANTLR3_BASE_TREE tree = (pANTLR3_BASE_TREE)$formalParameters->elements[i].element;
			CodeLocation parameterLocation(tree->getLine(tree), tree->getCharPositionInLine(tree));
			//varDecls.AddItem(new ScriptVariableDefinition(String((char*)tree->getText(tree)->chars)));
		        codeBlock.AddInstruction(new instructions::DeclareLocalValueInstruction(location, (Char*)tree->getText(tree)->chars));	
		        codeBlock.AddInstruction(new instructions::StoreScopeValueInstruction(location, (Char*)tree->getText(tree)->chars));	
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
    	$returnCodeBlock = new CodeBlock(); 
    	$returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); delete $expression.returnCodeBlock; 
    	
    	$returnCodeBlock->AddInstruction(new instructions::PopInstruction((*($returnCodeBlock->End() - 1))->GetLocation()));
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
	if($block.returnCodeBlock->Begin() != $block.returnCodeBlock->End())
	{	
		instructions::CallBlockInstruction* instr = new instructions::CallBlockInstruction((*$block.returnCodeBlock->Begin())->GetLocation());
	
		instr->GetCodeBlock().MoveInstructionsFrom(*$block.returnCodeBlock);
		delete $block.returnCodeBlock;
		
		$returnCodeBlock->AddInstruction(instr);	
	}
	else {
		delete $block.returnCodeBlock;	
	}

};
    
variableDeclaration returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); bool hadValue = false; } 
: ^(VAR IDENTIFIER (expression 
	{
		hadValue = true;
	
    		CodeLocation location($VAR.line, $VAR.pos);
    		
		$returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); 
		delete $expression.returnCodeBlock; 
		
		/*$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, "=", 1));*/
		$returnCodeBlock->AddInstruction(new instructions::DeclareLocalValueInstruction(location, (Char*)$IDENTIFIER.text->chars));
		$returnCodeBlock->AddInstruction(new instructions::StoreScopeValueInstruction(location, (Char*)$IDENTIFIER.text->chars));
		$returnCodeBlock->AddInstruction(new instructions::PopInstruction(location));
	})?)
{
    	CodeLocation location($VAR.line, $VAR.pos);
	if(!hadValue){
		$returnCodeBlock->AddInstruction(new instructions::DeclareLocalValueInstruction(location, (Char*)$IDENTIFIER.text->chars));
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
		
		/*$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, "=", 1));*/
		$returnCodeBlock->AddInstruction(new instructions::DeclareGlobalValueInstruction(location, (Char*)$IDENTIFIER.text->chars));
		$returnCodeBlock->AddInstruction(new instructions::StoreGlobalValueInstruction(location, (Char*)$IDENTIFIER.text->chars));
		$returnCodeBlock->AddInstruction(new instructions::PopInstruction(location));
	})?)
{
    	CodeLocation location($GLOBAL.line, $GLOBAL.pos);
	if(!hadValue){
		$returnCodeBlock->AddInstruction(new instructions::DeclareGlobalValueInstruction(location, (Char*)$IDENTIFIER.text->chars));
	}
};

yieldStatement returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); } 
: ^(YIELD expression STRING?) 
{ 
	CodeLocation location($YIELD.line, $YIELD.pos);
	
	$returnCodeBlock->MoveInstructionsFrom(*$expression.returnCodeBlock); 
	delete $expression.returnCodeBlock; 
	
	if($STRING == NULL){
		$returnCodeBlock->AddInstruction(new instructions::YieldAsNamedInstruction(location, renderer::Renderer::MAP_NAME_MAIN));
	}
	else 
	{
		$returnCodeBlock->AddInstruction(new instructions::YieldAsNamedInstruction(location, (Char*)$STRING.text->chars));	
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
	
	std::auto_ptr<instructions::CallBlockInstruction> wrappingInstruction(new instructions::CallBlockInstruction(location));

	if(initExpressionCodeBlock != NULL)
	{
		wrappingInstruction->GetCodeBlock().MoveInstructionsFrom(*initExpressionCodeBlock);
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
	
	wrappingInstruction->GetCodeBlock().AddInstruction(whileInstr);
	
	$returnCodeBlock->AddInstruction(wrappingInstruction.release());
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
		
/*		$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, "=", 1));*/
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
		
		$returnCodeBlock->AddInstruction(new instructions::LoadMemberValueInstruction(location, (Char*)$IDENTIFIER.text->chars));
	}
	| callExpression { $returnCodeBlock->MoveInstructionsFrom(*$callExpression.returnCodeBlock); delete $callExpression.returnCodeBlock;} 
	| indexAccessExpression { $returnCodeBlock->MoveInstructionsFrom(*$indexAccessExpression.returnCodeBlock); delete $indexAccessExpression.returnCodeBlock;} 
	| IDENTIFIER 
	{ 
		CodeLocation location($IDENTIFIER.line, $IDENTIFIER.pos);
		$returnCodeBlock->AddInstruction(new instructions::LoadScopeValueInstruction(location, (Char*)$IDENTIFIER.text->chars)); 
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
		$returnCodeBlock->AddInstruction(new instructions::LoadConstNumberInstruction(location, StringToNumber((Char*)$NUMBER.text->chars)));
	}
	| stringLiteral 
	{ 
		CodeLocation location($stringLiteral.line, $stringLiteral.pos);	
		$returnCodeBlock->AddInstruction(new instructions::LoadConstStringInstruction(location, $stringLiteral.value));
	}
	| coordinateExpression { $returnCodeBlock->MoveInstructionsFrom(*$coordinateExpression.returnCodeBlock); delete $coordinateExpression.returnCodeBlock;} 
	| collectionLiteral { $returnCodeBlock->MoveInstructionsFrom(*$collectionLiteral.returnCodeBlock); delete $collectionLiteral.returnCodeBlock;} 
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
		throw InternalErrorException(GG_STR("Incorrect number of arguments in coordinate literal."));
	}
	
	if(argumentCodeBlocks.size() == 1){
		$returnCodeBlock->AddInstruction(new instructions::LoadScopeValueInstruction(location, GG_STR("Coordinate")));	
		$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, GG_STR("FromNumber"), argumentCodeBlocks.size()));	
	}
	else if(argumentCodeBlocks.size() == 2){
		$returnCodeBlock->AddInstruction(new instructions::LoadScopeValueInstruction(location, GG_STR("Point")));	
		$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, GG_STR("Create"), argumentCodeBlocks.size()));	
	}
	
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

	$returnCodeBlock->AddInstruction(new instructions::CallGlobalInstruction(location, (Char*)$IDENTIFIER.text->chars, argumentCodeBlocks.size()));
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
	
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, (Char*)$IDENTIFIER.text->chars, argumentCodeBlocks.size()));
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
	
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, GG_STR("[]"), argumentCodeBlocks.size()));
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
		$returnCodeBlock->AddInstruction(new instructions::StoreScopeValueInstruction(location, (Char*)$IDENTIFIER.text->chars));  
	}
	| ^(OPERATOR_DOT e1=expression IDENTIFIER) 
	{
		CodeLocation location($OPERATOR_DOT.line, $OPERATOR_DOT.pos);
	
		$returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); 
		delete $e1.returnCodeBlock; 
		
		$returnCodeBlock->AddInstruction(new instructions::StoreMemberValueInstruction(location, (Char*)$IDENTIFIER.text->chars));
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
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, GG_STR("[]="), argumentCodeBlocks.size() + 1));
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

collectionLiteral returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); unsigned itemCount = 0; } :
^(COLLECTION (collectionLiteralItem { returnCodeBlock->MoveInstructionsFrom(*$collectionLiteralItem.returnCodeBlock); delete $collectionLiteralItem.returnCodeBlock; itemCount++; } )*) 
{
	CodeLocation location($COLLECTION.line, $COLLECTION.pos);	
	$returnCodeBlock->AddInstruction(new instructions::LoadScopeValueInstruction(location, GG_STR("Array")));	
	$returnCodeBlock->AddInstruction(new instructions::CallMemberInstruction(location, GG_STR("<FromList>"), itemCount * 2));		
};


collectionLiteralItem returns [CodeBlock* returnCodeBlock]
@init { $returnCodeBlock = new CodeBlock(); bool hasSecond = false; } :
    ^(COLLECTION_ITEM e1=expression     	
    	(e2=expression { returnCodeBlock->MoveInstructionsFrom(*$e2.returnCodeBlock); delete $e2.returnCodeBlock; hasSecond = true; })?) 
    {
	CodeLocation location($COLLECTION_ITEM.line, $COLLECTION_ITEM.pos);
    
	returnCodeBlock->MoveInstructionsFrom(*$e1.returnCodeBlock); delete $e1.returnCodeBlock;
		
    
	if(!hasSecond)
	{
		returnCodeBlock->AddInstruction(new instructions::LoadNullInstruction(location));
	}

    };

label:        
	^(IDENTCHAIN IDENTIFIER+)
	| 'true'
        | 'false'
	| NUMBER
	| STRING;
	
stringLiteral returns [String value, int line, int pos]:
	STRING
{
	String str = (Char*)$STRING.text->chars;
	$value = str;//str.substr(1, str.length() - 2);
	$line = $STRING.line;
	$pos = $STRING.pos;
};