#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "CompilerException.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class SymbolNameTable
		{
		private:
			std::vector<std::string> table;

		public:
			typedef std::vector<std::string>::const_iterator const_iterator;

			void AddName(std::string name);
			int GetNameIndex(std::string name);
			int GetNameIndex(std::string name) const;
			size_t ContainsName(std::string name) const;

			inline const_iterator Begin() const { return this->table.begin(); }
			inline const_iterator End() const { return this->table.end(); }
		};
	}
}