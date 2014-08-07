#pragma once

#include <vector>

#include "../GeoGen/String.hpp"

namespace geogen
{
	namespace console
	{
		class Command
		{
		protected:
			std::vector<String> cues;
		public:
			virtual String GetName() = 0;
			inline std::vector<String>& GetCues() { return this->cues; };
			virtual String GetHelpString() = 0;

			virtual ~Command() {}
		};
	}
}