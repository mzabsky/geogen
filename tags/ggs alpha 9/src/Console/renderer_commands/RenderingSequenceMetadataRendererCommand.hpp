#pragma once

#include <vector>

#include "../RendererCommand.hpp"
#include "../RendererDebugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace renderer_commands
		{
			class RenderingSequenceMetadataRendererCommand : public RendererCommand
			{
			public:
				RenderingSequenceMetadataRendererCommand()
				{
					this->cues.push_back(GG_STR("rsm"));
					this->cues.push_back(GG_STR("seqm"));
					this->cues.push_back(GG_STR("seqmeta"));
					this->cues.push_back(GG_STR("rseqm"));
					this->cues.push_back(GG_STR("rseqmeta"));
					this->cues.push_back(GG_STR("renseqmeta"));
					this->cues.push_back(GG_STR("renseqmetadata"));
					this->cues.push_back(GG_STR("renderingsequencemetadata"));
				}

				virtual String GetName() const { return GG_STR("Rendering sequence metadata"); };

				virtual String GetHelpString() const { return GG_STR("rsm - Displays the rendering sequence metadata."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Rendering sequence metadata (scale ") << debugger->GetRenderer()->GetRenderingSequence().GetRenderScale() << GG_STR("):") << std::endl;

					for (renderer::RenderingSequence::const_iterator it = debugger->GetRenderer()->GetRenderingSequence().Begin(); it != debugger->GetRenderer()->GetRenderingSequence().End(); it++)
					{
						debugger->GetOut() << (*it)->ToString() << std::endl;
						debugger->GetOut() << GG_STR("\tBounds: ") << debugger->GetRenderer()->GetRenderingSequenceMetadata().GetRenderingBounds(*it)->ToString() << std::endl;

						debugger->GetOut() << GG_STR("\tObjects to release: ");

						std::vector<unsigned> const& objectIndexesToRelease = debugger->GetRenderer()->GetRenderingSequenceMetadata().GetObjectIndexesToRelease(*it);
						for (std::vector<unsigned>::const_iterator it2 = objectIndexesToRelease.begin(); it2 != objectIndexesToRelease.end(); it2++)
						{
							debugger->GetOut() << *it2;

							if (it2 + 1 != objectIndexesToRelease.end())
							{
								debugger->GetOut() << GG_STR(", ");
							}
						}

						debugger->GetOut() << std::endl;
					}

					debugger->GetOut() << std::endl;
				}
			};
		}
	}
}