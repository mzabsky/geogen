#pragma once

#include <vector>

namespace geogen
{
	namespace renderer
	{
		class RenderingStep;

		class RenderingGraphNode
		{
		private:
			RenderingStep const* step;
			std::vector<RenderingGraphNode*> edges;
			std::vector<RenderingGraphNode*> backEdges;

			// Non-copyable
		public:
			typedef std::vector<RenderingGraphNode const*>::const_iterator const_iterator;
			typedef std::vector<RenderingGraphNode*>::iterator iterator;

			RenderingGraphNode() : step(NULL) {};
			RenderingGraphNode(RenderingStep const* step) : step(step) {}
			RenderingGraphNode(RenderingGraphNode const& other) { this->step = other.step; this->edges = other.edges; this->backEdges = other.backEdges; };
			RenderingGraphNode& operator=(RenderingGraphNode const& other) { this->step = other.step; this->edges = other.edges; this->backEdges = other.backEdges; return *this; };

			inline RenderingStep const* GetStep() { return this->step; }

			inline void AddEdge(RenderingGraphNode* edge) { this->edges.push_back(edge); edge->backEdges.push_back(this); }
			//inline void AddBackEdge(RenderingGraphNode* edge) { this->edges.push_back(edge); }

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