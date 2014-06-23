#pragma once

#include <functional>

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../genlib/DataObject.hpp"

namespace geogen
{
	namespace renderer
	{
		/// <summary> Supported types of <see cref="RendererObject"/>. </summary>
		enum RendererObjectType
		{
			/// <summary> 1-dimensional map (profile). </summary>
			RENDERER_OBJECT_TYPE_HEIGHT_PROFILE,
			/// <summary> 2-dimensional map. </summary>
			RENDERER_OBJECT_TYPE_HEIGHT_MAP
		};

		/// <summary> Converts a <see cref="RendererObjectType"/> to string. </summary>
		/// <param name="objectType"> The renderer object type. </param>
		/// <returns> The string name of the type. </returns>
		String RenderObjectTypeToString(RendererObjectType objectType);

		/// <summary> Represents an object managed by the renderer. Associates a type label with an instance of <see cref="DataObject"/>.</summary>
		class RendererObject : public Serializable
		{			
		private:
			genlib::DataObject* ptr;
			RendererObjectType objectType;

			// Non-copyable
			RendererObject(RendererObject const&) {};
			RendererObject& operator=(RendererObject const&) {};
		public:

			/// <summary> Initializes a new instance of the RendererObject class. </summary>
			/// <param name="objectType"> Type of the object. </param>
			/// <param name="ptr"> The data object. The renderer object takes ownership of this pointer. It will be released when the renderer object is released. </param>
			RendererObject(RendererObjectType objectType, genlib::DataObject* ptr) : ptr(ptr), objectType(objectType)
			{
			}

			/// <summary> Finalizes an instance of the RendererObject class. Releases the wrapped pointer. </summary>
			~RendererObject()
			{
				delete ptr;
			}

			/// <summary> Gets the wrapped pointer. </summary>
			/// <returns> The wrapped pointer. </returns>
			inline genlib::DataObject* GetPtr() { return this->ptr; }

			/// <summary> Gets type of the object. </summary>
			/// <returns> The object type. </returns>
			inline RendererObjectType GetObjectType() { return this->objectType; }

			/// <summary> Writes the human readable string representation of this instance to the given stream. </summary>
			/// <param name="stream"> The stream. </param>
			virtual void Serialize(IOStream& stream) const
			{
				stream << RenderObjectTypeToString(this->objectType) << "[ " << this->ptr << "]" << std::endl;
			}
		};
	}
}