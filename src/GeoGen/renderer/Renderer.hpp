#pragma once

#include <vector>

#include "../Point.hpp"
#include "RenderingSequence.hpp"
#include "RendererObjectTable.hpp"
#include "RenderingSequenceMetadata.hpp"
#include "RenderingGraph.hpp"

namespace geogen
{
	namespace renderer
	{
		/// <summary> Results of a <see cref="Renderer.Step"/> call. </summary>
		enum RendererStepResult 
		{
			/// <summary> The step was executed and next step is available and ready to be executed. </summary>
			RENDERER_STEP_RESULT_RUNNING,
			/// <summary> The step was executed, but it was the last and the Renderer can't continue. </summary>
			RENDERER_STEP_RESULT_FINISHED
		};

		/// <summary> State of a <see cref="Renderer"/>. </summary>
		enum RendererStatus
		{
			/// <summary> The renderer is ready to step. </summary>
			RENDERER_STATUS_READY,
			/// <summary> The renderer is finished, no more steps are available. </summary>
			RENDERER_STATUS_FINISHED,
			/// <summary> The renderer has caused an error and can't continue. </summary>
			RENDERER_STATUS_FAULTED
		};

		/// <summary> Renders a single <see cref="RenderingSequence"/> into a set of height maps. </summary>
		class Renderer
		{
		private:
			RenderingSequence const& renderingSequence;
			RendererObjectTable objectTable;
			RendererStatus status;
			RenderingSequenceMetadata renderingSequenceMetadata;
			RenderingGraph graph;

			// Non-copyable
			Renderer(Renderer const&) : renderingSequence(*(RenderingSequence*)NULL), objectTable(0), renderingSequenceMetadata(*(RenderingSequence*)NULL), graph(*(RenderingSequence*)NULL) {};
			Renderer& operator=(Renderer const&) {};
		public:

			/// <summary> Initializes a new instance of the Renderer class. </summary>
			/// <param name="renderingSequence"> The rendering sequence to be rendered with this instance. The rendering sequence must exist for whole life of the renderer. </param>
			Renderer(RenderingSequence const& renderingSequence);

			/// <summary> Gets the status of the Renderer. </summary>
			/// <returns> The status. </returns>
			inline RendererStatus GetStatus() const { return this->status; }

			/// <summary> Gets the rendering sequence. </summary>
			/// <returns> The rendering sequence. </returns>
			inline RenderingSequence const& GetRenderingSequence() { return this->renderingSequence; }

			/// <summary> Gets the rendering sequence. </summary>
			/// <returns> The rendering sequence. </returns>
			inline RenderingSequence const& GetRenderingSequence() const { return this->renderingSequence; }

			inline RenderingSequenceMetadata const& GetRenderingSequenceMetadata() const { return this->renderingSequenceMetadata; }
			inline RenderingSequenceMetadata& GetRenderingSequenceMetadata()  { return this->renderingSequenceMetadata; }

			
			/// <summary> Gets the object table. </summary>
			/// <returns> The object table. </returns>
			inline RendererObjectTable& GetObjectTable() { return this->objectTable; }
			
			/// <summary> Gets the object table. </summary>
			/// <returns> The object table. </returns>
			inline RendererObjectTable const& GetObjectTable() const { return this->objectTable; }

			void CalculateRenderingBounds();

			/// <summary> Executes next step in the rendering sequence. Can only be called if the renderer is in status <see cref="RENDERER_STATUS_READY"/>.</summary>
			/// <returns> A step result. </returns>
			RendererStepResult Step();

			/// <summary> Executes steps of the rendering sequence until the renderer finished or fails. </summary>
			void Run();
		};
	}
}