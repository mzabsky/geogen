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
						filenameStream << GG_STR("object") << number;
						filename = filenameStream.str();
					}

					filename = debugger->GetOutputDirectory() + GG_STR("/") + filename;

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