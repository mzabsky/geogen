#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class MetadataKeyRedefinitionException : public CompilerException
		{
		private:
			String key;
		public:
			MetadataKeyRedefinitionException(CodeLocation location, String const& key) :
				CompilerException(GGE1401_MetadataKeyAlreadyDefined, location), key(key) {};

			inline String GetKey() const { return this->key; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Metadata key \"" << key << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << " was already defined.";

				return ss.str();
			}
		};
	}
}