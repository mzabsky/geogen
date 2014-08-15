#include <iomanip>

#include "CodeBlock.hpp"
#include "instructions/Instruction.hpp"

using namespace geogen;
using namespace runtime;
using namespace std;

CodeBlock::~CodeBlock() 
{
	for (std::vector<instructions::Instruction const*>::iterator it = this->instructions.begin(); it != this->instructions.end(); it++)
	{
		delete *it;
	}
}

void CodeBlock::AddInstruction(instructions::Instruction const* instruction)
{
	this->instructions.push_back(instruction);
}

void CodeBlock::MoveInstructionsFrom(CodeBlock& another)
{
	std::move(another.instructions.begin(), another.instructions.end(), back_inserter(this->instructions));

	/*for(std::vector<instructions::Instruction const*>::iterator it = another.instructions.begin(); it != another.instructions.end(); it++)
	{
		this->instructions.push_back(*it);
	}*/

	another.instructions.clear();
}

void CodeBlock::Serialize(IOStream& stream) const
{
	//stream << "{" << std::endl;

	for(const_iterator it = this->Begin(); it != this->End(); it++)
	{
		//StringStream instructionStream;
		//String line;

		(*it)->Serialize(stream);
		stream << endl;

		/*while (getline(instructionStream, line))
		{
			stream << " [" << std::setw(3) << (*it)->GetLocation().GetLine() << ", " << std::setw(3) << (*it)->GetLocation().GetColumn() << "]\t" << line << std::endl;
		}*/
	}

	//stream << "}" << std::endl;
}
