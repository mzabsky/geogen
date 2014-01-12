#include <ostream>
#include <sstream>

#include "WhileInstruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			void WhileInstruction::Serialize(std::iostream& stream) const
			{
				stream << "While" << std::endl;

				std::stringstream substream;
				this->codeBlock.Serialize(substream);
				
				stream << substream.str();
			}
		}
	}
}