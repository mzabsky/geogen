#pragma once

namespace geogen
{
	/// Error codes produced by GeoGen. Detailed information on each of the codes can be found in the scripting reference.
	enum ErrorCode
	{
		/// An unexpected character was encountered in the input. This usually means a severe syntax error.
		GGE1101_UnexpectedCharacter = 1101,
		/// An invalid escape sequence was encountered in the input.
		GGE1102_InvalidEscapeSequence = 1102,
		/// The parser hes encountered an unexpected token. 
		GGE1201_UnexpectedToken = 1201,
		/// The parser has encountered a `break` statement outside of a loop.
		GGE1301_InvalidBreak = 1301,
		/// The parser has encountered a `continue` statement outside of a loop.
		GGE1303_InvalidContinue = 1303,
		/// The parser has encountered a `return` statement outside of a function body.
		GGE1304_InvalidReturn = 1304,
		/// The script has declared a function which with a name which was already used by another function (either predefined or defined previously by the script).
		GGE1306_FunctionAlreadyDefined = 1306,
		/// A read only expression was used as a left-expression with a assignment operator such as =, += or ++. Only variable references and array item references are legal as left-expression.
		GGE1307_ReadOnlyExpressionWrite = 1307,
		/// The script attempted to define an enum type with a name already used another class (either predefined or script defined).
		GGE1308_TypeAlreadyDefined = 1308,
		/// The script defined an enum item with the same name as another item of the same enum type.
		GGE1309_EnumValueAlreadyDefined = 1309,
		/// The script defined an enum item with a numeric value that was not an integer.
		GGE1310_EnumValueNotInteger = 1310,
		/// The script defined an enum type with a name that is reserved by the language. Currently, the only reserved name is "Type".
		GGE1311_ReservedTypeNameUsed = 1311,
		/// The script didn't contain a `yield` statement returning the primary map.
		GGE1312_MainMapNotSupportedByScript = 1312,
		/// The script metadata defined key a key which was already defined under the same node.
		GGE1401_MetadataKeyAlreadyDefined = 1401,
		/// The `Parameters` node of script metadata was not key-value collection node.
		GGE1402_ScriptParametersNotKeyValueCollection = 1402,
		/// Node of script metadata node representing a single script parameter was not key-value collection node.
		GGE1403_ScriptParameterNotKeyValueCollection = 1403,
		/// Script parameter defined in script metadata used name reserved by the language.
		GGE1405_ReservedScriptParameterNameUsed = 1405,
		/// An attribute of a script parameter defined in script metadata was a node of a different type than expected.
		GGE1406_IncorrectScriptParameterAttributeType = 1406,
		/// Unknown script parameter restriction was used.
		GGE1407_IncorrectScriptParameterValueRestriction = 1407,
		/// The `Width` node of script metadata was of incorrect type.
		GGE1408_WidthNotKeyValueCollection = 1408,
		/// The `Height` node of script metadata was of incorrect type.
		GGE1409_HeightNotKeyValueCollection = 1409,
		/// An identifier used in script metadata was not recognized.
		GGE1410_UndefinedMetadataIdentifier = 1410,
		/// A map size (maximum, minimum or default width or height) in script metadata was outside of the supported range.
		GGE1411_IncorrectMapSizeNumericValue = 1411,
		/// A minimum map size (width or height) in script metadata was greater than corresponding maximum map size.
		GGE1412_MinGreaterThanMaxMapSize = 1412,
		/// The `Type` of an script parameter in script metadata was not valid.
		GGE1413_IncorrectEnumScriptParameterType = 1413,
		/// An argument passed to a native function was of incorrect type.
		GGE2101_IncorrectNativeFunctionArgumentType = 2101,
		/// An operand of an operator was of incorrect type.
		GGE2102_IncorrectOperandType = 2102,
		/// Incorrect number of arguments was passed to a function.
		GGE2103_IncorrectNumberOfArguments = 2103,
		/// Condition expression in an `if`, `while` or `for` statement returned a value of incorrect type (Boolean is expected).
		GGE2104_IncorrectConditionResultType = 2104,
		/// The script attempted to access a member of `null`.
		GGE2106_NullReference = 2106,
		/// The script attempted to yield an object of incorrect type (HeightMap is expected).
		GGE2107_IncorrectYieldType = 2107,
		/// The script attempted to write into a read-only variable.
		GGE2108_ReadOnlyWrite = 2108,
		/// The script attempted to call an undefined function.
		GGE2201_UndefinedFunction = 2201,
		/// The script attempted to read or write an undefined variable.
		GGE2202_UndefinedVariable = 2202,
		//GGE2203_UndefinedMemberVariable = 2203,
		/// The script attempted to define a variable which was already defined in the same scope.
		GGE2204_VariableAlreadyDefined = 2204,
		/// The script attempted to divide a number by zero.
		GGE2301_DivisionByZero = 2301,
		/// A number or a result of a numeric operation was outside of the range allowed by Number.
		GGE2302_NumberOverflow = 2302,
		/// The script attempted to specify a height outside of the range of allowed heigts (-1 to 1).
		GGE2303_HeightOverflow = 2303,
		/// The script attempted to specify a coordinate outside of the range of allowed values (-2147483648 to 2147483647).
		GGE2304_CoordinateOverflow = 2304,
		/// Argument of a trigonometric function was outside of its definition range.
		GGE2305_OutsideTrigonometricFunctionDefinitionRange = 2305,
		/// Argument of a Log function was outside of its definition range.
		GGE2306_OutsideLogFunctionDefinitionRange = 2306,
		/// Argument of a Sqrt function was outside of its definition range.
		GGE2307_OutsideSqrtFunctionDefinitionRange = 2307,
		/// A number or a result of a numeric operation was too small to represent with Number.
		GGE2308_NumberUnderflow = 2308,
		/// The script attempted to specify a size (or length) outside of the range of allowed values (0 to 4294967295).
		GGE2309_SizeOverflow = 2309,
		/// Call stack became too large while executing the script.
		GGE2501_CallStackOverflow = 2501,
		/// Code block stack became too large while executing the script.
		GGE2502_CodeBlockStackOverflow = 2502,
		/// Object stack became too large while executing the script.
		GGE2503_ObjectStackOverflow = 2503,
		/// Rendering sequence became too long while executing the script.
		GGE2504_RenderingSequenceTooLong = 2504,
		/// An operation that requires non-empty was called on an empty array.
		GGE2601_InvalidOperationOnEmptyArray = 2601,
		/// Specified key was not found in an array.
		GGE2602_ArrayKeyNotFound = 2602,
		/// The script attempted to insert a key-value pair with `null` as its key into an array.
		GGE2603_ArrayNullKey = 2603,
		/// Specified index was not found in the array.
		GGE2604_ArrayIndexNotFound = 2604,
		/// A string format template had incorrect formating marks in it.
		GGE2701_IncorrectStringFormat = 2701,
		/// The script attempted to translate a incorrect numeric value to an enum value using FromNumber function of that enum type.
		GGE2702_InvalidEnumNumericValue = 2702,
		/// The script attempted to use relative coordinate in context to which dimension of the map the coordinate is relative to could not be determined.
		GGE2703_UnknownRelativeCoordinateDirection = 2703,
		/// The script attempted to use relative coordinate in dimension in which the map is infinite.
		GGE2704_RelativeCoordinateInInfiniteDimensions = 2704,
		/// A maximum passed to a standard library function was less than a minimum.
		GGE2705_MaxLessThanMin = 2705,
		/// A key-value pair with a key of incorrect type was passed to HeightProfile.FromArray (Number is expected).
		GGE2706_IncorrectHeightProfileKeyType = 2706,
		/// A key-value pair with a value of incorrect type was passed to HeightProfile.FromArray (Number is expected).
		GGE2707_IncorrectHeightProfileHeightType = 2707,
		/// A wave length-amplitude pair with a key (wave length) of incorrect type was passed to HeightProfile. NHeightMap.Noise (Number is expected).
		GGE2708_IncorrectNoiseLayerWaveLengthType = 2708,
		/// A wave length-amplitude pair with a value (amplitude) of incorrect type was passed to HeightProfile.Noise HeightMap.Noise (Number is expected).
		GGE2709_IncorrectNoiseLayerAmplitudeType = 2709,
		/// A wave length-amplitude pair with a non-positive wave length (key) was passed to HeightProfile.Noise or HeightMap.Noise.
		GGE2710_NonpositiveNoiseLayerWaveLength = 2710,
		/// A wave length-amplitude pair with a non-positive amplitude (value) was passed to HeightProfile.Noise or HeightMap.Noise.
		GGE2711_NonpositiveNoiseLayerAmplitude = 2711,
		/// An invalid transformation matrix was passed to HeightMap.Transform.
		GGE2712_InvalidTransformationMatrix = 2712,
		/// A persistence number passed to CreateNoiseLayers was ouside of the supported range (0.1 to 0.9).
		GGE2713_InvalidPersistence = 2713,
		/// A scale number passed to function like HeightMap.Rescale was ouside of the supported range (0.1 to 10).
		GGE2714_InvalidScale = 2714,
		/// A strength number passed to function like HeightMap.Glaciate was ouside of the supported range (0 to 1).
		GGE2715_InvalidStrength = 2715,
		/// Number of strata passed to function like HeightMap.Stratify was ouside of the supported range (1 to 100).
		GGE2716_InvalidNumberOfStrata = 2716,
		/// The script finished without yielding the main height map.
		GGE2801_MainMapNotGenerated = 2801,
		/// Rendering step requires too much memory.
		GGE3001_RendererMemoryLimitReached = 3001,
		/// API usage error - such as incorrect parameter or a method being called in incorrect context).
		GGE4000_ApiUsageError = 4000,
		/// An unrecoverable internal error in GeoGen occured. These should be reported as bugs (although an incorrect usage of the API might also be sometimes the cause).
		GGE5000_InternalError = 5000,
		/// An error in the intermediate code was detected (such as incorrect state of the stack).
		GGE5001_IntermediateCodeError = 5001,
		/// An error was triggered using the TriggerError function from a script.
		GGE6000_UserError = 6000
	};
}