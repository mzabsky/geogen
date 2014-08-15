#pragma once

#include <map>

#include "../Serializable.hpp"
#include "../ErrorCode.hpp"
#include "../ApiUsageException.hpp"

namespace geogen 
{
	namespace runtime 
	{		
		template<class TSymbolBase, bool TOwning = false>
		class SymbolDefinitionTable : public Serializable
		{
		private:
			std::map<String, TSymbolBase*> table;

			// Non-copyable
			SymbolDefinitionTable(SymbolDefinitionTable const&) {}
			SymbolDefinitionTable& operator=(SymbolDefinitionTable const&) {};
		public:
			SymbolDefinitionTable() {}

			typedef typename std::map<String, TSymbolBase const*>::const_iterator const_iterator;
			//typedef std::map<String, TSymbolBase*>::iterator iterator;

			/*void MoveItemsFrom(SymbolDefinitionTable<TSymbolBase>& another)
			{
				for (std::map<String, TSymbolBase*>::iterator it = another.table.begin(); it != another.table.end(); it++)
				{
					this->AddItem(it->second);
				}

				another.table.clear();
			}*/

			/*inline TSymbolBase* GetItem(String symbolName)
			{
				std::map<String, TSymbolBase*>::iterator item = this->table.find(symbolName);
				
				if(item == this->table.end()){
					return NULL;
				}

				return (*item).second;
			};*/

			inline TSymbolBase const* GetItem(String const& symbolName) const
			{
				std::map<String, TSymbolBase*>::const_iterator item = this->table.find(symbolName);
				
				if(item == this->table.end()){
					return NULL;
				}

				return (*item).second;
			};

			inline bool ContainsItem(String const& symbolName) const { return this->table.find(symbolName) != this->table.end(); };

			inline bool AddItem(TSymbolBase const* symbol) {
				if (TOwning)
				{
					throw ApiUsageException(GG_STR("Cannot add const object to an exclusively owning collection."));
				}

				if (this->table.find(symbol->GetName()) != this->table.end()){
					return false;
				}

				// We know only const operations will be done with the object (if the collection is not exclusively owning).
				this->table[symbol->GetName()] = const_cast<TSymbolBase*>(symbol);

				return true;
			};

			inline bool AddItem(TSymbolBase* symbol) {
				if (this->table.find(symbol->GetName()) != this->table.end()){
					return false;
				}

				this->table[symbol->GetName()] = symbol;

				return true;
			};

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			virtual void Serialize(IOStream& stream) const
			{
				stream << GG_STR("{") << std::endl;
				
				for (const_iterator it = this->Begin(); it != this->End(); it++)
				{
					stream << GG_STR("\t") << it->first << GG_STR(": ");
					it->second->SerializeWithTabs(stream, 1);
				}

				stream << GG_STR("}") << std::endl;
			}

			~SymbolDefinitionTable()
			{
				if (TOwning)
				{
					for (std::map<String, TSymbolBase*>::iterator it = this->table.begin(); it != this->table.end(); it++)
					{
						delete it->second;
					}
				}
			}
		};
	}
}