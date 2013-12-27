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

			// Non-copyable
			SymbolDefinitionTable(SymbolDefinitionTable const&) {}
			SymbolDefinitionTable& operator=(SymbolDefinitionTable const&) {};
		public:
			SymbolDefinitionTable() {}

			typedef typename std::map<std::string, TSymbolBase const*>::const_iterator const_iterator;
			//typedef std::map<std::string, TSymbolBase*>::iterator iterator;

			void MoveItemsFrom(SymbolDefinitionTable<TSymbolBase>& another)
			{
				for (std::map<std::string, TSymbolBase*>::iterator it = another.table.begin(); it != another.table.end(); it++)
				{
					this->AddItem(it->second);
				}

				another.table.clear();
			}

			inline TSymbolBase* GetItem(std::string const& name) 
			{
				std::map<std::string, TSymbolBase*>::iterator item = this->table.find(name);
				
				if(item == this->table.end()){
					throw CompilerException("Symbol " + name + " not defined.");
				}

				return (*item).second;
			};

			inline TSymbolBase const* GetItem(std::string const& name) const 
			{
				std::map<std::string, TSymbolBase*>::const_iterator item = this->table.find(name);
				
				if(item == this->table.end()){
					throw CompilerException("Symbol " + name + " not defined.");
				}

				return (*item).second;
			};

			inline bool ContainsItem(std::string const& name) const { return this->table.find(name) == this->table.end(); };

			inline void AddItem(TSymbolBase* symbol) {				
				if(this->table.find(symbol->GetName()) != this->table.end()){
					throw CompilerException("Symbol " + symbol->GetName() + " already defined.");
				}

				this->table[symbol->GetName()] = symbol;
			};

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			~SymbolDefinitionTable()
			{
				for(std::map<std::string, TSymbolBase*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete it->second;
				}
			}
		};
	}
}