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

#include "Serializable.hpp"

namespace geogen
{
	/// Settings allowing to slightly customize behavior of the generator.
	class Configuration : public Serializable
	{
	public:
		/// If this setting is set to true, the Compiler and VirtualMachine trigger errors if the script doesn't yield the main map. Default: true.
		bool MainMapIsMandatory;

		/// Maximum sum of memory footprints of all the maps allocated simultaneously by the Renderer, in bytes. Default: 100 MiB.
		unsigned long RendererMemoryLimit;

		Configuration() :
			MainMapIsMandatory(true),
			RendererMemoryLimit(100 * 1024 * 1024) {};

		virtual void Serialize(IOStream& stream) const;
	};
}