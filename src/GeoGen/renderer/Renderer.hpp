#pragma once

#include <vector>

#include "../Point.hpp"
#include "RenderingSequence.hpp"
#include "RendererObjectTable.hpp"

namespace geogen
{
	namespace renderer
	{
		enum RendererStepResult 
		{
			RENDERER_STEP_RESULT_RUNNING,
			RENDERER_STEP_RESULT_FINISHED
		};

		enum RendererStatus
		{
			RENDERER_STATUS_READY,
			RENDERER_STATUS_FINISHED,
			RENDERER_STATUS_FAULTED
		};

		class Renderer
		{
		private:
			RenderingSequence& renderingSequence;
			RendererObjectTable objectTable;
			RendererStatus status;

			// Non-copyable
			Renderer(Renderer const&) : renderingSequence(*(RenderingSequence*)NULL), objectTable(0) {};
			Renderer& operator=(Renderer const&) {};
		public:
			Renderer(RenderingSequence& renderingSequence);

			inline RendererStatus GetStatus() const { return this->status; }

			inline RenderingSequence& GetRenderingSequence() { return this->renderingSequence; }
			inline RenderingSequence const& GetRenderingSequence() const { return this->renderingSequence; }
			inline RendererObjectTable& GetObjectTable() { return this->objectTable; }
			inline RendererObjectTable const& GetObjectTable() const { return this->objectTable; }

			RendererStepResult Step();
			void Run();
		};
	}
}