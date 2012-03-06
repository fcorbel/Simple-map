#include "voxelColored.h"


VoxelColored::VoxelColored(int _hexColor)
	: Voxel(),
	hexColor(_hexColor)
{
	type = "colored";
}

VoxelColored::~VoxelColored()
{

}

int VoxelColored::getColor()
{
	return hexColor;
}

void VoxelColored::setColor(int _hexColor)
{
	hexColor = _hexColor;	
}
