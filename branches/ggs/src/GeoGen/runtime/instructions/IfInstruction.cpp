#include <ostream>
#include <sstream>

#include "IfInstruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			void IfInstruction::Serialize(std::iostream& stream) const
			{
				stream << "If" << std::endl;

				std::stringstream substream1;
				this->ifBranchCodeBlock.Serialize(substream1);

				stream << substream1.str();

				std::stringstream substream2;
				this->elseBranchCodeBlock.Serialize(substream2);

				stream << substream2.str();
			}
		}
	}
}