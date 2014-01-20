#pragma once

#include <string>
#include <map>

#include "../ErrorCode.hpp"

namespace geogen 
{
	namespace runtime 
	{		
		template<class TSymbolBase>
		class SymbolDefinitionTable
		{
		private:
			std::map<int, TSymbolBase*> table;

			// Non-copyable
			SymbolDefinitionTable(SymbolDefinitionTable const&) {}
			SymbolDefinitionTable& operator=(SymbolDefinitionTable const&) {};
		public:
			SymbolDefinitionTable() {}

			typedef typename std::map<int, TSymbolBase const*>::const_iterator const_iterator;
			//typedef std::map<std::string, TSymbolBase*>::iterator iterator;

			/*void MoveItemsFrom(SymbolDefinitionTable<TSymbolBase>& another)
			{
				for (std::map<std::string, TSymbolBase*>::iterator it = another.table.begin(); it != another.table.end(); it++)
				{
					this->AddItem(it->second);
				}

				another.table.clear();
			}*/

			inline TSymbolBase* GetItem(int symbolNameIndex) 
			{
				std::map<int, TSymbolBase*>::iterator item = this->table.find(symbolNameIndex);
				
				if(item == this->table.end()){
					return NULL;
				}

				return (*item).second;
			};

			inline TSymbolBase const* GetItem(int symbolNameIndex) const
			{
				std::map<int, TSymbolBase*>::const_iterator item = this->table.find(symbolNameIndex);
				
				if(item == this->table.end()){
					return NULL;
				}

				return (*item).second;
			};

			inline bool ContainsItem(int symbolNameIndex) const { return this->table.find(symbolNameIndex) == this->table.end(); };

			inline bool AddItem(TSymbolBase* symbol, int symbolNameIndex) {				
				if (this->table.find(symbolNameIndex) != this->table.end()){
					return false;
				}

				this->table[symbolNameIndex] = symbol;

				return true;
			};

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			~SymbolDefinitionTable()
			{
				for(std::map<int, TSymbolBase*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete it->second;
				}
			}
		};
	}
}