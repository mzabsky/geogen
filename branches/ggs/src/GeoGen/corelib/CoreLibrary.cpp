#include "CoreLibrary.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "../corelib/NullTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "BinaryArithmeticOperatorFunctionDefinition.hpp"
#include "AssignmentOperatorFunctionDefinition.hpp"
#include "RelationalOperatorFunctionDefinition.hpp"
#include "EqualityOperatorFunctionDefinition.hpp"
#include "BitLogicOperatorFunctionDefinition.hpp"
#include "LogicalOperatorFunctionDefinition.hpp"
#include "NegationOperatorFunctionDefinition.hpp"
#include "UnaryArithmeticOperatorFunctionDefinition.hpp"
#include "StringTypeDefinition.hpp"
#include "MathMinMaxFunctionDefinition.hpp"
#include "MathTrigonometricFunctionDefinition.hpp"

using namespace geogen::corelib;
using namespace geogen::runtime;

CoreLibrary::CoreLibrary()
{
	// Core types
	this->typeDefinitions.AddItem(new NullTypeDefinition());
	this->typeDefinitions.AddItem(new BooleanTypeDefinition());
	this->typeDefinitions.AddItem(new NumberTypeDefinition());
	this->typeDefinitions.AddItem(new StringTypeDefinition());
	this->typeDefinitions.AddItem(new ArrayTypeDefinition());

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

	// Math functions
	this->globalFunctionDefinitions.AddItem(MathMinMaxFunctionDefinition::Create(MathMinMaxFunctionDefinition::MIN));
	this->globalFunctionDefinitions.AddItem(MathMinMaxFunctionDefinition::Create(MathMinMaxFunctionDefinition::MAX));
	this->globalFunctionDefinitions.AddItem(MathTrigonometricFunctionDefinition::Create(MathTrigonometricFunctionDefinition::SIN));
	this->globalFunctionDefinitions.AddItem(MathTrigonometricFunctionDefinition::Create(MathTrigonometricFunctionDefinition::COS));
	this->globalFunctionDefinitions.AddItem(MathTrigonometricFunctionDefinition::Create(MathTrigonometricFunctionDefinition::TAN));
	this->globalFunctionDefinitions.AddItem(MathTrigonometricFunctionDefinition::Create(MathTrigonometricFunctionDefinition::ASIN));
	this->globalFunctionDefinitions.AddItem(MathTrigonometricFunctionDefinition::Create(MathTrigonometricFunctionDefinition::ACOS));
	this->globalFunctionDefinitions.AddItem(MathTrigonometricFunctionDefinition::Create(MathTrigonometricFunctionDefinition::ATAN));
	this->globalFunctionDefinitions.AddItem(MathTrigonometricFunctionDefinition::Create(MathTrigonometricFunctionDefinition::DEG_TO_RAD));
	this->globalFunctionDefinitions.AddItem(MathTrigonometricFunctionDefinition::Create(MathTrigonometricFunctionDefinition::RAD_TO_DEG));
}