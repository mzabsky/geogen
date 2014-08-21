#pragma once

#include <vector>
#include <exception>


#include "../ImageWriter.hpp"
#include "../RendererCommand.hpp"
#include "../RendererDebugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace renderer_commands
		{
			class SaveRendererCommand : public RendererCommand
			{
			public:
				SaveRendererCommand()
				{
					this->cues.push_back(GG_STR("save"));
				}

				virtual String GetName() const { return GG_STR("Save"); };

				virtual String GetHelpString() const { return GG_STR("save [number] [filename] - Saves an object from the renderer object table as PNG image (2D objects) or CSV (1D) to the hard drive (default filename = object[number].png)."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						debugger->GetOut() << GG_STR("Object number must be specified.") << std::endl << std::endl;
						return;
					}

					StringStream ss(arguments);
					unsigned number;
					ss >> number;

					if (ss.fail() || number >= debugger->GetRenderer()->GetObjectTable().GetSize())
					{
						debugger->GetOut() << GG_STR("Invalid renderer object number (use command \"ot\" to list valid object numbers).") << std::endl << std::endl;
						return;
					}

					renderer::RendererObject* object = debugger->GetRenderer()->GetObjectTable().GetObjectBySlot(number);

					if (object == NULL)
					{
						debugger->GetOut() << GG_STR("Renderer object slot ") << number << GG_STR(" is empty.") << std::endl << std::endl;
						return;
					}

					String filename;
					if (!ss.eof())
					{
						ss >> filename;
					}
					else 
					{
						StringStream filenameStream;
						filenameStream << GG_STR("object") << number << (object->GetObjectType() == renderer::RENDERER_OBJECT_TYPE_HEIGHT_MAP ? GG_STR(".png") : GG_STR(".csv"));
						filename = filenameStream.str();
					}

					try
					{
						WriteImage(object->GetPtr(), object->GetObjectType(), filename);
					}
					catch (std::exception&)
					{
						debugger->GetOut() << GG_STR("Could not write \"") << filename << ("\".") << std::endl << std::endl;
						return;
					}
					
					debugger->GetOut() << GG_STR("Saved object ") << number << GG_STR(" as \"") << filename << ("\".") << std::endl << std::endl;
				}
			};
		}
	}
}