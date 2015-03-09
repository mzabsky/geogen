#pragma once

#include <vector>

#include "../Number.hpp"
#include "../Serializable.hpp"

namespace geogen 
{
	namespace renderer
	{
		class RenderingStep;

		class RenderingSequence : public Serializable
		{
			std::vector<RenderingStep*> steps;
			unsigned objectTableSize;
			Scale renderScale;

			// Non-copyable
			RenderingSequence(RenderingSequence const&) {};
			RenderingSequence& operator=(RenderingSequence const&) {};
		public:
			/// Maximum number of steps allowed to be in the sequence.
			static const unsigned SIZE_LIMIT;

			typedef std::vector<RenderingStep const*>::const_iterator const_iterator;
			typedef std::vector<RenderingStep*>::iterator iterator;
			typedef std::vector<RenderingStep const*>::const_reverse_iterator const_reverse_iterator;
			typedef std::vector<RenderingStep*>::reverse_iterator reverse_iterator;

			RenderingSequence(Scale renderScale) : renderScale(renderScale), objectTableSize(0){};
			~RenderingSequence();

			inline Scale GetRenderScale() const { return this->renderScale; }

			inline const_iterator Begin() const { std::vector<RenderingStep*>::const_iterator it = this->steps.begin(); return (const_iterator&)(it); }
			inline const_iterator End() const { std::vector<RenderingStep*>::const_iterator it = this->steps.end(); return (const_iterator&)(it); }
			inline iterator Begin() { return this->steps.begin(); }
			inline iterator End() { return this->steps.end(); }

			inline const_reverse_iterator RBegin() const { std::vector<RenderingStep*>::const_reverse_iterator it = this->steps.rbegin(); return (const_reverse_iterator&)(it); }
			inline const_reverse_iterator REnd() const { std::vector<RenderingStep*>::const_reverse_iterator it = this->steps.rend(); return (const_reverse_iterator&)(it); }
			inline reverse_iterator RBegin() { return this->steps.rbegin(); }
			inline reverse_iterator REnd() { return this->steps.rend(); }

			inline unsigned Size() const { return this->steps.size(); }

			bool AddStep(RenderingStep* step);

			inline unsigned GetRequiredObjectTableSize() const { return this->objectTableSize; };

			inline Size1D GetScaledSize(Size1D size) const { return Size1D(size * this->renderScale); }

			virtual void Serialize(IOStream& stream) const;
		};
	}
}