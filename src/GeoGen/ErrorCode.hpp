#pragma once

namespace geogen
{
	enum ErrorCode
	{
		GGE1101_UnexpectedCharacter = 1101,
		GGE1201_UnexpectedToken = 1201,
		GGE1301_InvalidBreak = 1301,
		GGE1302_MissingBreak = 1302,
		GGE1303_InvalidContinue = 1303,
		GGE1304_InvalidReturn = 1304,
		GGE1306_FunctionAlreadyDefined = 1306,
		GGE1307_InvalidLvalueExpression = 1307,
		GGE1308_TypeAlreadyDefined = 1308,
		GGE1309_EnumValueAlreadyDefined = 1309,
		GGE1310_EnumValueNotInteger = 1309,
		GGE1401_MetadataValueAlreadyDefined = 1401,
		GGE1501_ParametersNotKeyValueSection = 1501,
		GGE1502_IncorrectParameterDefinition = 1502,
		GGE2101_IncorrectNativeFunctionArgumentType = 2101,
		GGE2102_IncorrectOperandType = 2102,
		GGE2103_IncorrectNumberOfArguments = 2103,
		GGE2104_IncorrectConditionResultType = 2104,
		GGE2105_IncorrectCoordinateComponentType = 2105,
		GGE2106_NullReference = 2106,
		GGE2107_IncorrectYieldType = 2107,
		GGE2108_ReadOnlyWrite = 2108,
		GGE2201_UndefinedFunction = 2201,
		GGE2202_UndefinedVariable = 2202,
		GGE2203_UndefinedMemberVariable = 2203,
		GGE2204_VariableAlreadyDefined = 2204,
		GGE2301_DivisionByZero = 2301,
		GGE2302_NumberOverflow = 2302,
		GGE2303_HeightOverflow = 2303,
		GGE2304_CoordinateOverflow = 2304,
		GGE2501_CallStackOverflow = 2501,
		GGE2502_CodeBlockStackOverflow = 2502,
		GGE2503_ObjectStackOverflow = 2503,
		GGE3000_ApiUsageError = 3000,
		GGE4000_InternalError = 4000,
		GGE4001_IntermediateCodeError = 4001,
	};
}