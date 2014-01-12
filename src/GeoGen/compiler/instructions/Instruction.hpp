#pragma once

#include <string>
#include <ostream>

#include "..\..\Serializable.hpp"
#include "..\..\CodeLocation.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class Instruction : public Serializable
			{
			private:
				CodeLocation location;
			protected:
				Instruction(CodeLocation location) : location(location) {}
			public:
				inline CodeLocation GetLocation() const { return this->location; }
			};
		}
	}
}