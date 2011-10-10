/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

/** 
 * @file ggen_erosionsimulator.h GGen_ErosionSimulator encapsulates all erosion-related logic.
 **/

#pragma once

#include "ggen_support.h"
#include "ggen_point.h"
#include "ggen_data_2d.h"
#include <list>

struct GGen_VelocityVector{
	double x;
	double y;
};

struct GGen_OutflowValues{
	double left;
	double right;
	double top;
	double bottom;
};

struct GGen_ThermalWeatheringValues{
    double topLeft;
    double top;
    double topRight;
    double right;
    double bottomRight;
    double bottom;
    double bottomLeft;
    double left;
};

class GGen_ErosionSimulator{
	public:
		GGen_Size width;
		GGen_Size height;
		GGen_TotalSize length;
		double deltaT;
		double pipeLength;
        double pipeCrossectionArea;
		double graviationalAcceleration;
		double sedimentCapacityConstant;
		double dissolvingConstant;
		double depositionConstant;
		double minimumComputedSurfaceTilt;
        double talusAngle;
	
		GGen_ErosionSimulator(GGen_Size width, GGen_Size height);
		double* ImportHeightMap(GGen_Data_2D& heightMap);
		double GetSurfaceTilt(double* heightMap, GGen_Coord x, GGen_Coord y );
		double ExportHeightMap(double* heightMap, GGen_Data_2D& ggenHeightMap);
		void ExportVelocityVectorMap(GGen_VelocityVector* velocityVectorMap, GGen_Data_2D& ggenHeightMap);		
		void ApplyWaterSources(double* waterMap, double waterAmount);
		void ApplyEvaporation(double* waterMap);
		void ApplyFlowSimulation(double* heightMap, double* waterMap, GGen_OutflowValues* outflowFluxMap, GGen_VelocityVector* velocityVectorMap );
		void ApplyErosion(double* heightMap, double* waterMap, GGen_VelocityVector* velocityVectorMap, double* sedimentMap);
        void ApplyThermalWeathering(double* heightMap, double powerMultiplier = 1);
};