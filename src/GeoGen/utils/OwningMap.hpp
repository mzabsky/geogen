#pragma once

#include <map>

#include "../String.hpp"

namespace geogen
{
	namespace utils
	{
		template<typename T>
		class OwningMap
		{
		protected:
			std::map<String, T*> table;
		public:
			bool AddItem(String const& name, T* value)
			{
				if (this->table.find(name) != this->table.end()){
					return false;
				}

				this->table[name] = value;

				return true;
			}

			inline unsigned Size() const { return this->table.size(); }
			inline typename std::map<String, T const*>::const_iterator Begin() const { return *(typename std::map<String, T const*>::const_iterator*)(&this->table.begin()); }
			inline typename std::map<String, T const*>::const_iterator End() const { return *(typename std::map<String, T const*>::const_iterator*)(&this->table.end()); }
			inline typename std::map<String, T*>::iterator Begin() { return this->table.begin(); }
			inline typename std::map<String, T*>::iterator End() { return this->table.end(); }

			inline typename std::map<String, T const*>::const_iterator FindItem(String const& key) const { return *(typename std::map<String, T const*>::const_iterator*)(&this->table.find(key)); }
			inline typename std::map<String, T*>::iterator FindItem(String const& key) { return this->table.find(key); }
			inline bool ContainsItem(String const& key) const { return this->FindItem(key) != this->End(); };
			T const* GetItem(String const& key) const
			{
				typename std::map<String, T const*>::const_iterator it = this->FindItem(key);
				if (it == this->End())
				{
					return NULL;
				}

				return it->second;
			};

			T* GetItem(String const& key)
			{
				typename std::map<String, T*>::iterator it = this->FindItem(key);
				if (it == this->End())
				{
					return NULL;
				}

				return it->second;
			};

			virtual ~OwningMap()
			{
				for (typename std::map<String, T*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete it->second;
				}
			}
		};
	}
}