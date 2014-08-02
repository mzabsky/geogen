#pragma once

#include <map>
#include <algorithm>

#include "ScriptParameter.hpp" // Necessary to properly compile the owning map.
#include "../utils/OwningMap.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{	
	namespace runtime
	{
		class ScriptParameter;

		/// <summary> Infinite size in given dimension. </summary>
		const unsigned MAP_SIZE_INFINITE = -1;

		/// <summary> The size in given dimension will be set automatically by the generator. </summary>
		const unsigned MAP_SIZE_AUTOMATIC = -2;

		/// <summary> Maximum numeric value of a map size in either dimension. </summary>
		const unsigned MAP_SIZE_MAX = -3;

		/// <summary> Minimum numeric value of a map size in either dimension. </summary>
		const unsigned MAP_SIZE_MIN = 1;

		/// <summary> The minimum render scale. </summary>
		const double RENDER_SCALE_MIN = 0.001;

		/// <summary> The maximum render scale. </summary>
		const double RENDER_SCALE_MAX = 10;

		/// <summary>
		/// Contains configuration of map size, rendering bounds and any additional parameters the script may have.
		/// </summary>
		class ScriptParameters : public utils::OwningMap<ScriptParameter>
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
		public:
			typedef std::map<String, ScriptParameter const*>::const_iterator const_iterator;
			typedef std::map<String, ScriptParameter*>::iterator iterator;

			/// <summary>
			/// Initializes a new instance of the <see cref="ScriptParameters"/> class.
			/// </summary>
			ScriptParameters(unsigned defaultMapWidth, unsigned defaultMapHeight, unsigned minMapWidth, unsigned minMapHeight, unsigned maxMapWidth, unsigned maxMapHeight) :
				mapWidth(defaultMapWidth), mapHeight(defaultMapHeight), defaultMapWidth(defaultMapWidth), defaultMapHeight(defaultMapHeight), minMapWidth(minMapWidth), maxMapHeight(maxMapHeight), maxMapWidth(maxMapWidth), minMapHeight(minMapHeight) {}

			/// <summary>
			/// Initializes a new instance of the <see cref="ScriptParameters"/> class.
			/// </summary>
			ScriptParameters() :
				defaultMapWidth(MAP_SIZE_INFINITE), defaultMapHeight(MAP_SIZE_INFINITE), minMapWidth(MAP_SIZE_MIN), minMapHeight(MAP_SIZE_MIN), maxMapHeight(MAP_SIZE_INFINITE), maxMapWidth(MAP_SIZE_INFINITE) {}

			ScriptParameters(ScriptParameters const&);
			ScriptParameters& operator=(ScriptParameters const&);

			/// <summary>
			/// Gets current width of the map. If map width wasn't set yet, default width will be returned.
			/// </summary>
			/// <returns>Current map width.</returns>
			inline unsigned GetMapWidth() const { return this->mapWidth; };

			/// <summary>
			/// Gets the default width of the map.
			/// </summary>
			/// <returns>Default width of the map.</returns>
			inline unsigned GetDefaultMapWidth() const { return this->defaultMapWidth; };

			/// <summary>
			/// Gets the minimum width of the map.
			/// </summary>
			/// <returns>Minimum width of the map. Always greater than 0.</returns>
			inline unsigned GetMinMapWidth() const  { return this->minMapWidth; };

			/// <summary>
			/// Gets the maximum width of the map.
			/// </summary>
			/// <returns>Maximum width of the map. <see>MAP_SIZE_INFINITE</see> if infinite. </returns>
			inline unsigned GetMaxMapWidth() const { return this->maxMapWidth; };

			/// <summary>
			/// Sets the width of the map. 
			/// </summary>
			/// <param name="width">The new width. If <see>MAP_SIZE_AUTOMATIC</see>, default width will be used. </param>
			void SetMapWidth(unsigned width);

			/// <summary>
			/// Gets current height of the map. If map height wasn't set yet, default height will be returned.
			/// </summary>
			/// <returns>Current map height.</returns>
			inline unsigned GetMapHeight() const { return this->mapHeight; };

			/// <summary>
			/// Gets the default height of the map.
			/// </summary>
			/// <returns>Default height of the map.</returns>
			inline unsigned GetDefaultMapHeight() const { return this->defaultMapHeight; };

			/// <summary>
			/// Gets the minimum height of the map.
			/// </summary>
			/// <returns>Minimum height of the map. Always greater than 0.</returns>
			inline unsigned GetMinMapHeight() const  { return this->minMapHeight; };

			/// <summary>
			/// Gets the maximum height of the map.
			/// </summary>
			/// <returns>Maximum height of the map. <see>MAP_SIZE_INFINITE</see> if infinite. </returns>
			inline unsigned GetMaxMapHeight() const { return this->maxMapHeight; };

			/// <summary>
			/// Sets the height of the map. 
			/// </summary>
			/// <param name="height">The new height. If <see>MAP_SIZE_AUTOMATIC</see>, default height will be used. </param>
			void SetMapHeight(unsigned height);

			/// <summary> Gets X coordinate of the render origin. </summary>
			/// <returns> X coordinate of the render origin. </returns>
			inline int GetRenderOriginX() const { return this->renderOriginX; };

			/// <summary> Sets X coordinate of the render origin. </summary>
			/// <param name="renderOriginX"> X coordinate of the render origin. </param>
			inline void SetRenderOriginX(int renderOriginX) { this->renderOriginX = renderOriginX; };

			/// <summary> Gets Y coordinate of the render origin. </summary>
			/// <returns> Y coordinate of the render origin. </returns>
			inline int GetRenderOriginY() const { return this->renderOriginY; };

			/// <summary> Sets Y coordinate of the render origin. </summary>
			/// <param name="renderOriginY"> Y coordinate of the render origin. </param>
			inline void SetRenderOriginY(int renderOriginY) { this->renderOriginY = renderOriginY; };

			/// <summary> Gets width of the render. </summary>
			/// <returns> Width of the render. </returns>
			inline unsigned GetRenderWidth() const { return this->renderWidth; };

			/// <summary> Sets width of the render. </summary>
			/// <param name="renderWidth"> Width of the render. </param>
			inline void SetRenderWidth(unsigned renderWidth) { this->renderWidth = renderWidth; };

			/// <summary> Gets height of the render. </summary>
			/// <returns> Height of the render. </returns>
			inline unsigned GetRenderHeight() const { return this->renderHeight; };

			/// <summary> Sets height of the render. </summary>
			/// <param name="renderHeight"> Height of the render. </param>
			inline void SetRenderHeight(unsigned renderHeight) { this->renderHeight = renderHeight; };

			/// <summary> Gets scale of the render. </summary>
			/// <returns> Scale of the render. </returns>
			inline double GetRenderScale() const { return this->renderScale; };

			/// <summary> Sets scale of the render. </summary>
			/// <param name="renderHeight"> Scale of the render. </param>
			void SetRenderScale(double renderScale);

			inline random::RandomSeed GetRandomSeed() const { return this->randomSeed; }

			inline void SetRandomSeed(random::RandomSeed randomSeed) { this->randomSeed = randomSeed; }

			/// <summary>
			/// Resets all parameters to their default values.
			/// </summary>
			void ResetToDefaults();
		};
	}
}