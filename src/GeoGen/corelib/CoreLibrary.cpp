#include "CoreLibrary.hpp"
#include "NumberTypeDefinition.hpp"
#include "BooleanTypeDefinition.hpp"
#include "NullTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "DirectionTypeDefinition.hpp"
#include "../runtime/ReferenceTypeDefinition.hpp"
#include "BinaryArithmeticOperatorFunctionDefinition.hpp"
#include "ArithmeticAssignmentOperatorFunctionDefinition.hpp"
#include "BitLogicAssignmentOperatorFunctionDefinition.hpp"
#include "AssignmentOperatorFunctionDefinition.hpp"
#include "RelationalOperatorFunctionDefinition.hpp"
#include "EqualityOperatorFunctionDefinition.hpp"
#include "IncrementDecrementOperatorFunctionDefinition.hpp"
#include "BitLogicOperatorFunctionDefinition.hpp"
#include "LogicalOperatorFunctionDefinition.hpp"
#include "NegationOperatorFunctionDefinition.hpp"
#include "UnaryArithmeticOperatorFunctionDefinition.hpp"
#include "RelativeCoordinateOperatorFunctionDefinition.hpp"
#include "StringTypeDefinition.hpp"
#include "MathMinMaxFunctionDefinition.hpp"
#include "MathTrigonometricFunctionDefinition.hpp"
#include "MathAtan2FunctionDefinition.hpp"
#include "MathLogPowFunctionDefinition.hpp"
#include "MathSqrtFunctionDefinition.hpp"
#include "MathRoundFunctionDefinition.hpp"
#include "MessageFunctionDefinition.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "RandomFunctionDefinition.hpp"
#include "RandomSequence2DTypeDefinition.hpp"

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
	this->typeDefinitions.AddItem(new CoordinateTypeDefinition());
	this->typeDefinitions.AddItem(new PointTypeDefinition());
	this->typeDefinitions.AddItem(new ReferenceTypeDefinition());

	// Enums
	this->typeDefinitions.AddItem(DirectionTypeDefinition::Create());

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
	this->globalFunctionDefinitions.AddItem(new RelativeCoordinateOperatorFunctionDefinition());
	this->globalFunctionDefinitions.AddItem(IncrementDecrementOperatorFunctionDefinition::Create(IncrementDecrementOperatorFunctionDefinition::PLUS_PLUS_POST));
	this->globalFunctionDefinitions.AddItem(IncrementDecrementOperatorFunctionDefinition::Create(IncrementDecrementOperatorFunctionDefinition::PLUS_PLUS_PRE));
	this->globalFunctionDefinitions.AddItem(IncrementDecrementOperatorFunctionDefinition::Create(IncrementDecrementOperatorFunctionDefinition::MINUS_MINUS_POST));
	this->globalFunctionDefinitions.AddItem(IncrementDecrementOperatorFunctionDefinition::Create(IncrementDecrementOperatorFunctionDefinition::MINUS_MINUS_PRE));
	this->globalFunctionDefinitions.AddItem(ArithmeticAssignmentOperatorFunctionDefinition::Create(ArithmeticAssignmentOperatorFunctionDefinition::MULTIPLICATION));
	this->globalFunctionDefinitions.AddItem(ArithmeticAssignmentOperatorFunctionDefinition::Create(ArithmeticAssignmentOperatorFunctionDefinition::DIVISION));
	this->globalFunctionDefinitions.AddItem(ArithmeticAssignmentOperatorFunctionDefinition::Create(ArithmeticAssignmentOperatorFunctionDefinition::MODULO));
	this->globalFunctionDefinitions.AddItem(ArithmeticAssignmentOperatorFunctionDefinition::Create(ArithmeticAssignmentOperatorFunctionDefinition::ADDITION));
	this->globalFunctionDefinitions.AddItem(ArithmeticAssignmentOperatorFunctionDefinition::Create(ArithmeticAssignmentOperatorFunctionDefinition::SUBTRACTION));
	this->globalFunctionDefinitions.AddItem(ArithmeticAssignmentOperatorFunctionDefinition::Create(ArithmeticAssignmentOperatorFunctionDefinition::BIT_SHIFT_LEFT));
	this->globalFunctionDefinitions.AddItem(ArithmeticAssignmentOperatorFunctionDefinition::Create(ArithmeticAssignmentOperatorFunctionDefinition::BIT_SHIFT_RIGHT));
	this->globalFunctionDefinitions.AddItem(BitLogicAssignmentOperatorFunctionDefinition::Create(BitLogicAssignmentOperatorFunctionDefinition::AND));
	this->globalFunctionDefinitions.AddItem(BitLogicAssignmentOperatorFunctionDefinition::Create(BitLogicAssignmentOperatorFunctionDefinition::OR));


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
	this->globalFunctionDefinitions.AddItem(new MathAtan2FunctionDefinition());
	this->globalFunctionDefinitions.AddItem(MathLogPowFunctionDefinition::Create(MathLogPowFunctionDefinition::LOG));
	this->globalFunctionDefinitions.AddItem(MathLogPowFunctionDefinition::Create(MathLogPowFunctionDefinition::POW));
	this->globalFunctionDefinitions.AddItem(new MathSqrtFunctionDefinition());
	this->globalFunctionDefinitions.AddItem(MathRoundFunctionDefinition::Create(MathRoundFunctionDefinition::ROUND));
	this->globalFunctionDefinitions.AddItem(MathRoundFunctionDefinition::Create(MathRoundFunctionDefinition::CEIL));
	this->globalFunctionDefinitions.AddItem(MathRoundFunctionDefinition::Create(MathRoundFunctionDefinition::FLOOR));
	this->globalFunctionDefinitions.AddItem(MathRoundFunctionDefinition::Create(MathRoundFunctionDefinition::ABS));

	// Randomness
	this->globalFunctionDefinitions.AddItem(new RandomFunctionDefinition());
	this->typeDefinitions.AddItem(new RandomSequence2DTypeDefinition());

	// Text reporting
	this->globalFunctionDefinitions.AddItem(MessageFunctionDefinition::Create(MessageFunctionDefinition::PRINT));
	this->globalFunctionDefinitions.AddItem(MessageFunctionDefinition::Create(MessageFunctionDefinition::TRIGGER_ERROR));

	// Generator types
	this->typeDefinitions.AddItem(new HeightProfileTypeDefinition());
	this->typeDefinitions.AddItem(new HeightMapTypeDefinition());
}