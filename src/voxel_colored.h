#ifndef VOXEL_COLORED_H
#define VOXEL_COLORED_H

#include <voxel.h>

class Voxel_colored: public Voxel
{
	public:
		Voxel_colored(int _color);
		~Voxel_colored();
			
		int getColor();
			
	private:
		int color;

};

#endif /* VOXEL_COLORED_H */ 
