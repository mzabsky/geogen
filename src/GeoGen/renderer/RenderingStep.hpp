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

#include <vector>

#include "../CodeLocation.hpp"
#include "../Point.hpp"
#include "../Serializable.hpp"

namespace geogen
{
	namespace renderer
	{
		class Renderer;
		class RenderingBounds;

		enum RenderingStepType
		{
			RENDERING_STEP_TYPE_1D,
			RENDERING_STEP_TYPE_2D
		};

		class RenderingStep : public Serializable
		{
		private:
			CodeLocation location;
			std::vector<unsigned> argumentSlots;
			unsigned returnSlot;			

			// Non-copyable
			RenderingStep(RenderingStep const&) : location( 0, 0) {};
			RenderingStep& operator=(RenderingStep const&) {};
		protected:
			void TriggerRenderingBoundsCalculationError(String message) const;			
		public:
			RenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot) : location(location), argumentSlots(argumentSlots), returnSlot(returnSlot) {}

			virtual RenderingStepType GetRenderingStepType() const = 0;
			virtual String GetName() const = 0;

			inline CodeLocation GetLocation() const { return this->location; }
			inline std::vector<unsigned> const& GetArgumentSlots() const { return this->argumentSlots; };
			inline unsigned GetReturnSlot() const { return this->returnSlot; }

			/// Executes this step.
			/// @param renderer The renderer.
			virtual void Step(Renderer* renderer) const = 0;

			/// Updates the rendering bounds of this step's argument steps based on this step's rendering bounds (used by Renderer::CalculateRenderingBounds).
			/// @param renderer			 The renderer.
			/// @param referencingBounds The referencing bounds.
			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const = 0;

			/// Gets number of bytes allocated (and usually released) by this step on top of any other object
			/// already allocated by the Renderer (used by Renderer::CalculateMemoryRequirements).
			/// @param renderer The renderer.
			/// @param argumentBounds The argument bounds.
			/// @return The number of bytes required by this step beyond any memory already allocated by existing objects.
			virtual unsigned GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const;

			/// Simulates the effect this step would have on the return RendererObject's size without actually executing it.
			/// @param renderingBounds Reference to the rendering bounds to update with the simulated bounds.
			virtual void SimulateOnRenderingBounds(RenderingBounds* renderingBounds) const;

			void Serialize(IOStream& stream) const;
			virtual void SerializeArguments(IOStream& stream) const {};
		};
	}
}