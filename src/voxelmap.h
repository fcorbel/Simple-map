#ifndef VOXELMAP_H
#define VOXELMAP_H

#include <glog/logging.h>
#include "voxelfactory.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "eventManager.h"

class VoxelMap
{
	public:
		VoxelMap(int, int, int);
		~VoxelMap();

		bool resize(int, int, int);
		Voxel* getVoxel(int, int, int);
		void setVoxel(Voxel*, int, int, int);
		template<typename T> void setAllVoxels(int arg)
		{
			for (int i=0; i<x; ++i) {
				for (int j=0; j<y; ++j) {
					for (int k=0; k<z; ++k) {
						//WRN arg is just for VoxelColored, not clean
						setVoxel(new T(arg), i,j,k);
					}
				}
			}
			EventManager::Arguments arg;
			EventManager::sendEvent("mapUpdated", arg);
		}
		int getSizeX() { return x; }
		int getSizeY() { return y; }
		int getSizeZ() { return z; }

		bool writeToFile(std::string filename);
		
	private:
		std::vector< std::vector< std::vector< Voxel* > > > map;
		int x;
		int y;
		int z;

};

#endif /* VOXELMAP_H */ 
