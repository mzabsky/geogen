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

#include "ScriptParameter.hpp"
#include "../Number.hpp"

namespace geogen
{
	namespace corelib 
	{
		class EnumTypeDefinition;
	}

	namespace runtime
	{
		/// Script parameter of type Enum.
		class EnumScriptParameter : public ScriptParameter
		{
		private:
			int value;
			int defaultValue;
			corelib::EnumTypeDefinition const* enumType;
		public:

			/// Constructor.
			/// @param name The name.
			/// @param label The label.
			/// @param description The description.
			/// @param enumType Type of the enum.
			/// @param defaultValue The default value.
			EnumScriptParameter(String const& name, String const& label, String const& description, corelib::EnumTypeDefinition const* enumType, int defaultValue);

			/// Copy constructor.
			/// @param other The other.
			EnumScriptParameter(EnumScriptParameter const&);

			/// Assignment operator.
			/// @param other The other.
			/// @return The copy.
			EnumScriptParameter& operator=(EnumScriptParameter const&);

			virtual ScriptParameterType GetType() const { return SCRIPT_PARAMETER_TYPE_ENUM; };

			/// Gets definition of the enum type.
			/// @return The type definition.
			inline corelib::EnumTypeDefinition const* GetEnumType() const { return this->enumType; }

			/// Gets the default value.
			/// @return The default value.
			inline int GetDefaultValue() const { return this->defaultValue; }

			/// Gets current value.
			/// @return The value.
			inline int GetValue() const { return this->value; }

			/// Sets current value.
			/// @param value The value.
			void SetValue(int value);

			/// Sets current value.
			/// @param value The value.
			void SetValue(String value);

			virtual bool EqualsTo(ScriptParameter const* other) const;
			virtual void ResetToDefault() { this->value = this->defaultValue; };
			virtual ScriptParameter* Clone() const;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}