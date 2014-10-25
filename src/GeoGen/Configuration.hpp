#pragma once

#include "Serializable.hpp"

namespace geogen
{
		/// Settings allowing to slightly customize behavior of the generator.
		class Configuration : public Serializable
		{
		public:
			/// If this setting is set to true, the Compiler and VirtualMachine trigger errors if the script doesn't yield the main map.
			bool MainMapIsMandatory = true;

			virtual void Serialize(IOStream& stream) const;
		};
}