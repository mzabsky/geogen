#include <algorithm>

#include "FunctionDefinition.hpp"
#include "IncorrectTypeException.hpp"
#include "NumberOfArgumentsException.hpp"
#include "ManagedObject.hpp"
#include "TypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

bool FunctionDefinition::CheckArgument(VirtualMachine* vm, CodeLocation location, TypeDefinition const* expectedType, ManagedObject*& actualArgument, ManagedObjectHolder& managedObjectHolder) const
{
	if (actualArgument->IsStaticObject())
	{
		ErrorCode errorCode = this->GetFunctionType() == FUNCTION_TYPE_FUNCTION ? GGE2101_IncorrectNativeFunctionArgumentType : GGE2102_IncorrectOperandType;

		throw IncorrectTypeException(errorCode, location, expectedType->GetName(), GG_STR("Type"));
	}

	if (actualArgument->GetType() != expectedType)
	{
		if (expectedType->IsConvertibleFrom(vm, actualArgument->GetType()))
		{
			actualArgument = expectedType->Convert(vm, actualArgument);
			
			managedObjectHolder = ManagedObjectHolder(vm, actualArgument);
			return true;
		}
		else
		{
			ErrorCode errorCode = this->GetFunctionType() == FUNCTION_TYPE_FUNCTION ? GGE2101_IncorrectNativeFunctionArgumentType : GGE2102_IncorrectOperandType;

			throw IncorrectTypeException(errorCode, location, expectedType->GetName(), actualArgument->GetType()->GetName());
		}
	}

	return false;
}

vector<ManagedObjectHolder> FunctionDefinition::CheckArguments(VirtualMachine* vm, CodeLocation location, vector<TypeDefinition const*> expectedTypes, vector<ManagedObject*>& actualArguments, int requiredArgumentCount) const
{
	// Check argument count first
	if (requiredArgumentCount == -1)
	{
		CheckArguments(location, expectedTypes.size(), actualArguments);
	}	
	else 
	{
		CheckArguments(location, max((unsigned)requiredArgumentCount, min(expectedTypes.size(), actualArguments.size())), actualArguments);
	}

	vector<ManagedObjectHolder> holders;
	for (vector<ManagedObject*>::size_type i = 0; i < actualArguments.size(); i++)
	{
		ManagedObjectHolder holder;
		if (this->CheckArgument(vm, location, expectedTypes[i], actualArguments[i], holder))
		{
			holders.push_back(holder);
		}
	}

	return holders;
}

void FunctionDefinition::CheckArguments(CodeLocation location, unsigned expectedArgumentCount, vector<ManagedObject*> actualArguments) const
{
	if (actualArguments.size() != expectedArgumentCount)
	{
		throw NumberOfArgumentsException(location, expectedArgumentCount, actualArguments.size());
	}
}