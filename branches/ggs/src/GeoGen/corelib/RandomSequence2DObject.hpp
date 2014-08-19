#pragma once

#include "../runtime/ManagedObject.hpp"
#include "../random/RandomSequence2D.hpp"

namespace geogen
{
	namespace runtime
	{
		class RandomSequence2DObject : public ManagedObject
		{
		private:
			random::RandomSeed seed;
			random::RandomSequence2D sequence;
		public:
			RandomSequence2DObject(VirtualMachine* vm, TypeDefinition const* type, random::RandomSeed randomSeed) : ManagedObject(vm, type), sequence(randomSeed), seed(randomSeed)
			{
			};
			inline random::RandomSequence2D& GetSequence() { return this->sequence; }

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << GG_STR("Seed[") << this->seed << GG_STR("]");

				return ss.str();
			}
		};
	}
}