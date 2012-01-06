#include "voxel_colored.h"


Voxel_colored::Voxel_colored(int _color)
	: Voxel(),
	color(_color)
{
	type = "colored voxel";
}

int Voxel_colored::getColor()
{
	return color;
}

