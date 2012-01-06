#ifndef VOXELMAP_H
#define VOXELMAP_H

#include <glog/logging.h>
#include "voxel.h"
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
		Voxel getVoxel(int, int, int);
		void setVoxel(Voxel, int, int, int);
		int getSizeX() { return x; }
		int getSizeY() { return y; }
		int getSizeZ() { return z; }

		bool writeToFile(std::string filename);
		
	private:
		std::vector< std::vector< std::vector< Voxel > > > map;
		int x;
		int y;
		int z;

};

#endif /* VOXELMAP_H */ 
