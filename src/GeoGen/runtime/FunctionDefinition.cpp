#include "FunctionDefinition.hpp"
#include "IncorrectTypeException.hpp"
#include "NumberOfArgumentsException.hpp"
#include "ManagedObject.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void FunctionDefinition::CheckArguments(CodeLocation location, vector<TypeDefinition const*> expectedTypes, vector<ManagedObject*> actualArguments) const
{
	CheckArguments(location, expectedTypes.size(), actualArguments);

	for (vector<ManagedObject*>::size_type i = 0; i < actualArguments.size(); i++)
	{
		if (actualArguments[i]->IsStaticObject())
		{
			ErrorCode errorCode = this->GetFunctionType() == FUNCTION_TYPE_FUNCTION ? GGE2101_IncorrectNativeFunctionArgumentType : GGE2102_IncorrectOperandType;

			throw IncorrectTypeException(errorCode, location, expectedTypes[i]->GetName(), GG_STR("Static"));
		}

		if (actualArguments[i]->GetType() != expectedTypes[i])
		{
			ErrorCode errorCode = this->GetFunctionType() == FUNCTION_TYPE_FUNCTION ? GGE2101_IncorrectNativeFunctionArgumentType : GGE2102_IncorrectOperandType;

			throw IncorrectTypeException(errorCode, location, expectedTypes[i]->GetName(), actualArguments[i]->GetType()->GetName());
		}
	}
}

void FunctionDefinition::CheckArguments(CodeLocation location, unsigned expectedArgumentCount, vector<ManagedObject*> actualArguments) const
{
	if (actualArguments.size() != expectedArgumentCount)
	{
		throw NumberOfArgumentsException(location, expectedArgumentCount, actualArguments.size());
	}
}