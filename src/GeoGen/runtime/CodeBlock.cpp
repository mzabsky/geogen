/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <iomanip>

#include "CodeBlock.hpp"
#include "instructions/Instruction.hpp"

using namespace geogen;
using namespace runtime;
using namespace std;

CodeBlock::CodeBlock(CodeBlock const& other)
{
	for (std::vector<instructions::Instruction const*>::const_iterator it = other.instructions.begin(); it != other.instructions.end(); it++)
	{
		this->AddInstruction((*it)->Clone());
	}
}

CodeBlock& CodeBlock::operator=(CodeBlock const& other)
{
	if (this != &other)
	{
		for (std::vector<instructions::Instruction const*>::iterator it = this->instructions.begin(); it != this->instructions.end(); it++)
		{
			delete *it;
		}

		for (std::vector<instructions::Instruction const*>::const_iterator it = other.instructions.begin(); it != other.instructions.end(); it++)
		{
			this->AddInstruction((*it)->Clone());
		}
	}

	return *this;
}

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
	//std::move(another.instructions.begin(), another.instructions.end(), back_inserter(this->instructions));

	for(std::vector<instructions::Instruction const*>::iterator it = another.instructions.begin(); it != another.instructions.end(); it++)
	{
		this->instructions.push_back(*it);
	}

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
