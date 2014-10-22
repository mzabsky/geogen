#pragma once

namespace geogen
{
	/// Error codes produced by GeoGen.
	enum ErrorCode
	{
		/// An unexpected character was encountered in the input. This usually means a severe syntax error.
		GGE1101_UnexpectedCharacter = 1101,
		/// An invalid escape sequence was encountered in the input.
		GGE1102_InvalidEscapeSequence = 1102,
		/// The parser hes encountered an unexpected token. 
		GGE1201_UnexpectedToken = 1201,
		GGE1301_InvalidBreak = 1301,
		//GGE1302_MissingBreak = 1302,
		GGE1303_InvalidContinue = 1303,
		GGE1304_InvalidReturn = 1304,
		GGE1306_FunctionAlreadyDefined = 1306,
		GGE1307_ReadOnlyExpressionWrite = 1307,
		GGE1308_TypeAlreadyDefined = 1308,
		GGE1309_EnumValueAlreadyDefined = 1309,
		GGE1310_EnumValueNotInteger = 1310,
		GGE1311_ReservedTypeNameUsed = 1311,
		GGE1312_MainMapNotSupportedByScript = 1312,
		GGE1401_MetadataKeyAlreadyDefined = 1401,
		GGE1402_ScriptParametersNotKeyValueCollection = 1402,
		GGE1403_ScriptParameterNotKeyValueCollection = 1403,
		//GGE1404_ScriptParameterTypeNotSpecified = 1404,
		GGE1405_ReservedScriptParameterNameUsed = 1405,
		GGE1406_IncorrectScriptParameterAttributeType = 1406,
		/// Unknown script parameter restriction was used.
		GGE1407_IncorrectScriptParameterValueRestriction = 1407,
		GGE1408_WidthNotKeyValueCollection = 1408,
		GGE1409_HeightNotKeyValueCollection = 1409,
		GGE1410_UndefinedMetadataIdentifier = 1410,
		GGE1411_IncorrectMapSizeNumericValue = 1411,
		GGE1412_MinGreaterThanMaxMapSize = 1412,
		GGE1413_IncorrectEnumScriptParameterType = 1413,
		GGE2101_IncorrectNativeFunctionArgumentType = 2101,
		GGE2102_IncorrectOperandType = 2102,
		GGE2103_IncorrectNumberOfArguments = 2103,
		GGE2104_IncorrectConditionResultType = 2104,
		//GGE2105_IncorrectCoordinateComponentType = 2105,
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
		GGE2305_OutsideTrigonometricFunctionDefinitionRange = 2305,
		GGE2306_OutsideLogFunctionDefinitionRange = 2306,
		GGE2307_OutsideSqrtFunctionDefinitionRange = 2307,
		GGE2308_NumberUnderflow = 2308,
		GGE2309_SizeOverflow = 2309,
		GGE2501_CallStackOverflow = 2501,
		GGE2502_CodeBlockStackOverflow = 2502,
		GGE2503_ObjectStackOverflow = 2503,
		GGE2504_RenderingSequenceTooLong = 2504,
		GGE2601_InvalidOperationOnEmptyArray = 2601,
		GGE2602_ArrayKeyNotFound = 2602,
		GGE2603_ArrayNullKey = 2603,
		GGE2604_ArrayIndexNotFound = 2604,
		GGE2701_IncorrectStringFormat = 2701,
		GGE2702_InvalidEnumNumericValue = 2702,
		GGE2703_UnknownRelativeCoordinateDirection = 2703,
		GGE2704_RelativeCoordinateInInfiniteDimensions = 2704,
		GGE2705_MaxLessThanMin = 2705,
		GGE2706_IncorrectHeightProfileKeyType = 2706,
		GGE2707_IncorrectHeightProfileHeightType = 2707,
		GGE2708_IncorrectNoiseLayerWaveLengthType = 2708,
		GGE2709_IncorrectNoiseLayerAmplitudeType = 2709,
		GGE2710_NonpositiveNoiseLayerWaveLength = 2710,
		GGE2711_NonpositiveNoiseLayerAmplitude = 2711,
		GGE2712_InvalidTransformationMatrix = 2712,
		GGE2713_InvalidPersistence = 2713,
		GGE2714_InvalidScale = 2714,
		GGE2715_InvalidStrength = 2715,
		GGE2716_InvalidNumberOfStrata = 2716,

		GGE2801_MainMapNotGenerated = 2801,
		/// API usage error - such as incorrect parameter or a method being called in incorrect context).
		GGE3000_ApiUsageError = 3000,
		/// An unrecoverable internal error in GeoGen occured. These should be reported as bugs (although an incorrect usage of the API might also be sometimes the cause).
		GGE4000_InternalError = 4000,
		/// An error in the intermediate code was detected (such as incorrect state of the stack).
		GGE4001_IntermediateCodeError = 4001,
		/// An error was triggered using the TriggerError function from a script.
		GGE5000_UserError = 5000
	};
}