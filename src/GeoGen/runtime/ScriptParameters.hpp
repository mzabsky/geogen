#pragma once

#include <map>
#include <algorithm>

#include "ScriptParameter.hpp" // Necessary to properly compile the owning map.
#include "../utils/OwningMap.hpp"
#include "../random/RandomSeed.hpp"
#include "../Serializable.hpp"
#include "../Rectangle.hpp"
#include "../Point.hpp"

namespace geogen
{	
	namespace runtime
	{
		/// Infinite size in given dimension.
		const unsigned MAP_SIZE_INFINITE = -1;

		/// <summary> The size in given dimension will be set automatically by the generator. </summary>
		const unsigned MAP_SIZE_AUTOMATIC = -2;

		/// Maximum numeric value of a map size in either dimension.
		const unsigned MAP_SIZE_MAX = -3;

		/// Minimum numeric value of a map size in either dimension.
		const unsigned MAP_SIZE_MIN = 1;

		/// The minimum render scale.
		const double RENDER_SCALE_MIN = 0.001;

		/// The maximum render scale.
		const double RENDER_SCALE_MAX = 10;

		/// The default render size (if no other size can be used).
		const Size1D RENDER_SIZE_DEFAULT = 1000;

		/// The default default map size.
		const Size1D MAP_SIZE_DEFAULT = 1000;

		/// Contains configuration of map size, rendering bounds and any additional parameters the script
		/// may have.
		class ScriptParameters : public utils::OwningMap<ScriptParameter>, public Serializable
		{
		private:
			unsigned mapWidth;
			unsigned defaultMapWidth;
			unsigned minMapWidth;
			unsigned maxMapWidth;

			unsigned mapHeight;
			unsigned defaultMapHeight;
			unsigned minMapHeight;
			unsigned maxMapHeight;

			int renderOriginX = 0;
			int renderOriginY = 0;

			unsigned renderWidth = MAP_SIZE_AUTOMATIC;
			unsigned renderHeight = MAP_SIZE_AUTOMATIC;

			double renderScale = 1;

			random::RandomSeed randomSeed;

			std::vector<String> enabledMaps;
		public:
			typedef std::map<String, ScriptParameter const*>::const_iterator const_iterator;
			typedef std::map<String, ScriptParameter*>::iterator iterator;

			/// Initializes a new instance of the ScriptParameters class.
			/// @param defaultMapWidth  The default map width.
			/// @param defaultMapHeight The default map height.
			/// @param minMapWidth	    The minimum map width.
			/// @param minMapHeight	    The minimum map height.
			/// @param maxMapWidth	    The maximum map width.
			/// @param maxMapHeight	    The maximum map height.
			ScriptParameters(unsigned defaultMapWidth, unsigned defaultMapHeight, unsigned minMapWidth, unsigned minMapHeight, unsigned maxMapWidth, unsigned maxMapHeight) :
				mapWidth(MAP_SIZE_AUTOMATIC), mapHeight(MAP_SIZE_AUTOMATIC), defaultMapWidth(defaultMapWidth), defaultMapHeight(defaultMapHeight), minMapWidth(minMapWidth), maxMapHeight(maxMapHeight), maxMapWidth(maxMapWidth), minMapHeight(minMapHeight) {}

			/// Initializes a new instance of the ScriptParameters class.
			ScriptParameters() :
				mapWidth(MAP_SIZE_AUTOMATIC), mapHeight(MAP_SIZE_AUTOMATIC), defaultMapWidth(MAP_SIZE_INFINITE), defaultMapHeight(MAP_SIZE_INFINITE), minMapWidth(MAP_SIZE_MIN), minMapHeight(MAP_SIZE_MIN), maxMapHeight(MAP_SIZE_INFINITE), maxMapWidth(MAP_SIZE_INFINITE) {}

			/// Copy constructor.
			/// @param parameter1 The other instance.
			ScriptParameters(ScriptParameters const&);

			/// Assignment operator.
			/// @param parameter1 The other instance.
			/// @return A deep copy of this ScriptParameters.
			ScriptParameters& operator=(ScriptParameters const&);

			/// Gets current width of the map. If map width wasn't set yet, default width will be returned.
			/// @return Current map width.
			inline unsigned GetMapWidth() const
			{
				if (this->mapWidth == MAP_SIZE_AUTOMATIC)
				{
					if (this->maxMapWidth == MAP_SIZE_INFINITE)
					{
						return MAP_SIZE_INFINITE;
					}
					else
					{
						return Size1D(this->GetRenderWidth() / this->GetRenderScale());
					}
				}
				else
				{
					return this->mapWidth;
				}
			};

			/// Gets the default width of the map.
			/// @return Default width of the map.
			inline unsigned GetDefaultMapWidth() const 
			{ 
				if (this->maxMapWidth == MAP_SIZE_INFINITE)
				{
					return MAP_SIZE_INFINITE;
				}
				else
				{
					return this->defaultMapWidth != MAP_SIZE_AUTOMATIC ? this->defaultMapWidth : MAP_SIZE_DEFAULT;
				}
			};

			/// Gets the minimum width of the map.
			/// @return Minimum width of the map. Always greater than 0.
			inline unsigned GetMinMapWidth() const  { return this->minMapWidth; };

			/// Gets the maximum width of the map.
			/// @return Maximum width of the map. <see>MAP_SIZE_INFINITE</see> if infinite.
			inline unsigned GetMaxMapWidth() const { return this->maxMapWidth; };

			/// Sets the width of the map.
			/// @param width The new width. If MAP_SIZE_AUTOMATIC, default width will be used.
			void SetMapWidth(unsigned width);

			/// Gets current height of the map. If map height wasn't set yet, default height will be returned.
			/// @return Current map height.
			inline unsigned GetMapHeight() const 
			{ 
				if (this->mapHeight == MAP_SIZE_AUTOMATIC)
				{
					if (this->maxMapHeight == MAP_SIZE_INFINITE)
					{
						return MAP_SIZE_INFINITE;
					}
					else 
					{
						return Size1D(this->GetRenderHeight() / this->GetRenderScale());
					}
				}
				else
				{
					return this->mapHeight;
				}
			};

			/// Gets the default height of the map.
			/// @return Default height of the map.
			inline unsigned GetDefaultMapHeight() const 
			{ 
				if (this->maxMapHeight == MAP_SIZE_INFINITE)
				{
					return MAP_SIZE_INFINITE;
				}
				else 
				{
					return this->defaultMapHeight != MAP_SIZE_AUTOMATIC ? this->defaultMapHeight : MAP_SIZE_DEFAULT;
				}
			};

			/// Gets the minimum height of the map.
			/// @return Minimum height of the map. Always greater than 0.
			inline unsigned GetMinMapHeight() const  { return this->minMapHeight; };

			/// Gets the maximum height of the map.
			/// @return Maximum height of the map. <see>MAP_SIZE_INFINITE</see> if infinite.
			inline unsigned GetMaxMapHeight() const { return this->maxMapHeight; };

			/// Sets the height of the map.
			/// @param height The new height. If MAP_SIZE_AUTOMATIC, default height will be used.
			void SetMapHeight(unsigned height);

			/// Gets X coordinate of the render origin.
			/// @return X coordinate of the render origin.
			inline int GetRenderOriginX() const { return this->renderOriginX; };

			/// Sets X coordinate of the render origin.
			/// @param renderOriginX X coordinate of the render origin.
			inline void SetRenderOriginX(int renderOriginX) { this->renderOriginX = renderOriginX; };

			/// Gets Y coordinate of the render origin.
			/// @return Y coordinate of the render origin.
			inline int GetRenderOriginY() const { return this->renderOriginY; };

			/// Sets Y coordinate of the render origin.
			/// @param renderOriginY Y coordinate of the render origin.
			inline void SetRenderOriginY(int renderOriginY) { this->renderOriginY = renderOriginY; };

			/// Gets width of the render.
			/// @return Width of the render.
			inline unsigned GetRenderWidth() const 
			{ 
				return this->renderWidth == MAP_SIZE_AUTOMATIC ? RENDER_SIZE_DEFAULT : this->renderWidth; 
			};

			/// Sets width of the render.
			/// @param renderWidth Width of the render.
			inline void SetRenderWidth(unsigned renderWidth) { this->renderWidth = renderWidth; };

			/// Gets height of the render.
			/// @return Height of the render.
			inline unsigned GetRenderHeight() const 
			{ 
				return this->renderWidth == MAP_SIZE_AUTOMATIC ? RENDER_SIZE_DEFAULT : this->renderHeight;
			};

			/// Sets height of the render.
			/// @param renderHeight Height of the render.
			inline void SetRenderHeight(unsigned renderHeight) { this->renderHeight = renderHeight; };

			/// Sets the render rectangle.
			/// @param renderRect The render rectangle.
			inline void SetRenderRectangle(Rectangle renderRectangle) 
			{ 
				this->SetRenderOriginX(renderRectangle.GetPosition().GetX());
				this->SetRenderOriginY(renderRectangle.GetPosition().GetY());
				this->SetRenderWidth(renderRectangle.GetSize().GetWidth());
				this->SetRenderHeight(renderRectangle.GetSize().GetHeight());
			};

			/// Gets scale of the render.
			/// @return Scale of the render.
			inline double GetRenderScale() const { return this->renderScale; };

			/// Sets scale of the render.
			/// @param renderScale Scale of the render.
			void SetRenderScale(double renderScale);

			/// Gets the random seed.
			/// @return The random seed.
			inline random::RandomSeed GetRandomSeed() const { return this->randomSeed; }

			/// Sets the random seed.
			/// @param randomSeed The random seed.
			inline void SetRandomSeed(random::RandomSeed randomSeed) { this->randomSeed = randomSeed; }

			inline std::vector<String>& GetEnabledMaps() { return this->enabledMaps; }
			inline std::vector<String> const& GetEnabledMaps() const { return this->enabledMaps; }

			/// Resets all parameters to their default values.
			void ResetToDefaults();

			virtual void Serialize(IOStream& stream) const;
		};
	}
}