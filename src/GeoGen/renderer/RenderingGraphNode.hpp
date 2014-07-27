#pragma once

#include <vector>

namespace geogen
{
	namespace renderer
	{
		class RenderingStep;

		/// Single node in RenderingGraph.
		class RenderingGraphNode
		{
		private:
			RenderingStep const* step;
			std::vector<RenderingGraphNode*> edges;
			std::vector<RenderingGraphNode*> backEdges;
		public:
			typedef std::vector<RenderingGraphNode const*>::const_iterator const_iterator;
			typedef std::vector<RenderingGraphNode*>::iterator iterator;

			/// Creates a node which is not associated with any step. Required to make this object work with STL containers (should not be used in actual graphs).
			RenderingGraphNode() : step(NULL) {};

			/// Initializes a new instance of the RenderingStepNode class.
			/// @param step The rendering step.
			RenderingGraphNode(RenderingStep const* step) : step(step) {}

			/// Copy constructor.
			/// @param other The other instance.
			RenderingGraphNode(RenderingGraphNode const& other) { this->step = other.step; this->edges = other.edges; this->backEdges = other.backEdges; };

			/// Assignment operator.
			/// @param other The other instance.
			/// @return A shallow copy of this RenderingGraphNode.
			RenderingGraphNode& operator=(RenderingGraphNode const& other) { this->step = other.step; this->edges = other.edges; this->backEdges = other.backEdges; return *this; };

			/// Gets the rendering step associated  with this node.
			/// @return The rendering step.
			inline RenderingStep const* GetStep() { return this->step; }

			/// Adds a forward edge to this node and a reverse edge to the target node.
			/// @param target The taget node.
			inline void AddEdge(RenderingGraphNode* target) { this->edges.push_back(target); target->backEdges.push_back(this); }
			
			inline const_iterator ForwardBegin() const { return *(const_iterator*)(&this->edges.begin()); }
			inline const_iterator ForwardEnd() const { return *(const_iterator*)(&this->edges.end()); }
			inline iterator ForwardBegin() { return this->edges.begin(); }
			inline iterator ForwardEnd() { return this->edges.end(); }

			inline const_iterator BackBegin() const { return *(const_iterator*)(&this->backEdges.begin()); }
			inline const_iterator BackEnd() const { return *(const_iterator*)(&this->backEdges.end()); }
			inline iterator BackBegin() { return this->backEdges.begin(); }
			inline iterator BackEnd() { return this->backEdges.end(); }
		};
	}
}