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

namespace geogen
{
	namespace runtime
	{
		/// Script parameter of type Boolean.
		class BooleanScriptParameter : public ScriptParameter
		{
		private:
			bool defaultValue;
			bool value;
		public:

			/// Initializes a new instance of BooleanScriptParameter class.
			/// @param name The name.
			/// @param label The label.
			/// @param description The description.
			/// @param defaultValue The default value.
			BooleanScriptParameter(String const& name, String const& label, String const& description, bool defaultValue) : ScriptParameter(name, label, description), value(defaultValue), defaultValue(defaultValue) {};

			/// Copy constructor.
			/// @param other The other.
			BooleanScriptParameter(BooleanScriptParameter const&);

			/// Assignment operator.
			/// @param other The other.
			/// @return This instance.
			BooleanScriptParameter& operator=(BooleanScriptParameter const&);

			virtual ScriptParameterType GetType() const { return SCRIPT_PARAMETER_TYPE_BOOLEAN; };

			/// Gets default value.
			/// @return The default value.
			inline bool GetDefaultValue() const { return this->defaultValue; }

			/// Gets current value.
			/// @return The current value.
			inline bool GetValue() const { return this->value; }

			/// Sets current value.
			/// @param value The new current value.
			inline void SetValue(bool value) { this->value = value; }

			virtual void ResetToDefault() { this->value = this->defaultValue; };

			virtual bool EqualsTo(ScriptParameter const* other) const;

			virtual ScriptParameter* Clone() const;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}