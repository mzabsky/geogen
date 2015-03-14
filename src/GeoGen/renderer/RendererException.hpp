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

#include "../CodeLocation.hpp"
#include "../GeoGenException.hpp"

namespace geogen
{
	namespace renderer
	{
		/// Base class for exceptions throw during map rendering.
		class RendererException : public GeoGenException
		{
		private:
			CodeLocation location;
		public:

			/// Constructor.
			/// @param code The error code.
			/// @param location The code location.
			explicit RendererException(ErrorCode code, CodeLocation location) :
				GeoGenException(code), location(location) {};

			virtual ~RendererException() throw () {}

			/// Gets the code location.
			/// @return The code location.
			inline CodeLocation GetLocation(){ return this->location; }

			virtual String GetDetailMessage() = 0;
		};
	}
}