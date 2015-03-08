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