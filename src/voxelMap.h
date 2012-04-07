#ifndef VOXELMAP_H
#define VOXELMAP_H

#include <glog/logging.h>
#include "voxelFactory.h"
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iostream>
#include <fstream>
#include "eventManager.h"

class VoxelMap
{
	public:
		VoxelMap(int, int, int);
		VoxelMap(std::string filename);
		~VoxelMap();

		Voxel* getVoxel(int, int, int);
		void setVoxel(Voxel*, int, int, int);
		template<typename T> void setAllVoxels(int color)
		{
			for (int i=0; i<x_; ++i) {
				for (int j=0; j<y_; ++j) {
					for (int k=0; k<z_; ++k) {
						//WRN arg is just for VoxelColored, not clean
						if (map[i][j][k]) {
							delete map[i][j][k];
						}
						map[i][j][k] = new T(color);
					}
				}
			}
			EventManager::Arguments arg;
			EventManager::sendEvent("mapUpdated", arg);
		}
		void deleteVoxel(int, int, int);
		int getSizeX() { return x_; }
		int getSizeY() { return y_; }
		int getSizeZ() { return z_; }

		bool writeToFile(std::string filename);

	private:
		std::vector< std::vector< std::vector< Voxel* > > > map;
		int x_;
		int y_;
		int z_;
		bool rangeCorrect(int, int, int);
		bool resize(int, int, int);
		bool hexStringToFloat(std::string hexString, float &red, float &green, float &blue);

};

#endif /* VOXELMAP_H */ 
