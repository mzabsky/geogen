#pragma once

#include <vector>
#include <map>

#include "RenderingSequence.hpp"
#include "../CodeLocation.hpp"
#include "../Point.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingBounds;
		class RenderingStep;

		class RenderingSequenceMetadata
		{
		private:
			std::map<RenderingStep const*, unsigned> stepNumbers;
			std::vector<RenderingBounds*> renderingBounds;

			// Non-copyable
			RenderingSequenceMetadata(RenderingSequenceMetadata const&) {};
			RenderingSequenceMetadata& operator=(RenderingSequenceMetadata const&) {};
		protected:
		public:
			RenderingSequenceMetadata(RenderingSequence const& renderingSequence);
			~RenderingSequenceMetadata();

			RenderingBounds* GetRenderingBounds(RenderingStep const* step);

			unsigned GetStepNumberByAddress(RenderingStep const* step) const;
		};
	}
}