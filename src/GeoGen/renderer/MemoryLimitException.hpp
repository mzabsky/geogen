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

#include "../ErrorCode.hpp"
#include "../GeoGenException.hpp"
#include "../utils/StringUtils.hpp"

namespace geogen
{
	namespace renderer
	{
		/// Exception thrown when the Renderer determines it can't execute a step because it would need to allocate amount of memory greater than Configuration::RendererMemoryLimit.
		class MemoryLimitException : public GeoGenException
		{
		private:
			unsigned memoryLimit;
			unsigned memoryRequired;
		public:
			explicit MemoryLimitException(unsigned memoryLimit, unsigned memoryRequired) :
				GeoGenException(GGE3001_RendererMemoryLimitReached), memoryLimit(memoryLimit), memoryRequired(memoryRequired) {};

			virtual ~MemoryLimitException() throw () {}

			unsigned GetMemoryLimit() const
			{
				return this->memoryLimit;
			}

			unsigned GetMemoryRequired() const
			{
				return this->memoryRequired;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Renderer requires " << geogen::utils::FormatFileSize(this->memoryRequired) << " of memory, which is more than it's limit of " << geogen::utils::FormatFileSize(this->memoryLimit) << ".";

				return ss.str();
			}
		};
	}
}