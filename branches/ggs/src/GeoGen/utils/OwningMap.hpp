#pragma once

#include <string>
#include <map>

namespace geogen
{
	namespace utils
	{
		template<typename T>
		class OwningMap
		{
		protected:
			std::map<std::string, T*> table;
		public:
			bool AddItem(std::string const& name, T* value)
			{
				if (this->table.find(name) != this->table.end()){
					return false;
				}

				this->table[name] = value;

				return true;
			}

			inline unsigned Size() const { return this->table.size(); }
			inline typename std::map<std::string, T const*>::const_iterator Begin() const { return *(typename std::map<std::string, T const*>::const_iterator*)(&this->table.begin()); }
			inline typename std::map<std::string, T const*>::const_iterator End() const { return *(typename std::map<std::string, T const*>::const_iterator*)(&this->table.end()); }
			inline typename std::map<std::string, T*>::iterator Begin() { return this->table.begin(); }
			inline typename std::map<std::string, T*>::iterator End() { return this->table.end(); }

			inline typename std::map<std::string, T const*>::const_iterator FindItem(std::string const& key) const { return *(typename std::map<std::string, T const*>::const_iterator*)(&this->table.find(key)); }
			inline typename std::map<std::string, T*>::iterator FindItem(std::string const& key) { return this->table.find(key); }
			inline bool ContainsItem(std::string const& key) const { return this->FindItem(key) != this->End(); };
			T const* GetItem(std::string const& key) const
			{
				typename std::map<std::string, T const*>::const_iterator it = this->FindItem(key);
				if (it == this->End())
				{
					return NULL;
				}

				return it->second;
			};

			T* GetItem(std::string const& key)
			{
				typename std::map<std::string, T*>::iterator it = this->FindItem(key);
				if (it == this->End())
				{
					return NULL;
				}

				return it->second;
			};

			virtual ~OwningMap()
			{
				for (typename std::map<std::string, T*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete it->second;
				}
			}
		};
	}
}