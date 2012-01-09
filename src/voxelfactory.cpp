#include "voxelfactory.h"


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

Voxel* VoxelFactory::createColoredVoxel(int color)
{
	return new VoxelColored(color);
}
