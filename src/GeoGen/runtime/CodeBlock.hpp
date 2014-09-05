#pragma once

#include <vector>

#include "instructions/Instruction.hpp"
#include "../Serializable.hpp"

namespace geogen 
{
	namespace runtime
	{
		class CodeBlock : public Serializable
		{
		private:
			std::vector<instructions::Instruction const*> instructions;			
		public:		
			CodeBlock() {};
			CodeBlock(CodeBlock const& other);
			CodeBlock& operator=(CodeBlock const&);
			~CodeBlock();

			typedef std::vector<instructions::Instruction const*>::const_iterator const_iterator;

			void AddInstruction(instructions::Instruction const* instruction);
			void MoveInstructionsFrom(CodeBlock& another);

			inline const_iterator Begin() const { return this->instructions.begin(); }
			inline const_iterator End() const { return this->instructions.end(); }

			virtual void Serialize(IOStream& stream) const;
		};
	}
}