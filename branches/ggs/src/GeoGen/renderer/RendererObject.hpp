#pragma once

#include <functional>

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../genlib/DataObject.hpp"

namespace geogen
{
	namespace renderer
	{
		enum RendererObjectType
		{
			RENDERER_OBJECT_TYPE_HEIGHT_MAP,
			RENDERER_OBJECT_TYPE_HEIGHT_PROFILE
		};

		String RenderObjectTypeToString(RendererObjectType objectType);

		class RendererObject : public Serializable
		{			
		private:
			genlib::DataObject* ptr;
			RendererObjectType objectType;

			// Non-copyable
			RendererObject(RendererObject const&) {};
			RendererObject& operator=(RendererObject const&) {};
		public:
			RendererObject(RendererObjectType objectType, genlib::DataObject* ptr) : ptr(ptr), objectType(objectType)
			{
			}

			~RendererObject()
			{
				delete ptr;
			}

			inline genlib::DataObject* GetPtr() { return this->ptr; }
			inline RendererObjectType GetObjectType() { return this->objectType; }

			virtual void Serialize(IOStream& stream) const
			{
				stream << RenderObjectTypeToString(this->objectType) << "[ " << this->ptr << "]" << std::endl;
			}
		};
	}
}