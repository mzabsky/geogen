#pragma once

#include <vector>

namespace geogen 
{
	namespace renderer
	{
		class RenderingStep;

		class RenderingSequence 
		{
			std::vector<RenderingStep*> steps;
		public:
			const unsigned SLOT_NUMBER_NONE = -1;

			~RenderingSequence();

			void AddStep(RenderingStep* step);
		};
	}
}