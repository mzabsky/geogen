#include "CoreLibrary.hpp"
#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/BooleanTypeDefinition.hpp"
#include "../runtime/NullTypeDefinition.hpp"
#include "BinaryArithmeticOperatorFunctionDefinition.hpp"
#include "AssignmentOperatorFunctionDefinition.hpp"
#include "RelationalOperatorFunctionDefinition.hpp"
#include "EqualityOperatorFunctionDefinition.hpp"
#include "BitLogicOperatorFunctionDefinition.hpp"
#include "LogicalOperatorFunctionDefinition.hpp"
#include "NegationOperatorFunctionDefinition.hpp"
#include "UnaryArithmeticOperatorFunctionDefinition.hpp"

using namespace geogen::corelib;
using namespace geogen::runtime;

CoreLibrary::CoreLibrary()
{
	// Core types
	this->typeDefinitions.AddItem(new NullTypeDefinition());
	this->typeDefinitions.AddItem(new BooleanTypeDefinition());
	this->typeDefinitions.AddItem(new NumberTypeDefinition());

    // Core operators
	this->globalFunctionDefinitions.AddItem(new AssignmentOperatorFunctionDefinition());
	this->globalFunctionDefinitions.AddItem(new NegationOperatorFunctionDefinition());
	this->globalFunctionDefinitions.AddItem(UnaryArithmeticOperatorFunctionDefinition::Create(UnaryArithmeticOperatorFunctionDefinition::UNARY_PLUS));
	this->globalFunctionDefinitions.AddItem(UnaryArithmeticOperatorFunctionDefinition::Create(UnaryArithmeticOperatorFunctionDefinition::UNARY_MINUS));
	this->globalFunctionDefinitions.AddItem(BinaryArithmeticOperatorFunctionDefinition::Create(BinaryArithmeticOperatorFunctionDefinition::MULTIPLICATION));
	this->globalFunctionDefinitions.AddItem(BinaryArithmeticOperatorFunctionDefinition::Create(BinaryArithmeticOperatorFunctionDefinition::DIVISION));
	this->globalFunctionDefinitions.AddItem(BinaryArithmeticOperatorFunctionDefinition::Create(BinaryArithmeticOperatorFunctionDefinition::MODULO));
	this->globalFunctionDefinitions.AddItem(BinaryArithmeticOperatorFunctionDefinition::Create(BinaryArithmeticOperatorFunctionDefinition::ADDITION));
	this->globalFunctionDefinitions.AddItem(BinaryArithmeticOperatorFunctionDefinition::Create(BinaryArithmeticOperatorFunctionDefinition::SUBTRACTION));
	this->globalFunctionDefinitions.AddItem(BinaryArithmeticOperatorFunctionDefinition::Create(BinaryArithmeticOperatorFunctionDefinition::BIT_SHIFT_LEFT));
	this->globalFunctionDefinitions.AddItem(BinaryArithmeticOperatorFunctionDefinition::Create(BinaryArithmeticOperatorFunctionDefinition::BIT_SHIFT_RIGHT));
	this->globalFunctionDefinitions.AddItem(RelationalOperatorFunctionDefinition::Create(RelationalOperatorFunctionDefinition::LESS_THAN));
	this->globalFunctionDefinitions.AddItem(RelationalOperatorFunctionDefinition::Create(RelationalOperatorFunctionDefinition::LESS_THAN_OR_EQUAL_TO));
	this->globalFunctionDefinitions.AddItem(RelationalOperatorFunctionDefinition::Create(RelationalOperatorFunctionDefinition::GREATER_THAN));
	this->globalFunctionDefinitions.AddItem(RelationalOperatorFunctionDefinition::Create(RelationalOperatorFunctionDefinition::GREATER_THAN_OR_EQUAL_TO));
	this->globalFunctionDefinitions.AddItem(EqualityOperatorFunctionDefinition::Create(EqualityOperatorFunctionDefinition::EQUAL_TO));
	this->globalFunctionDefinitions.AddItem(EqualityOperatorFunctionDefinition::Create(EqualityOperatorFunctionDefinition::NOT_EQUAL_TO));
	this->globalFunctionDefinitions.AddItem(BitLogicOperatorFunctionDefinition::Create(BitLogicOperatorFunctionDefinition::BIT_AND));
	this->globalFunctionDefinitions.AddItem(BitLogicOperatorFunctionDefinition::Create(BitLogicOperatorFunctionDefinition::BIT_OR));
	this->globalFunctionDefinitions.AddItem(BitLogicOperatorFunctionDefinition::Create(BitLogicOperatorFunctionDefinition::BIT_XOR));
	this->globalFunctionDefinitions.AddItem(LogicalOperatorFunctionDefinition::Create(LogicalOperatorFunctionDefinition::LOGICAL_AND));
	this->globalFunctionDefinitions.AddItem(LogicalOperatorFunctionDefinition::Create(LogicalOperatorFunctionDefinition::LOGICAL_OR));


	//this->typeDefinitions.AddItem(new StringTypeDefinition());
}