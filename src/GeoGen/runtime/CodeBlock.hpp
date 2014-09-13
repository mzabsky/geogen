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


		/// A linear sequence of instruction.
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