#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class MetadataKeyRedefinitionException : public CompilerException
		{
		private:
			std::string key;
		public:
			MetadataKeyRedefinitionException(CodeLocation location, std::string const& key) :
				CompilerException(GGE1401_MetadataKeyAlreadyDefined, location), key(key) {};

			inline std::string GetKey() const { return this->key; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Metadata key \"" << key << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << " was already defined.";

				return ss.str();
			}
		};
	}
}