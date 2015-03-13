/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <map>

#include "../Serializable.hpp"
#include "../ErrorCode.hpp"
#include "../ApiUsageException.hpp"

namespace geogen 
{
	namespace runtime 
	{		
		/// Table containing symbol definitions, indexed by name. Has owning and non-owning mode.
		/// @tparam TSymbolBase Type of the base symbol class.
		/// @tparam TOwning True if the table is to own the contained pointers exclusively, false otherwise.
		template<class TSymbolBase, bool TOwning = false>
		class SymbolDefinitionTable : public Serializable
		{
		private:
			typename std::map<String, TSymbolBase*> table;

			// Non-copyable
			SymbolDefinitionTable(SymbolDefinitionTable const&) {}
			SymbolDefinitionTable& operator=(SymbolDefinitionTable const&) {};
		public:
			SymbolDefinitionTable() {}

			typedef typename std::map<String, TSymbolBase const*>::const_iterator const_iterator;

			inline TSymbolBase const* GetItem(String const& symbolName) const
			{
				typename std::map<String, TSymbolBase*>::const_iterator item = this->table.find(symbolName);
				
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

			inline unsigned Size() const { return this->table.size(); }

			inline const_iterator Begin() const { typename std::map<String, TSymbolBase*>::const_iterator it = this->table.begin(); return (const_iterator&)(it); }
			inline const_iterator End() const { typename std::map<String, TSymbolBase*>::const_iterator it = this->table.end(); return (const_iterator&)(it); }

			virtual void Serialize(IOStream& stream) const
			{
				for (const_iterator it = this->Begin(); it != this->End(); it++)
				{
					stream << it->first << GG_STR(": ") << std::endl;
					it->second->SerializeWithTabs(stream, 1);
				}
			}

			~SymbolDefinitionTable()
			{
				if (TOwning)
				{
					for (typename std::map<String, TSymbolBase*>::iterator it = this->table.begin(); it != this->table.end(); it++)
					{
						delete it->second;
					}
				}
			}
		};
	}
}