#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "../Serializable.hpp"
#include "CompilerException.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class SymbolNameTable : public Serializable
		{
		private:
			std::vector<std::string> table;

		public:
			typedef std::vector<std::string>::const_iterator const_iterator;

			void AddName(std::string const& name);
			int GetNameIndex(std::string const& name);
			int GetNameIndex(std::string const& name) const;
			size_t ContainsName(std::string const& name) const;

			inline const_iterator Begin() const { return this->table.begin(); }
			inline const_iterator End() const { return this->table.end(); }

			virtual void Serialize(std::iostream& stream) const;
		};
	}
}