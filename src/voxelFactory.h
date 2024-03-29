#ifndef VOXELFACTORY_H
#define VOXELFACTORY_H

#include "voxel.h"
#include "voxelColored.h"
#include <string>

class VoxelFactory
{
	public:
		VoxelFactory();
		static Voxel* createVoxel();
		static Voxel* createVoxel(std::string name);
		static Voxel* createColoredVoxel(float red, float green, float blue);
			
	private:
		/* add your private declarations */
};

#endif /* VOXELFACTORY_H */ 
