#include "voxelColored.h"


VoxelColored::VoxelColored(float red, float green, float blue):
	Voxel(),
	red_(red),
	green_(green),
	blue_(blue)
{
	type = "colored";
}

VoxelColored::~VoxelColored()
{

}

int VoxelColored::getColorAsHex()
{
	//TODO do the conversion float->hex
	return 777777;
}

void VoxelColored::setColor(float red, float green, float blue)
{
	red_ = red;
	green_ = green;
	blue_ = blue;
}
