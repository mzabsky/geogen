#pragma once

#include "../Serializable.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Settings allowing to slightly customize behavior of the Compiler.
		class CompilerConfiguration : public Serializable
		{
		public:
			/// If this setting is set to true, the Compiler will throw GGE1312_MainMapNotSupportedByScript if the script doesn't yield the main map anywhere.
			bool MainMapIsMandatory = true;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}