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

#include <iostream>
#include <list>
#include <queue>
#include <tuple>
#include <math.h>

#include "ggen_support.h"
#include "ggen_erosionsimulator.h"
#include "ggen.h"
#include <windows.h>

double GGen_AngleToRadians(GGen_Height angle){
	return (double)((int32) angle + (int32) GGEN_MAX_HEIGHT) * 3.14 / GGEN_MAX_HEIGHT;
}

double GGen_GetVectorX(GGen_Height angle, double force){
	return cos(GGen_AngleToRadians(angle)) * force;
}

double GGen_GetVectorY(GGen_Height angle, double force){
	return sin(GGen_AngleToRadians(angle)) * force;
}

GGen_ErosionSimulator::GGen_ErosionSimulator(GGen_Size width, GGen_Size height)
	:width(width), height(height), length(width * height)
{}

struct GGen_ErosionPointData{
	GGen_Height height;
	double waterAmount;
	double sedimentAmount;
	double vectorX;
	double vectorY;
};


double* GGen_ErosionSimulator::ImportHeightMap( GGen_Data_2D& heightMap )
{
	double* returnData = new double[this->length];

	for(GGen_Index i = 0; i < this->length; i++){
		returnData[i] = heightMap.data[i];
	}

	return returnData;
}

void GGen_ErosionSimulator::ExportHeightMap( double* heightMap, GGen_Data_2D& ggenHeightMap )
{
	for(GGen_Index i = 0; i < this->length; i++){
		ggenHeightMap.data[i] = (GGen_Height) heightMap[i];
	}
}

double GGen_ErosionSimulator::GetSurfaceTilt(double* heightMap, GGen_Coord x, GGen_Coord y )
{

	GGen_Index indexLeft = x > 0 ? (x - 1) + y * this->width : x + y * this->width;
	GGen_Index indexRight = x + 1 < this->width ? (x + 1) + y * this->width : x + y * this->width;
	GGen_Index indexTop = y > 0 ? x + (y - 1) * this->width : x + y * this->width;
	GGen_Index indexBottom = y + 1 < this->height ? x + (y + 1) * this->width : x + y * this->width;

	double heightLeft = heightMap[indexLeft];
	double heightRight = heightMap[indexRight];
	double heightTop = heightMap[indexTop];
	double heightBottom = heightMap[indexBottom];

	double vectorAX = 2;
	double vectorAY = 0;
	double vectorAZ = (double) heightMap[indexLeft] - (double) heightMap[indexRight];

	double vectorBX = 0;
	double vectorBY = -2;
	double vectorBZ = (double) heightMap[indexTop] - (double) heightMap[indexBottom];

	/* The tile has upwards normal (all the surrounding tiles have the same height) */
	if(vectorAZ == 0 && vectorBZ == 0){
		return GGEN_INVALID_HEIGHT;
	}

	double productX = vectorAY * vectorBZ - vectorAZ * vectorBY;
	//double productY = vectorAZ * vectorBX - vectorAX * vectorBZ;
	double productZ = vectorAX * vectorBY - vectorAY * vectorBX;

	double angle = atan2(productX, productZ);

	return angle;

	//return (GGen_Height) (angle / 3.14159 * GGEN_MAX_HEIGHT);
}

void GGen_ErosionSimulator::Erode(GGen_Data_2D& ggenHeightMap)
{	
	this->deltaT = 0.1;
	this->pipeLength = 1;
	this->graviationalAcceleration = 9.7;
	this->sedimentCapacityConstant = 1;
	this->dissolvingConstant = 1;
	this->depositionConstant = 1;
	this->minimumComputedSurfaceTilt = 0.2;


	double minimumWaterAmount = 0.1;
	double sedimentationThreshold = 1;
	double sedimentCarriedPerUnitOfForce = 0.5;
	double sedimentPickedPerUnitOfForce = 0.05;
	double sedimentReleasedPerUnitOfForce = 0.05;
	double timeMultiplier = 1;

	double* heightMap = ImportHeightMap(ggenHeightMap);

	double* waterMap = new double[this->length];
	double* sedimentMap = new double[this->length];
	GGen_OutflowValues* outflowFluxMap = new GGen_OutflowValues[this->length];	
	//double 	

	// Initialize values in maps to zero where necessary
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			waterMap[x + y * this->width] = 0;
			outflowFluxMap[x + y * this->width].left = 0;
			outflowFluxMap[x + y * this->width].right = 0;
			outflowFluxMap[x + y * this->width].top = 0;
			outflowFluxMap[x + y * this->width].bottom = 0;
			sedimentMap[x + y * this->width] = 0;
		}
	}

	GGen_Data_2D exportMap(this->width, this->height, 0);

	//double* waterMapWithWaterSources = new double[this->length];
	//memcpy(waterMapWithWaterSources, waterMap, this->length * sizeof(double));

	

	int roundCount = 100;
	for(int round = 0; round < roundCount; round++){
		cout << "round " << round << endl;

		GGen_VelocityVector* velocityVectorMap = new GGen_VelocityVector[this->length];

		// Clone the water map.

		this->ApplyWaterSources(waterMap);

		//double* waterMap3 = new double[this->length];
		//memcpy(waterMap3, waterMap, this->length * sizeof(double));
		//delete [] waterMapWithWaterSources;

		//GGen_OutflowValues* outflowFluxMap2 = new GGen_OutflowValues[this->length];
		//memcpy(outflowFluxMap2, outflowFluxMap, this->length * sizeof(GGen_OutflowValues));

		//double* sedimentMap1 = new double[this->heightMap.length];
		//memcpy(sedimentMap1, sedimentMap, this->heightMap.length * sizeof(double));

		this->ApplyFlowSimulation(heightMap, waterMap, sedimentMap, outflowFluxMap, velocityVectorMap);

		//this->ApplyErosion(heightMap, velocityVectorMap, sedimentMap);

		delete [] velocityVectorMap;

		//memcpy(waterMap, waterMap3, this->length * sizeof(double));

		//delete [] waterMap3;

		this->ApplyEvaporation(waterMap);		
	}

	this->ExportHeightMap(sedimentMap, exportMap);
	exportMap.ReturnAs(GGen_Const_String("sedimentMap"));

	this->ExportHeightMap(waterMap, exportMap);
	exportMap.ReturnAs(GGen_Const_String("waterMap"));

	this->ExportHeightMap(heightMap, ggenHeightMap);

	delete [] heightMap;
	delete [] waterMap;
	delete [] sedimentMap;
	delete [] outflowFluxMap;

	
	return;

	// Calculate normal map.
	/*GGen_Data_2D* normalMap = heightMap.Clone();
	normalMap->NormalMap();

	// Build an erosion data array from the rainfall and height maps
	GGen_ErosionPointData* erosionData = new GGen_ErosionPointData[heightMap.length];
	
	for(GGen_Index i = 0; i < heightMap.length; i++){
		erosionData[i].height = heightMap.data[i];
		erosionData->waterAmount = 5;
		erosionData->sedimentAmount = 0;
		erosionData->vectorX = 0;
		erosionData->vectorY = 0;
	}

	// Repeat until the number of tiles with non-zero water value is lower than a set threshold
	uint32 tileWithWaterCount = heightMap.length;
	while(tileWithWaterCount > 0){
		// Create a copy the erosion data array and the height map itself (operations on individual tiles will modify neighboring tiles) 	
		GGen_ErosionPointData* erosionDataCopy = new GGen_ErosionPointData[heightMap.length];		
		memcpy(erosionDataCopy, erosionData, heightMap.length * sizeof(GGen_ErosionPointData));

		// For every tile until the number of tiles with non-zero water value is greater than a threshold number.
		for(GGen_Coord y = 0; y < heightMap.height; y++){
			for(GGen_Coord x = 0; x < heightMap.width; x++){
				GGen_Index tileIndex = x + y * heightMap.width;
				double currentTileFlowForce = sqrt(erosionData[tileIndex].vectorX * erosionData[tileIndex].vectorX + erosionData[tileIndex].vectorY * erosionData[tileIndex].vectorY);

				// Don't bother with the tile if there is no water there.
				if(erosionData[tileIndex].waterAmount < minimumWaterAmount){
					continue;
				}

				// Calculate modification of the flow vector from terrain shape.				
				double terrainVectorX = GGen_GetVectorX(normalMap->data[tileIndex], currentTileFlowForce);
				double terrainVectorY = GGen_GetVectorY(normalMap->data[tileIndex], currentTileFlowForce);
				erosionDataCopy[tileIndex].vectorX += terrainVectorX;
				erosionDataCopy[tileIndex].vectorY += terrainVectorY;

				// Update the flow force (the flow vector has changed when terrain shape modification was applied)
				currentTileFlowForce = sqrt(erosionDataCopy[tileIndex].vectorX * erosionDataCopy[tileIndex].vectorX + erosionData[tileIndex].vectorY * erosionData[tileIndex].vectorY);

				// Calculate the height change from sedimentation/erosion.
				// TODO: A CO PRO 0?????
				double sedimentCapacity = currentTileFlowForce * sedimentCarriedPerUnitOfForce;
				double sedimentChange = 0;
				if(erosionData[tileIndex].sedimentAmount > sedimentCapacity){
					// The flow is carrying too much sediment, some of it must be released
					sedimentChange = MAX(sedimentCapacity - erosionData[tileIndex].sedimentAmount, -currentTileFlowForce * sedimentReleasedPerUnitOfForce);
				}
				else {
					// The flow can carry some more material
					sedimentChange = MIN(sedimentCapacity - erosionData[tileIndex].sedimentAmount, currentTileFlowForce * sedimentPickedPerUnitOfForce);
				}

				erosionData[tileIndex].sedimentAmount += sedimentChange;
				erosionData[tileIndex].height -= sedimentChange;
				
				// Build a list of tiles, in which will the water spread.
				int asd = 5;
				asd += 5;
				queue<int> spreadIndexList();
				//spreadIndexList.back();
				if(currentTileFlowForce == 0){
					//spreadIndexList.push(tileIndex - heightMap.width - 1);
				}

				// Spread the water into all tiles in the spread list.
					// Calculate the amount of water and sediment carried into target tile.
					// Add the water and sediment values to the target tile.
					// Modify the target tile's flow vector using the vector sum formula (significance of each vector is determined by flow amount ratio).
				// Modify the height change from sedimentation/erosion.
			}
		}
	}
	// Apply the height changes from the erosion data array onto the main height array.  				
	// Initialize the wavefront queue.
	// Convert the data from the erosion data array into the wavefront queue
	// Until the wavefront is empty
		// Pretty much the same as above.

	*/
}

void GGen_ErosionSimulator::ApplyWaterSources(double* waterMap)
{
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			waterMap[x + y * this->width] += 1;
		}
	}
}

void GGen_ErosionSimulator::ApplyEvaporation( double* waterMap )
{
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			//waterMap[x + y * this->width] = MAX(0, waterMap[x + y * this->width] - 0.1);
		}
	}
}

void GGen_ErosionSimulator::ApplyFlowSimulation(double* heightMap, double* waterMap, double* sedimentMap, GGen_OutflowValues* outflowFluxMap, GGen_VelocityVector* velocityVectorMap )
{
	double* waterMapCopy  = new double[this->length];
	memcpy(waterMapCopy, waterMap, this->length * sizeof(double));

	double sumWater = 0;
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			sumWater += waterMap[x + this->width * y];
		}
	}

	cout << "Sum water (before): " << (int) sumWater << endl;

	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			GGen_Index currentIndex = x + this->width * y;
			GGen_Height currentHeight = heightMap[x + this->width * y];
			
			// Calculate outflow values for individual directions.
			if(x > 0){
				GGen_Height heightDifference = currentHeight + waterMapCopy[currentIndex] - heightMap[x - 1 + this->width * y] - waterMapCopy[x - 1 + this->width * y];
				double flowValue = outflowFluxMap[currentIndex].left + this->deltaT * (this->graviationalAcceleration * heightDifference) / this->pipeLength;
				outflowFluxMap[currentIndex].left = MAX(0, flowValue);
			}

			if(x + 1 < this->width){
				GGen_Height heightDifference = currentHeight + waterMapCopy[currentIndex] - heightMap[x + 1 + this->width * y] - waterMapCopy[x + 1 + this->width * y];
				double flowValue = outflowFluxMap[currentIndex].right + this->deltaT * (this->graviationalAcceleration * heightDifference) / this->pipeLength;
				outflowFluxMap[currentIndex].right = MAX(0, flowValue);
			}

			if(y > 0){
				GGen_Height heightDifference = currentHeight + waterMapCopy[currentIndex] - heightMap[x + this->width * (y - 1)] - waterMapCopy[x + this->width * (y - 1)];
				double flowValue = outflowFluxMap[currentIndex].top + this->deltaT * (this->graviationalAcceleration * heightDifference) / this->pipeLength;
				outflowFluxMap[currentIndex].top = MAX(0, flowValue);
			}

			if(y + 1 < this->height){
				GGen_Height heightDifference = currentHeight + waterMapCopy[currentIndex] - heightMap[x + this->width * (y + 1)] - waterMapCopy[x + this->width * (y + 1)];
				double flowValue = outflowFluxMap[currentIndex].bottom + this->deltaT * (this->graviationalAcceleration * heightDifference) / this->pipeLength;
				outflowFluxMap[currentIndex].bottom = MAX(0, flowValue);
			}

			// Scale the outflow values so sum(outflow) < amount of water in this tile.
			double sumOutflow = 
				outflowFluxMap[currentIndex].left + 
				outflowFluxMap[currentIndex].right + 
				outflowFluxMap[currentIndex].top + 
				outflowFluxMap[currentIndex].bottom;

			if((int)sumOutflow < 0){
				cout << 'E';
			}

			if(sumOutflow > waterMap[currentIndex]){
				double factor = MIN(1, (waterMap[currentIndex] * this->pipeLength * this->pipeLength) / (sumOutflow * this->deltaT));

				outflowFluxMap[currentIndex].left *= factor;
				outflowFluxMap[currentIndex].right *= factor;
				outflowFluxMap[currentIndex].top *= factor;
				outflowFluxMap[currentIndex].bottom *= factor;

				sumOutflow = 
					outflowFluxMap[currentIndex].left + 
					outflowFluxMap[currentIndex].right + 
					outflowFluxMap[currentIndex].top + 
					outflowFluxMap[currentIndex].bottom;

				if((int)sumOutflow < 0){
					cout << 'E';
				}
			}
		}		
	}

	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			GGen_Index currentIndex = x + this->width * y;

			// Update water level using outflow and inflow information from surrounding cells.
			double sumOutflow = 
				outflowFluxMap[currentIndex].left + 
				outflowFluxMap[currentIndex].right + 
				outflowFluxMap[currentIndex].top + 
				outflowFluxMap[currentIndex].bottom;

			double sumInflow = 0;
			if(x > 0){
				sumInflow += outflowFluxMap[currentIndex - 1].right;
			}

			if(x + 1 < this->width){
				sumInflow += outflowFluxMap[currentIndex + 1].left;
			}

			if(y > 0){
				sumInflow += outflowFluxMap[currentIndex - this->width].bottom;
			}

			if(y + 1 < this->height){
				sumInflow += outflowFluxMap[currentIndex + this->width].top;
			}
		
			double newWaterLevel = waterMap[currentIndex] + (this->deltaT * (sumInflow - sumOutflow)) / (this->pipeLength * this->pipeLength);
			waterMap[currentIndex] = MAX(0, newWaterLevel);

			if((int)waterMap[currentIndex] < 0){
				cout << 'E';
			}

			// The velocity field must be updated

			// Horizontal (x-axis) velocity field vector component
			if(x == 0) {
				// Left border.
				velocityVectorMap[currentIndex].x = 
					(
						outflowFluxMap[x + this->width * y].right -
						outflowFluxMap[x + 1 + this->width * y].left
					);
			}
			else if(x + 1 == this->width)	{
				// Right border.
				velocityVectorMap[currentIndex].x = 
					(
						outflowFluxMap[x - 1 + this->width * y].right -
						outflowFluxMap[x + this->width * y].left
					);
			}
			else {
				// The rest.
				velocityVectorMap[currentIndex].x = 
					(
						outflowFluxMap[x - 1 + this->width * y].right -
						outflowFluxMap[x + this->width * y].left +
						outflowFluxMap[x + this->width * y].right -
						outflowFluxMap[x + 1 + this->width * y].left
					) / 2;
			}

			// Vertical (y-axis) velocity field vector component
			if(y == 0) {
				// Top border.
				velocityVectorMap[currentIndex].y = 
					(
						outflowFluxMap[x + this->width * y].bottom -
						outflowFluxMap[x + 1 + this->width * (y + 1)].top
					);
			}
			else if(y + 1 == this->height)	{
				// Bottom border.
				velocityVectorMap[currentIndex].y = 
					(
						outflowFluxMap[x + this->width * (y - 1)].bottom -
						outflowFluxMap[x + this->width * y].top
					);
			}
			else {
				// The rest.
				velocityVectorMap[currentIndex].y = 
					(
						outflowFluxMap[x + this->width * (y - 1)].bottom -
						outflowFluxMap[x + this->width * y].top +
						outflowFluxMap[x + this->width * y].bottom -
						outflowFluxMap[x + this->width * (y + 1)].top
					) / 2;
			}
			//cout << "Change[" << x <<","<<y<<"] " << (this->deltaT * (sumInflow - sumOutflow)) / (this->pipeLength * this->pipeLength) << endl;
			//Sleep(100);
		}
	}

	sumWater = 0;
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			sumWater += waterMap[x + this->width * y];

			if((int)sumWater < 0){
		cout << 'E';
			}
		}
	}

	if((int)sumWater < 0){
		cout << 'E';
	}

	cout << "Sum water (after): " << (int) sumWater << endl;

	delete [] waterMapCopy;
}

void GGen_ErosionSimulator::ApplyErosion( double* heightMap, GGen_VelocityVector* velocityVectorMap, double* sedimentMap )
{
	double* sedimentMapCopy = new double[this->length];
	memcpy(sedimentMapCopy, sedimentMap, this->length * sizeof(double));

	// Update sediment amount carried by water in the current tile
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			GGen_Index currentIndex = x + this->width * y;

			double surfaceTilt = sin(this->GetSurfaceTilt(heightMap, x, y));
			double sedimentCapacity = this->sedimentCapacityConstant * MAX(0.2, surfaceTilt) * sqrt(velocityVectorMap[currentIndex].x * velocityVectorMap[currentIndex].x + velocityVectorMap[currentIndex].y * velocityVectorMap[currentIndex].y);
			double sedimentToMove = 0;

			//if(sedimentMapCopy[currentIndex] != 0)
			//cout << "Before:" << heightMap[currentIndex];
			//
			double sedimentChange;
			if(sedimentCapacity > sedimentMap[currentIndex]) {
				// The water can carry more sediment - some sediment will be picked up
				sedimentChange = this->dissolvingConstant * (sedimentCapacity - sedimentMapCopy[currentIndex]);
				heightMap[currentIndex] -= sedimentChange;
				sedimentToMove = sedimentMapCopy[currentIndex] + sedimentChange;
			}
			else {
				// The water is over saturated with sediment - some sediment will be deposited
				sedimentChange = this->depositionConstant * (sedimentMapCopy[currentIndex] - sedimentCapacity);
				heightMap[currentIndex] += sedimentChange;
				sedimentToMove = sedimentMapCopy[currentIndex] - sedimentChange;
			}

			//if(sedimentMapCopy[currentIndex] != 0){
			//cout << " After:" << heightMap[currentIndex] << " Capacity:" << sedimentCapacity << " ToMove:" << sedimentToMove << " Change:" << sedimentChange << endl;
			//Sleep(100);}

			// Move the sediment according to the velocity field map
			
			/* Sediment's location will be changed by the velocity vector's coordinates. But since velocity vector
			 * might not be a whole number, the target coordinate likely won't be located on the map grid. The sediment
			 * will be added proportionally to all four surrounding grid points. */ 
			GGen_CoordOffset coordinateOffsetX = (GGen_CoordOffset) floor(velocityVectorMap[currentIndex].x);
			GGen_CoordOffset coordinateOffsetY = (GGen_CoordOffset) floor(velocityVectorMap[currentIndex].y);
			GGen_Coord baseCoordinateX = x + coordinateOffsetX;
			GGen_Coord baseCoordinateY = y + coordinateOffsetY;
			double coordinatePartX = velocityVectorMap[currentIndex].x - coordinateOffsetX;
			double coordinatePartY = velocityVectorMap[currentIndex].y - coordinateOffsetY;			

			// The sediment might be projected outside the map borders. In that case, dump it.
			
			//cout << velocityVectorMap[currentIndex].x << ", " << velocityVectorMap[currentIndex].y << "             " << coordinatePartX << ", " << coordinatePartY << endl;
			//Sleep(200);

			// Top left point.
			if(
				baseCoordinateX < this->width &&
				x > -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y > -coordinateOffsetY
			){
				sedimentMap[baseCoordinateX + this->width * baseCoordinateY] = coordinatePartX * coordinatePartY * sedimentToMove;
			}

			// Top right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x + 1 > -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y > -coordinateOffsetY
				){
					sedimentMap[baseCoordinateX + 1 + this->width * baseCoordinateY] = (1 - coordinatePartX) * coordinatePartY * sedimentToMove;
			}

			// Bottom left point.
			if(
				baseCoordinateX < this->width &&
				x > -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 > -coordinateOffsetY
				){
					sedimentMap[baseCoordinateX + this->width * (baseCoordinateY + 1)] = coordinatePartX * (1 - coordinatePartY) * sedimentToMove;
			}

			// Bottom right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x > -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 > -coordinateOffsetY
				){
					sedimentMap[baseCoordinateX + 1 + this->width * (baseCoordinateY + 1)] = (1 - coordinatePartX) * (1 - coordinatePartY) * sedimentToMove;
			}
		}
	}

	delete [] sedimentMapCopy;
}
