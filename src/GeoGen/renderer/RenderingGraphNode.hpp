/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <cstddef>
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
			RenderingGraphNode(RenderingStep const* step);

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
			void AddEdge(RenderingGraphNode* target);
			
			inline const_iterator ForwardBegin() const { std::vector<RenderingGraphNode*>::const_iterator it = this->edges.begin(); return (const_iterator&)(it); }
			inline const_iterator ForwardEnd() const { std::vector<RenderingGraphNode*>::const_iterator it = this->edges.end(); return (const_iterator&)(it); }
			inline iterator ForwardBegin() { return this->edges.begin(); }
			inline iterator ForwardEnd() { return this->edges.end(); }

			inline const_iterator BackBegin() const { std::vector<RenderingGraphNode*>::const_iterator it = this->backEdges.begin(); return (const_iterator&)(it); }
			inline const_iterator BackEnd() const { std::vector<RenderingGraphNode*>::const_iterator it = this->backEdges.end(); return (const_iterator&)(it); }
			inline iterator BackBegin() { return this->backEdges.begin(); }
			inline iterator BackEnd() { return this->backEdges.end(); }
		};
	}
}