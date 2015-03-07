#pragma once

#include "Serializable.hpp"

namespace geogen
{
		/// Settings allowing to slightly customize behavior of the generator.
		class Configuration : public Serializable
		{
		public:
			/// If this setting is set to true, the Compiler and VirtualMachine trigger errors if the script doesn't yield the main map. Default: true.
			bool MainMapIsMandatory = true;

			/// Maximum sum of memory footprints of all the maps allocated simultaneously by the Renderer, in bytes. Default: 100 MiB.
			unsigned long long RendererMemoryLimit = 100 * 1024 * 1024 * 1024;

			virtual void Serialize(IOStream& stream) const;
		};
}