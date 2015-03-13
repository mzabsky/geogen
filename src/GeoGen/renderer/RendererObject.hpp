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

#include <functional>

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../genlib/DataObject.hpp"

namespace geogen
{
	namespace renderer
	{
		/// Supported types of RendererObject.
		enum RendererObjectType
		{
			/// 1-dimensional map (HeightProfile).
			RENDERER_OBJECT_TYPE_HEIGHT_PROFILE,
		
			/// 2-dimensional map (HeightMap).
			RENDERER_OBJECT_TYPE_HEIGHT_MAP
		};

		/// Converts a RendererObjectType to string.
		/// @param objectType The renderer object type.
		/// @return The string name of the type.
		String RendererObjectTypeToString(RendererObjectType objectType);

		/// Represents an object managed by the renderer. Associates a type label with an instance of genlib::DataObject.
		class RendererObject : public Serializable
		{			
		private:
			genlib::DataObject* ptr;
			RendererObjectType objectType;

			// Non-copyable
			RendererObject(RendererObject const&) {};
			RendererObject& operator=(RendererObject const&) {};
		public:

			/// Initializes a new instance of the RendererObject class.
			/// @param objectType Type of the object.
			/// @param ptr The data object. The renderer object takes ownership of this pointer. It will be released when the renderer object is released.
			RendererObject(RendererObjectType objectType, genlib::DataObject* ptr) : ptr(ptr), objectType(objectType)
			{
			}

			/// Finalizes an instance of the RendererObject class. Releases the wrapped pointer.
			~RendererObject()
			{
				delete ptr;
			}

			/// Gets the wrapped pointer.
			/// @return The wrapped pointer.
			inline genlib::DataObject* GetPtr() { return this->ptr; }

			/// Gets type of the object.
			/// @return The object type.
			inline RendererObjectType GetObjectType() { return this->objectType; }

			/// Writes the human readable string representation of this instance to the given stream.
			/// @param stream The stream.
			virtual void Serialize(IOStream& stream) const;
		};
	}
}