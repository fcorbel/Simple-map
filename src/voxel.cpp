#include "voxel.h"

Voxel::Voxel():
	type("unknown")
{

}


Voxel::Voxel(std::string voxType):
	type(voxType)
{
	
}

Voxel::~Voxel()
{
	
}

std::string Voxel::getType()
{
	return type;
}
