#ifndef VOXEL_COLORED_H
#define VOXEL_COLORED_H

#include "voxel.h"

class VoxelColored: public Voxel
{
	public:
		VoxelColored(int _hex_color);
		~VoxelColored();
			
		int getColor();
		void setColor(int _hex_color);
			
	private:
		int hex_color;

};

#endif /* VOXEL_COLORED_H */ 
