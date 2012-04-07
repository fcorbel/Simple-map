#include "voxelFactory.h"


VoxelFactory::VoxelFactory()
{
	
}

Voxel* VoxelFactory::createVoxel()
{
	return new Voxel();
}

Voxel* VoxelFactory::createVoxel(std::string name)
{
	return new Voxel();
}

Voxel* VoxelFactory::createColoredVoxel(float red, float green, float blue)
{
	return new VoxelColored(red, green, blue);
}
