#pragma once

#include <vector>

#include "../Point.hpp"
#include "RenderingSequence.hpp"
#include "RendererObjectTable.hpp"
#include "RenderingSequenceMetadata.hpp"
#include "RenderingGraph.hpp"
#include "RenderedMapTable.hpp"

namespace geogen
{
	namespace renderer
	{
		/// Results of a Renderer.Step call.
		enum RendererStepResult 
		{
			/// The step was executed and next step is available and ready to be executed.
			RENDERER_STEP_RESULT_RUNNING,

			/// The step was executed, but it was the last and the Renderer can't continue.
			RENDERER_STEP_RESULT_FINISHED
		};

		/// State of a Renderer.
		enum RendererStatus
		{
			/// The renderer is ready to step.
			RENDERER_STATUS_READY,

			/// The renderer is finished, no more steps are available.
			RENDERER_STATUS_FINISHED,

			/// The renderer has caused an error and can't continue.
			RENDERER_STATUS_FAULTED
		};

		/// Renders a single RenderingSequence into a set of height maps.
		class Renderer
		{
		private:
			RenderingSequence const& renderingSequence;
			RenderingSequence::const_iterator nextStep;

			RendererObjectTable objectTable;
			RendererStatus status;
			RenderingSequenceMetadata renderingSequenceMetadata;
			RenderingGraph graph;
			RenderedMapTable renderedMapTable;


			// Non-copyable
			Renderer(Renderer const&) : renderingSequence(*(RenderingSequence*)NULL), objectTable(0), renderingSequenceMetadata(*(RenderingSequence*)NULL), graph(*(RenderingSequence*)NULL) {};
			Renderer& operator=(Renderer const&) {};
		public:
			static const String MAP_NAME_MAIN;

			/// Initializes a new instance of the Renderer class.
			/// @param renderingSequence The rendering sequence to be rendered with this instance. The
			/// rendering sequence must exist for whole life of the renderer.
			Renderer(RenderingSequence const& renderingSequence);

			/// Gets the status of the Renderer.
			/// @return The status.
			inline RendererStatus GetStatus() const { return this->status; }

			/// Gets the rendering sequence.
			/// @return The rendering sequence.
			inline RenderingSequence const& GetRenderingSequence() { return this->renderingSequence; }

			/// Gets the next rendering step to be executed.
			/// @return The rendering step or NULL if there are no more steps in the sequence.
			inline RenderingStep const* GetNextRenderingStep() const { return this->nextStep == this->renderingSequence.End() ? NULL : *this->nextStep; }

			/// Gets the rendering sequence.
			/// @return The rendering sequence.
			inline RenderingSequence const& GetRenderingSequence() const { return this->renderingSequence; }

			/// Gets rendering sequence metadata.
			/// @return The rendering sequence metadata.
			inline RenderingSequenceMetadata const& GetRenderingSequenceMetadata() const { return this->renderingSequenceMetadata; }

			/// Gets rendering sequence metadata.
			/// @return The rendering sequence metadata.
			inline RenderingSequenceMetadata& GetRenderingSequenceMetadata()  { return this->renderingSequenceMetadata; }

			/// Gets the object table.
			/// @return The object table.
			inline RendererObjectTable& GetObjectTable() { return this->objectTable; }

			/// Gets the object table.
			/// @return The object table.
			inline RendererObjectTable const& GetObjectTable() const { return this->objectTable; }

			/// Gets rendered map table.
			/// @return The rendered map table.
			inline RenderedMapTable const& GetRenderedMapTable() const { return this->renderedMapTable; }

			/// Gets rendered map table.
			/// @return The rendered map table.
			inline RenderedMapTable& GetRenderedMapTable() { return this->renderedMapTable; }

			/// Calculates the rendering bounds for all steps in the RenderingSequence.
			void CalculateRenderingBounds();

			/// Executes next step in the rendering sequence. Can only be called if the renderer is in status
			/// <see cref="RENDERER_STATUS_READY"/>.
			/// @return A step result.
			RendererStepResult Step();

			/// Executes steps of the rendering sequence until the renderer finishes or fails.
			void Run();
		};
	}
}