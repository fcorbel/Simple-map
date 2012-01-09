#ifndef VOXEL_H
#define VOXEL_H

#include <string>

class Voxel
{
	public:
		Voxel();
		Voxel(std::string voxType);
		virtual ~Voxel();
		
		std::string getType();
	
	protected:
		std::string type;
};

#endif /* VOXEL_H */ 
