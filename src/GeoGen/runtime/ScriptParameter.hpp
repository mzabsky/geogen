#pragma once

#include "../Serializable.hpp"
#include "../String.hpp"
#include "../InternalErrorException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Type of a ScriptParameter.
		enum ScriptParameterType
		{
			/// Number. 
			SCRIPT_PARAMETER_TYPE_NUMBER,

			/// Boolean. 
			SCRIPT_PARAMETER_TYPE_BOOLEAN,

			// Enum.
			SCRIPT_PARAMETER_TYPE_ENUM
		};

		/// Base class for classes representing individual types of script parameters.
		class ScriptParameter : public Serializable
		{
		protected:
			String name;
			String label;
			String description;

		public:
			/// Virtual destructor.
			virtual ~ScriptParameter() {};

			/// Constructor.
			/// @param name The parameter name.
			/// @param label The user friendly label.
			/// @param description The description.
			ScriptParameter(String const& name, String const& label, String const& description) : name(name), label(label), description(description) { };

			/// Gets the name.
			/// @return The name.
			inline String GetName() const { return this->name; }

			/// Gets the label.
			/// @return The label.
			inline String GetLabel() const { return this->label; }

			/// Gets the description.
			/// @return The description.
			inline String GetDescription() const { return this->description; }

			/// Gets the script parameter type.
			/// @return The type.
			virtual ScriptParameterType GetType() const = 0;

			/// Resets to default.
			virtual void ResetToDefault() = 0;

			/// Compares this script parameter to another script parameter.
			/// @param other The other.
			/// @return true if the two parameters are equal.
			virtual bool EqualsTo(ScriptParameter const* other) const = 0;

			/// Makes a deep copy of this script parameter.
			/// @return The copy.
			virtual ScriptParameter* Clone() const = 0;

			virtual void Serialize(IOStream& stream) const = 0;
		};

		/// Converts a script parameter type to string.
		/// @param dimensions The script parameter type.
		/// @return Converted string.
		static inline String ScriptParameterTypeToString(ScriptParameterType scriptParameterType)
		{
			switch (scriptParameterType)
			{
			case SCRIPT_PARAMETER_TYPE_NUMBER: return GG_STR("Number");
			case SCRIPT_PARAMETER_TYPE_BOOLEAN: return GG_STR("Boolean");
			case SCRIPT_PARAMETER_TYPE_ENUM: return GG_STR("Enum");
			default:
				throw InternalErrorException(GG_STR("Invalid ScriptParameterType."));
			}
		}
	}
}