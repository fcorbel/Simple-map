#include "voxelcolored.h"


VoxelColored::VoxelColored(int _hex_color)
	: Voxel(),
	hex_color(_hex_color)
{
	type = "colored";
}

VoxelColored::~VoxelColored()
{
	
}

int VoxelColored::getColor()
{
	return hex_color;
}

void VoxelColored::setColor(int _hex_color)
{
	hex_color = _hex_color;	
}
