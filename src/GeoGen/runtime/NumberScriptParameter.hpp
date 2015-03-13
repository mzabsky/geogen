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
	namespace runtime
	{
		/// NumberScriptParameter value restrictions.
		enum ScriptParameterValueRestriction
		{
			/// Unrestricted.
			SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED,

			/// Integers only.
			SCRIPT_PARAMETER_VALUE_RESTRICTION_INTEGERS,
			
			/// Powers of two only.
			SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_2,
			
			/// Powers of ten only.
			SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_10,
		};

		/// Script parameter of type Number.
		class NumberScriptParameter : public ScriptParameter
		{
		private:
			Number defaultValue;
			Number value;
			Number min;
			Number max;
			ScriptParameterValueRestriction restriction;

			Number ApplyRestriction(Number value) const;
		public:

			/// Constructor.
			/// @param name The name.
			/// @param label The label.
			/// @param description The description.
			/// @param defaultValue The default value.
			/// @param min The minimum.
			/// @param max The maximum.
			/// @param restriction The restriction.
			NumberScriptParameter(String const& name, String const& label, String const& description, Number defaultValue, Number min, Number max, ScriptParameterValueRestriction restriction);

			/// Copy constructor.
			/// @param other The other.
			NumberScriptParameter(NumberScriptParameter const& other);

			/// Assignment operator.
			/// @param other The other.
			/// @return This instance.
			NumberScriptParameter& operator=(NumberScriptParameter const& other);

			virtual ScriptParameterType GetType() const { return SCRIPT_PARAMETER_TYPE_NUMBER; };

			/// Gets default value.
			/// @return The default value.
			inline Number GetDefaultValue() const { return this->defaultValue; }

			/// Gets the current value.
			/// @return The current value.
			inline Number GetValue() const { return this->value; }

			/// Gets the minimum.
			/// @return The minimum value.
			inline Number GetMin() const { return this->min; }

			/// Gets the maximum.
			/// @return The maximum value.
			inline Number GetMax() const { return this->max; }

			/// Gets the value restriction.
			/// @return The value restriction.
			inline ScriptParameterValueRestriction GetRestriction() const { return this->restriction; }

			/// Sets the value.
			/// @param value The value.
			void SetValue(Number value);

			virtual bool EqualsTo(ScriptParameter const* other) const;
			virtual void ResetToDefault() { this->value = this->defaultValue; };
			virtual ScriptParameter* Clone() const;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}