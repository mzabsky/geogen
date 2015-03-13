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

#pragma once

#include <vector>

#include "../Serializable.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class Instruction;
		}


		/// Linear sequence of instructions.
		class CodeBlock : public Serializable
		{
		private:
			std::vector<instructions::Instruction const*> instructions;			
		public:		
			/// Default constructor.
			CodeBlock() {};

			/// Copy constructor.
			/// @param other The other.
			CodeBlock(CodeBlock const& other);

			/// Assignment operator.
			/// @param other The other.
			/// @return A copy of this CodeBlock.
			CodeBlock& operator=(CodeBlock const&);

			/// Destructor. Destroys all owned instructions.
			~CodeBlock();

			/// A const iterator.
			typedef std::vector<instructions::Instruction const*>::const_iterator const_iterator;

			/// Adds an instruction to the end of the code block. The code block will take ownership of the pointer.
			/// @param instruction The instruction.
			void AddInstruction(instructions::Instruction const* instruction);

			/// Move all instructions from another code block. Ownership of all the instruction is transferred to this code block.
			/// @param another The other code block.
			void MoveInstructionsFrom(CodeBlock& another);

			/// Gets an iterator pointing to the first instruction.
			/// @return An iterator.
			inline const_iterator Begin() const { return this->instructions.begin(); }

			/// Gets the reverse end iterator.
			/// @return An iterator.
			inline const_iterator End() const { return this->instructions.end(); }

			virtual void Serialize(IOStream& stream) const;
		};
	}
}