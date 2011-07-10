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
#include <sstream>

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
        if(heightMap.data[i] < 0) {
            cout << "E";
        }

		returnData[i] = (double) heightMap.data[i] * 100. / (double) GGEN_MAX_HEIGHT;
	}

	return returnData;
}

void GGen_ErosionSimulator::ExportHeightMap( double* heightMap, GGen_Data_2D& ggenHeightMap )
{
	double max = 0;
	double min = 1000000000;

	for(GGen_Index i = 0; i < this->length; i++){
		if(heightMap[i] > max) max  = heightMap[i];
		if(heightMap[i] < min) min  = heightMap[i];
	}

	cout << "Min << " << min << "  ---- MAX: " << max << endl;

	for(GGen_Index i = 0; i < this->length; i++){
		ggenHeightMap.data[i] = (GGen_Height) (heightMap[i] * GGEN_MAX_HEIGHT / max);
	}
}

void GGen_ErosionSimulator::ExportVelocityVectorMap( GGen_VelocityVector* velocityVectorMap, GGen_Data_2D& ggenHeightMap )
{
	for(GGen_Index i = 0; i < this->length; i++){
		ggenHeightMap.data[i] = (GGen_Height) sqrt(velocityVectorMap[i].x * velocityVectorMap[i].x + velocityVectorMap[i].y * velocityVectorMap[i].y);
	}
}


double GGen_ErosionSimulator::GetSurfaceTilt(double* heightMap, GGen_Coord x, GGen_Coord y )
{
	/*if(x == 0 || x + 1 == this->width || y == 0 || y + 1 == this->width) return 0;

	double tilt = 
	MAX(
		ABS(heightMap[x + y * this->width - 1] - heightMap[x + y * this->width + 1]), 
		ABS(heightMap[x + y * this->width - this->width] - heightMap[x + y * this->width + this->width])
	);

	return tilt;*/
	
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
	/*if(vectorAZ == 0 && vectorBZ == 0){
		return GGEN_INVALID_HEIGHT;
	}*/

	double productX = vectorAY * vectorBZ - vectorAZ * vectorBY;
	double productY = vectorAZ * vectorBX - vectorAX * vectorBZ;
	double productZ = vectorAX * vectorBY - vectorAY * vectorBX;

	double angle = acos(sqrt(productX * productX + productY * productY) / sqrt(productX * productX + productY * productY + productZ * productZ)); //(ABS(productX) + ABS(productY)) / 2; //ABS(sin(productZ));

	if(angle > 3.15){
		cout << "E";
	}

	return angle;

	//return (GGen_Height) (angle / 3.14159 * GGEN_MAX_HEIGHT);
}

void GGen_ErosionSimulator::Erode(GGen_Data_2D& ggenHeightMap)
{	
	this->deltaT = 0.005;
	this->pipeLength = 1;
	this->graviationalAcceleration = 9.7;
	this->sedimentCapacityConstant = 5;
	this->dissolvingConstant = 0.2;
	this->depositionConstant = 0.2;
	this->minimumComputedSurfaceTilt = 0.2;

	double* heightMap = ImportHeightMap(ggenHeightMap);
	ExportHeightMap(heightMap, ggenHeightMap);
	ggenHeightMap.ReturnAs(GGen_Const_String("imported"));

	double* originalMap = new double[this->length];
	memcpy(originalMap, heightMap, this->length * sizeof(double));

	double* waterMap = new double[this->length];
	double* sedimentMap = new double[this->length];
	GGen_OutflowValues* outflowFluxMap = new GGen_OutflowValues[this->length];	
	//double 	

	double* tiltMap = new double[this->length];

	// Initialize values in maps to zero where necessary
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			GGen_Index currentIndex = x + y * this->width;
			
			waterMap[currentIndex] = heightMap > 0 ? 0 : -heightMap[currentIndex];
			outflowFluxMap[currentIndex].left = 0;
			outflowFluxMap[currentIndex].right = 0;
			outflowFluxMap[currentIndex].top = 0;
			outflowFluxMap[currentIndex].bottom = 0;
			sedimentMap[currentIndex] = 0;
			tiltMap[currentIndex] = 1 - sin(this->GetSurfaceTilt(heightMap, x, y));

            if(heightMap[x + y * this->width] < 0){
                cout << "E";
            }
		}
	}

	GGen_Data_2D exportMap(this->width, this->height, 0);

	ExportHeightMap(tiltMap, exportMap); exportMap.ReturnAs(GGen_Const_String("tilt"));


	
	double sum = 0;
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			sum += heightMap[x + this->width * y];			
		}
	}

	cout << "Initial Average Height " << (sum / this->length) << endl;

	int roundCount = 2500;
	for(int round = 0; round < roundCount; round++){
		cout << "round " << round << endl;

		GGen_VelocityVector* velocityVectorMap = new GGen_VelocityVector[this->length];

		// Clone the water map.

		if((round % 10 == 0) && (round < roundCount / 2)) {
			this->ApplyWaterSources(waterMap);
			cout << "W";
		}

		//double* waterMap3 = new double[this->length];
		//memcpy(waterMap3, waterMap, this->length * sizeof(double));
		//delete [] waterMapWithWaterSources;

		//GGen_OutflowValues* outflowFluxMap2 = new GGen_OutflowValues[this->length];
		//memcpy(outflowFluxMap2, outflowFluxMap, this->length * sizeof(GGen_OutflowValues));

		//double* sedimentMap1 = new double[this->heightMap.length];
		//memcpy(sedimentMap1, sedimentMap, this->heightMap.length * sizeof(double));


		this->ApplyFlowSimulation(heightMap, waterMap, outflowFluxMap, velocityVectorMap);
		
        this->ExportHeightMap(waterMap, exportMap);
        wstringstream ss;
        ss << GGen_Const_String("waterMapProgress");
        ss << round;

	    exportMap.ReturnAs(ss.str());

        //if(round > round / 2){
            this->ApplyErosion(heightMap, velocityVectorMap, sedimentMap);
        //}

		delete [] velocityVectorMap;

		//memcpy(waterMap, waterMap3, this->length * sizeof(double));

		//delete [] waterMap3;

		this->ApplyEvaporation(waterMap);	


        this->ExportHeightMap(heightMap, exportMap);
        wstringstream ss2;
        ss2 << GGen_Const_String("heightMapProgress");
        ss2 << round;

        exportMap.ReturnAs(ss2.str());

		double heightSum = 0;
		double sedimentSum = 0;
		for(unsigned i = 0; i < this->length; i++){
			if(heightMap[i] > GGEN_MAX_HEIGHT || heightMap[i] < GGEN_MIN_HEIGHT){
				cout << "E";
			}


			heightSum += heightMap[i];
			sedimentSum += sedimentMap[i];			
		}

		cout << ((heightSum + sedimentSum) / this->length) << " = " << heightSum / this->length << " + " << sedimentSum / this->length << endl;
	}

	this->ExportHeightMap(sedimentMap, exportMap);
	exportMap.ReturnAs(GGen_Const_String("sedimentMap"));

	this->ExportHeightMap(waterMap, exportMap);
	exportMap.ReturnAs(GGen_Const_String("waterMap"));

	this->ExportHeightMap(waterMap, ggenHeightMap);
	ggenHeightMap.ReturnAs(GGen_Const_String("eroded"));

	double* addedMap = new double[this->length];
	double* removedMap = new double[this->length];

     

	//ggenHeightMap.AddMap(&exportMap);

	for(GGen_Index i = 0; i < this->length; i++){
		addedMap[i] = originalMap[i] < heightMap[i] ? heightMap[i] - originalMap[i] : 0;
		removedMap[i] = originalMap[i] > heightMap[i] ? originalMap[i] - heightMap[i] : 0;
	}

	ExportHeightMap(addedMap, exportMap);
	exportMap.ReturnAs(GGen_Const_String("added"));

	ExportHeightMap(removedMap, exportMap);
	exportMap.ReturnAs(GGen_Const_String("removed"));

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
            //if(x % 20  == 0 || y % 20 == 0){
            waterMap[x + y * this->width] += 1.1 * this->deltaT;
            //}
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

void GGen_ErosionSimulator::ApplyFlowSimulation(double* heightMap, double* waterMap, GGen_OutflowValues* outflowFluxMap, GGen_VelocityVector* velocityVectorMap )
{
	double* waterMapCopy  = new double[this->length];
	memcpy(waterMapCopy, waterMap, this->length * sizeof(double));

	double sumWater = 0;
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			sumWater += waterMap[x + this->width * y];
		}
	}

	//cout << "Sum water (before): " << (int) sumWater << endl;

	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			GGen_Index currentIndex = x + this->width * y;
			double currentHeight = heightMap[x + this->width * y];
			
            if(outflowFluxMap[currentIndex].left < 0 || outflowFluxMap[currentIndex].right < 0 || outflowFluxMap[currentIndex].top < 0 || outflowFluxMap[currentIndex].bottom < 0){
                cout << "E";
            }

			// Calculate outflow values for individual directions.
			if(x > 0){
				double heightDifference = currentHeight + waterMapCopy[currentIndex] - heightMap[currentIndex - 1] - waterMapCopy[currentIndex - 1];
				double flowValue = outflowFluxMap[currentIndex].left + this->deltaT * (this->graviationalAcceleration * heightDifference) / this->pipeLength;
				outflowFluxMap[currentIndex].left = MAX(0, flowValue);
			}

			if(x + 1 < this->width){
				double heightDifference = currentHeight + waterMapCopy[currentIndex] - heightMap[currentIndex + 1] - waterMapCopy[currentIndex + 1];
				double flowValue = outflowFluxMap[currentIndex].right + this->deltaT * (this->graviationalAcceleration * heightDifference) / this->pipeLength;
				outflowFluxMap[currentIndex].right = MAX(0, flowValue);
			}

			if(y > 0){
                double heightDifference = currentHeight + waterMapCopy[currentIndex] - heightMap[currentIndex - this->width] - waterMapCopy[currentIndex - this->width];
				double flowValue = outflowFluxMap[currentIndex].top + this->deltaT * (this->graviationalAcceleration * heightDifference) / this->pipeLength;
				outflowFluxMap[currentIndex].top = MAX(0, flowValue);
			}

			if(y + 1 < this->height){
                double heightDifference = currentHeight + waterMapCopy[currentIndex] - heightMap[currentIndex + this->width] - waterMapCopy[currentIndex + this->width];
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
                double factor = MIN(1, waterMap[currentIndex] * this->pipeLength * this->pipeLength / sumOutflow * this->deltaT);

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

    /*GGen_OutflowValues* outflowFluxMapCopy  = new GGen_OutflowValues[this->length];
    memcpy(outflowFluxMapCopy, outflowFluxMap, this->length * sizeof(GGen_OutflowValues));

    for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
            GGen_Index currentIndex = x + this->width * y;
            double currentHeight = heightMap[currentIndex];
            
            // Update water level using outflow and inflow information from surrounding cells.
			double sumOutflow = 
				outflowFluxMapCopy[currentIndex].left + 
				outflowFluxMapCopy[currentIndex].right + 
				outflowFluxMapCopy[currentIndex].top + 
				outflowFluxMapCopy[currentIndex].bottom;

			double sumInflow = 0;
			if(x > 0){
				sumInflow += outflowFluxMapCopy[currentIndex - 1].right;
			}

			if(x + 1 < this->width){
				sumInflow += outflowFluxMapCopy[currentIndex + 1].left;
			}

			if(y > 0){
				sumInflow += outflowFluxMapCopy[currentIndex - this->width].bottom;
			}

			if(y + 1 < this->height){
				sumInflow += outflowFluxMapCopy[currentIndex + this->width].top;
			}
		
            double oldWaterLevel =  waterMap[currentIndex];
			double newWaterLevel = waterMap[currentIndex] + (this->deltaT * (sumInflow - sumOutflow)) / (this->pipeLength * this->pipeLength);

            double leftContributionOverflow = 0;
            double rightContributionOverflow = 0;
            double topContributionOverflow = 0;
            double bottomContributionOverflow = 0;

            if(x > 0 && outflowFluxMapCopy[currentIndex - 1].right > 0){
                leftContributionOverflow = currentHeight + newWaterLevel - heightMap[currentIndex - 1] - waterMap[currentIndex - 1];
                leftContributionOverflow = MAX(0, leftContributionOverflow);
            }

            if(x < this->width - 1 && outflowFluxMapCopy[currentIndex + 1].left > 0){
                rightContributionOverflow = currentHeight + newWaterLevel - heightMap[currentIndex + 1] - waterMap[currentIndex + 1];
                rightContributionOverflow = MAX(0, rightContributionOverflow);
            }

            if(y > 0 && outflowFluxMapCopy[currentIndex - this->width].bottom > 0){
                topContributionOverflow = currentHeight + newWaterLevel - heightMap[currentIndex - this->width] - waterMap[currentIndex - this->width];
                topContributionOverflow = MAX(0, topContributionOverflow);
            }

            if(y < this->height - 1 && outflowFluxMapCopy[currentIndex + this->width].top > 0){
                bottomContributionOverflow = currentHeight + newWaterLevel - heightMap[currentIndex + this->width] - waterMap[currentIndex + this->width];
                bottomContributionOverflow = MAX(0, bottomContributionOverflow);
            }

            double totalContributionOverflow = 
                leftContributionOverflow + 
                rightContributionOverflow +
                topContributionOverflow + 
                bottomContributionOverflow;

            double sumInflow2 = 0;
			if(x > 0 && leftContributionOverflow > 0){
				sumInflow2 += outflowFluxMapCopy[currentIndex - 1].right;
			}

			if(x + 1 < this->width && rightContributionOverflow > 0){
				sumInflow2 += outflowFluxMapCopy[currentIndex + 1].left;
			}

			if(y > 0 && topContributionOverflow > 0){
				sumInflow2 += outflowFluxMapCopy[currentIndex - this->width].bottom;
			}

			if(y + 1 < this->height && bottomContributionOverflow > 0){
				sumInflow2 += outflowFluxMapCopy[currentIndex + this->width].top;
			}

            double factor = 1 - (sumInflow2 != 0 ? (totalContributionOverflow / sumInflow2) : 0);

            //factor = MIN(1, MAX(factor, 0));
            factor = MAX(factor, 0);

            if(factor > 1 || factor < 0){
                cout << "E";
            }

            if(factor != 1 && factor != 0){
                //cout << "R";
            }


            if(factor > 0){
                if(x > 0 && leftContributionOverflow > 0){
				    outflowFluxMap[currentIndex - 1].right *= factor; // sem taky outflowFluxMapCopy?
			    }

			    if(x + 1 < this->width && rightContributionOverflow > 0){
				    outflowFluxMap[currentIndex + 1].left *= factor;
			    }

			    if(y > 0 && topContributionOverflow > 0){
				    outflowFluxMap[currentIndex - this->width].bottom *= factor;
			    }

			    if(y + 1 < this->height && bottomContributionOverflow > 0){
				    outflowFluxMap[currentIndex + this->width].top *= factor;
			    }
            }
            else{
                if(x > 0 && leftContributionOverflow > 0){
				    outflowFluxMap[currentIndex].left += outflowFluxMap[currentIndex - 1].right * factor * -1; // sem taky outflowFluxMapCopy?
                    outflowFluxMap[currentIndex - 1].right = 0;
			    }

			    if(x + 1 < this->width && rightContributionOverflow > 0){
				    outflowFluxMap[currentIndex].right += outflowFluxMap[currentIndex + 1].left * factor * -1;
                    outflowFluxMap[currentIndex + 1].left = 0;
			    }

			    if(y > 0 && topContributionOverflow > 0){
				    outflowFluxMap[currentIndex].top += outflowFluxMap[currentIndex - this->width].bottom * factor * -1;
                    outflowFluxMap[currentIndex - this->width].bottom = 0;
			    }

			    if(y + 1 < this->height && bottomContributionOverflow > 0){
				    outflowFluxMap[currentIndex].bottom += outflowFluxMap[currentIndex + this->width].top * factor * -1;
                    outflowFluxMap[currentIndex + this->width].top = 0;
			    }
            }

            if(factor != 1){
                //cout << "R";
            }

   //         if(x > 0){
			//	outflowFluxMap[currentIndex - 1].right *= factor;
			//}

			//if(x + 1 < this->width){
			//	outflowFluxMap[currentIndex + 1].left *= factor;
			//}

			//if(y > 0){
			//	outflowFluxMap[currentIndex - this->width].bottom *= factor;
			//}

			//if(y + 1 < this->height){
			//	outflowFluxMap[currentIndex + this->width].top *= factor;
			//}
        }
    }

    delete [] outflowFluxMapCopy;*/

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
		
            double oldWaterLevel =  waterMapCopy[currentIndex];
			double newWaterLevel = waterMapCopy[currentIndex] + (this->deltaT * (sumInflow - sumOutflow)) / (this->pipeLength * this->pipeLength);

            if(oldWaterLevel - newWaterLevel != 0){
                //cout << "A";
            }

			waterMap[currentIndex] = MAX(0, newWaterLevel);
            double waterAverage = (oldWaterLevel + waterMap[currentIndex]) / 2;

			if((int)waterMap[currentIndex] < 0){
				cout << 'E';
			}

			// The velocity field must be updated


            // TODO: PROVÌØIT - ZDE MUZE PRAMENIT AXIAL BIAS

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

            velocityVectorMap[x + width * y].x /= waterAverage * this->pipeLength;
            velocityVectorMap[x + width * y].y /= waterAverage * this->pipeLength;

			//cout << "Change[" << x <<","<<y<<"] " << (this->deltaT * (sumInflow - sumOutflow)) / (this->pipeLength * this->pipeLength) << endl;
			//Sleep(100);
		}
	}

	sumWater = 0;
	double sumVelocity = 0;
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			sumWater += waterMap[x + this->width * y];
			sumVelocity += sqrt(velocityVectorMap[x + y * this->width].x * velocityVectorMap[x + y * this->width].x + velocityVectorMap[x + y * this->width].y * velocityVectorMap[x + y * this->width].y);

			if((int)sumWater < 0){
		cout << 'E';
			}
		}
	}

	sumVelocity /= this->length;

	if((int)sumWater < 0){
		cout << 'E';
	}

	//cout << "Sum water (after): " << (int) sumWater << endl;
	//cout << "Average velocity (after): " << sumVelocity << endl;

	delete [] waterMapCopy;
}

void GGen_ErosionSimulator::ApplyErosion( double* heightMap, GGen_VelocityVector* velocityVectorMap, double* sedimentMap )
{
	//double* sedimentMapCopy = new double[this->length];
	//memcpy(sedimentMapCopy, sedimentMap, this->length * sizeof(double));

	double* sedimentToMoveMap = new double[this->length];

	/*for(GGen_Index i = 0; i < this->length; i++){
		sedimentMap[i] = 0;
	}*/

	// Update sediment amount carried by water in the current tile
	cout << "capacities: ";
	double capacitySum = 0;
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			GGen_Index currentIndex = x + this->width * y;
			//GGen_VelocityVector currentVelocityVector = velocityVectorMap[currentIndex];

			//double velocityVectorLength = sqrt(currentVelocityVector.x + currentVelocityVector.x + currentVelocityVector.y + currentVelocityVector.y);

			/* Sediment's location will be changed by the velocity vector's coordinates. But since velocity vector
			 * might not be a whole number, the target coordinate likely won't be located on the map grid. The sediment
			 * will be added proportionally to all four surrounding grid points. */ 

            GGen_VelocityVector currentVelocityVector = velocityVectorMap[currentIndex];

            //GGen_Script_Assert(currentVelocityVector.x < 1 && currentVelocityVector.x > -1 && currentVelocityVector.y < 1 && currentVelocityVector.y > -1)

            GGen_CoordOffset coordinateOffsetX = currentVelocityVector.x < 0 ? -1 : 0;//floor(currentVelocityVector.x * this->deltaT);//currentVelocityVector.x > 0 ? (GGen_CoordOffset) floor(currentVelocityVector.x * this->deltaT) : (GGen_CoordOffset) ceil(currentVelocityVector.x * this->deltaT);
            GGen_CoordOffset coordinateOffsetY = currentVelocityVector.y < 0 ? -1 : 0;//floor(currentVelocityVector.y * this->deltaT);//currentVelocityVector.y > 0 ? (GGen_CoordOffset) floor(currentVelocityVector.y * this->deltaT) : (GGen_CoordOffset) ceil(currentVelocityVector.y * this->deltaT);
            GGen_CoordOffset baseCoordinateX = x + coordinateOffsetX;
            GGen_CoordOffset baseCoordinateY = y + coordinateOffsetY;
            double coordinatePartX = currentVelocityVector.x * this->deltaT - coordinateOffsetX;
            double coordinatePartY = currentVelocityVector.y * this->deltaT - coordinateOffsetY;	


			if(coordinatePartX < 0){
				coordinatePartX += 1;
			}

			if(coordinatePartY < 0){
				coordinatePartY += 1;
			}

			/*if(coordinatePartX > 1 || coordinatePartY > 1 || coordinatePartX < 0 || coordinatePartY < 0){
				cout << "E";
			}*/

			// Calculate current height of the target point (it will likely to be interpolated from 4 surrounding points).
			
			// Top left point.
			double heightSum = 0;
			double heightWeightSum = 0;
			if(
                baseCoordinateX < this->width &&
                x >= -coordinateOffsetX &&
                baseCoordinateY < this->height &&
                y >= -coordinateOffsetY
                ){
                    double currentWeight = coordinatePartX * coordinatePartY;
					heightSum += heightMap[baseCoordinateX + this->width * baseCoordinateY];
					heightWeightSum += currentWeight;
			}

			// Top right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x + 1 >= -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y >= -coordinateOffsetY
				){
                    double currentWeight = (1 - coordinatePartX) * coordinatePartY;
					heightSum += heightMap[baseCoordinateX + 1 + this->width * baseCoordinateY];
					heightWeightSum += currentWeight;
			}

			// Bottom left point.
			if(
				baseCoordinateX < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 > -coordinateOffsetY
				){
                    double currentWeight = coordinatePartX * (1 - coordinatePartY);
					heightSum += heightMap[baseCoordinateX + this->width * (baseCoordinateY + 1)];
					heightWeightSum += currentWeight;
			}

			// Bottom right point.
			if(
                baseCoordinateX + 1 < this->width &&
                x + 1 >= -coordinateOffsetX &&
                baseCoordinateY + 1 < this->height &&
                y + 1 >= -coordinateOffsetY
				){
                    double currentWeight = (1 - coordinatePartX) * (1 - coordinatePartY);
					heightSum += heightMap[baseCoordinateX + 1 + this->width * (baseCoordinateY + 1)];
					heightWeightSum += currentWeight;
			}

            // TODO: KOREKTNI VZOREC PRO VAZENY PRUMER

			/*if(heightWeightSum == 0){
				// 
				continue;
			}*/

			if(heightWeightSum == 0){
				cout << "e";
			}

			double targetPointHeightDiff = heightSum / heightWeightSum - heightMap[currentIndex];
			//double targetPointDistance = sqrt(coordinatePartX * coordinatePartX + coordinatePartY * coordinatePartY);


            // VYPLEPSIT ANGLE CALCULATION - ABY BRALA VEKTOR V POTAZ
            double velocityVectorLength = sqrt(velocityVectorMap[currentIndex].x * velocityVectorMap[currentIndex].x + velocityVectorMap[currentIndex].y * velocityVectorMap[currentIndex].y) * this->deltaT;

            

			double surfaceTilt;
            


            if(velocityVectorLength > 0){
                surfaceTilt = atan(targetPointHeightDiff / velocityVectorLength);//1 - sin(this->GetSurfaceTilt(heightMap, x, y));
            }
            else {
                surfaceTilt = 0;
            }

            surfaceTilt = 1;

			double sedimentCapacity = this->sedimentCapacityConstant * MAX(0.2, surfaceTilt) * velocityVectorLength;
			double sedimentToMove = 0;

            if(x == 199 && y == 0){
                cout << "e";
            }

            //cout << sedimentCapacity << endl;

            if(surfaceTilt < -4 || surfaceTilt > 4){
                cout << "e";
            }

            if(sedimentCapacity < -10 ||sedimentCapacity > 10){
                cout << "e";
            }

			if(sedimentCapacity < 0){
				cout << "e";
			}

			capacitySum += sedimentCapacity;

            if(capacitySum < 0){
                cout << "e";
            } 

            if(capacitySum != capacitySum){
                cout << "e";
            }

			if(sedimentMap[currentIndex] == 0 &&  sedimentCapacity == 0) {
				sedimentToMoveMap[currentIndex] = 0;
				continue;
			};

			double originalHeight = heightMap[currentIndex];

			if(sedimentMap[x + this->width * y] < 0){
				cout << "E";
			}

			//if(sedimentMapCopy[currentIndex] != 0)
			//cout << "Before:" << heightMap[currentIndex];
			//

			/*if (sedimentMap[currentIndex] != 0) {
 				cout << "e";
			}*/

			//if(sedimentChange < )

			double sedimentChange;
			if(sedimentCapacity > sedimentMap[currentIndex]) {
				// The water can carry more sediment - some sediment will be picked up
				sedimentChange = this->dissolvingConstant * (sedimentCapacity - sedimentMap[currentIndex]);

				if(sedimentChange > 10){
					cout << "e";
				}

				heightMap[currentIndex] -= sedimentChange;
				sedimentToMove = sedimentMap[currentIndex] + sedimentChange;
			}
			else if(sedimentCapacity < sedimentMap[currentIndex]){
				// The water is over saturated with sediment - some sediment will be deposited
				sedimentChange = this->depositionConstant * (sedimentMap[currentIndex] - sedimentCapacity);

				if(sedimentChange > 10){
					cout << "e";
				}

				heightMap[currentIndex] += sedimentChange;

				sedimentToMove = sedimentMap[currentIndex] - sedimentChange;
			}

			if(sedimentToMove < 0){
				cout << "E";
			}

			sedimentToMoveMap[currentIndex] = sedimentToMove;
		}
	}

	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			GGen_Index currentIndex = x + this->width * y;
			GGen_VelocityVector currentVelocityVector = velocityVectorMap[currentIndex];
			//if(sedimentMapCopy[currentIndex] != 0){
			//cout << " After:" << heightMap[currentIndex] << " Capacity:" << sedimentCapacity << " ToMove:" << sedimentToMove << " Change:" << sedimentChange << endl;
			//Sleep(100);}

			// Move the sediment according to the velocity field map
			

			if(sedimentToMoveMap[currentIndex] == 0) continue;			

			// we are doing a step backwards in time, so inverse vector has to be used
			currentVelocityVector.x *= -1;
			currentVelocityVector.y *= -1;

			GGen_CoordOffset coordinateOffsetX = (GGen_CoordOffset) floor(currentVelocityVector.x * this->deltaT);//currentVelocityVector.x > 0 ? (GGen_CoordOffset) floor(currentVelocityVector.x * this->deltaT) : (GGen_CoordOffset) ceil(currentVelocityVector.x * this->deltaT);
			GGen_CoordOffset coordinateOffsetY = (GGen_CoordOffset) floor(currentVelocityVector.y * this->deltaT); //currentVelocityVector.y > 0 ? (GGen_CoordOffset) floor(currentVelocityVector.y * this->deltaT) : (GGen_CoordOffset) ceil(currentVelocityVector.y * this->deltaT);
			GGen_Coord baseCoordinateX = x - coordinateOffsetX;
			GGen_Coord baseCoordinateY = y - coordinateOffsetY;
			double coordinatePartX = currentVelocityVector.x * this->deltaT - coordinateOffsetX;
			double coordinatePartY = currentVelocityVector.y * this->deltaT - coordinateOffsetY;	

			/*if(coordinatePartX < 0){
				coordinatePartX += 1;
			}

			if(coordinatePartY < 0){
				coordinatePartY += 1;
			}*/

			if(coordinatePartX > 1 || coordinatePartY > 1 || coordinatePartX < 0 || coordinatePartY < 0){
				cout << "E";
			}

			// Calculate current height of the target point (it will likely to be interpolated from 4 surrounding points).

			// Top left point.
			double sedimentSum = 0;
			double sedimentWeightSum = 0;
			if(
				baseCoordinateX < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y >= -coordinateOffsetY
				){
					double currentWeight = coordinatePartX * coordinatePartY;
					sedimentSum += sedimentToMoveMap[baseCoordinateX + this->width * baseCoordinateY] * currentWeight;
					sedimentWeightSum += currentWeight;
			}

			// Top right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x + 1 >= -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y >= -coordinateOffsetY
				){
					double currentWeight = (1 - coordinatePartX) * coordinatePartY;
					sedimentSum += sedimentToMoveMap[baseCoordinateX + 1 + this->width * baseCoordinateY] * currentWeight;
					sedimentWeightSum += currentWeight;
			}

			// Bottom left point.
			if(
				baseCoordinateX < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 > -coordinateOffsetY
				){
					double currentWeight = coordinatePartX * (1 - coordinatePartY);
					sedimentSum += sedimentToMoveMap[baseCoordinateX + this->width * (baseCoordinateY + 1)] * currentWeight;
					sedimentWeightSum += currentWeight;
			}

			// Bottom right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x + 1 >= -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 >= -coordinateOffsetY
				){
					double currentWeight = (1 - coordinatePartX) * (1 - coordinatePartY);
					sedimentSum += sedimentToMoveMap[baseCoordinateX + 1 + this->width * (baseCoordinateY + 1)] * currentWeight;
					sedimentWeightSum += currentWeight;
			}

			if(sedimentSum * sedimentWeightSum > 10){
				cout << "e";
			}

			sedimentMap[currentIndex] = sedimentWeightSum > 0 ? sedimentSum / sedimentWeightSum : 0;
		

			// The sediment might be projected outside the map borders. In that case, dump it.
			
			//cout << velocityVectorMap[currentIndex].x << ", " << velocityVectorMap[currentIndex].y << "             " << coordinatePartX << ", " << coordinatePartY << endl;
			//Sleep(200);			

			//double totalSedimented = 0;

			// TADY SE TO MA DELAT OPACNE (interpolovat zdroj, ne cil)

			// Top left point.
			/*if(
				baseCoordinateX < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y >= -coordinateOffsetY
			){
				//sedimentMap[baseCoordinateX + this->width * baseCoordinateY] += coordinatePartX * coordinatePartY * sedimentToMove;				
			}

			// Top right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x + 1 >= -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y >= -coordinateOffsetY
				)
			{
					sedimentMap[baseCoordinateX + 1 + this->width * baseCoordinateY] += (1 - coordinatePartX) * coordinatePartY * sedimentToMove;
			}

			// Bottom left point.
			if(
				baseCoordinateX < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 >= -coordinateOffsetY
				){
					sedimentMap[baseCoordinateX + this->width * (baseCoordinateY + 1)] += coordinatePartX * (1 - coordinatePartY) * sedimentToMove;
			}

			// Bottom right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 >= -coordinateOffsetY
				){
					sedimentMap[baseCoordinateX + 1 + this->width * (baseCoordinateY + 1)] += (1 - coordinatePartX) * (1 - coordinatePartY) * sedimentToMove;
			}*/

			/*double sedimentSum = 0;
			double sedimentWeightSum = 0;
			if(
				baseCoordinateX < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y >= -coordinateOffsetY
				){
					sedimentSum += sedimentMap[baseCoordinateX + this->width * baseCoordinateY];
					sedimentWeightSum += coordinatePartX * coordinatePartY;
			}

			// Top right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x + 1 >= -coordinateOffsetX &&
				baseCoordinateY < this->height &&
				y >= -coordinateOffsetY
				){
					sedimentSum += heightMap[baseCoordinateX + 1 + this->width * baseCoordinateY];
					sedimentWeightSum += (1 - coordinatePartX) * coordinatePartY;
			}

			// Bottom left point.
			if(
				baseCoordinateX < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 > -coordinateOffsetY
				){
					sedimentSum += heightMap[baseCoordinateX + this->width * (baseCoordinateY + 1)];
					sedimentWeightSum += coordinatePartX * (1 - coordinatePartY);
			}

			// Bottom right point.
			if(
				baseCoordinateX + 1 < this->width &&
				x >= -coordinateOffsetX &&
				baseCoordinateY + 1 < this->height &&
				y + 1 >= -coordinateOffsetY
				){
					sedimentSum += heightMap[baseCoordinateX + 1 + this->width * (baseCoordinateY + 1)];
					sedimentWeightSum += (1 - coordinatePartX) * (1 - coordinatePartY);
			}*/
		}
	}

    if(capacitySum != capacitySum){
        cout << "e" << endl;
    }

	cout << (capacitySum / this->length) << endl;

	//delete [] sedimentMapCopy;
	delete [] sedimentToMoveMap;
}
