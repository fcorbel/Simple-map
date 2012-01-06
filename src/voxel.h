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
	
	private:
		std::string type;
};

#endif /* VOXEL_H */ 
