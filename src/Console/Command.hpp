#pragma once

#include <vector>

#include <GeoGen/GeoGen.hpp>

namespace geogen
{
	namespace console
	{
		class Command
		{
		protected:
			std::vector<String> cues;
		public:
			virtual String GetName() const = 0;
			inline std::vector<String> const& GetCues() const { return this->cues; };
			virtual String GetHelpString() const = 0;

			virtual ~Command() {}
		};
	}
}