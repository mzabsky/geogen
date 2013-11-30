#pragma once

#include <string>
#include <map>

#include "CompilerException.hpp"

namespace geogen 
{
	namespace compiler 
	{		
		template<class TSymbolBase>
		class SymbolDefinitionTable
		{
		private:
			std::map<std::string, TSymbolBase*> table;
		public:

			inline TSymbolBase* GetItem(std::string name) { return this->table[name]; };
			inline TSymbolBase const* GetItem(std::string name) const { return (*this->table.find(name)).second; };

			inline bool ContainsItem(std::string name) const { return this->table.find(name) == this->table.end(); };

			inline void AddItem(TSymbolBase* symbol) {				
				if(this->table->find(symbol->GetName()) == this->table->end()){
					throw new CompilerException("Symbol " << symbol->GetName() << " already defined.");
				}

				this->table[symbol->GetName()] = function;

				return Function;
			};
		};
	}
}