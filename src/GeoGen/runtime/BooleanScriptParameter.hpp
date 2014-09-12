#pragma once

#include "ScriptParameter.hpp"

namespace geogen
{
	namespace runtime
	{
		/// A script parameter of type Boolean.
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
			/// @return A copy of this BooleanScriptParameter.
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