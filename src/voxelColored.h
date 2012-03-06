#ifndef VOXEL_COLORED_H
#define VOXEL_COLORED_H

#include "voxel.h"
#include "OGRE/Ogre.h"


class VoxelColored: public Voxel
{
	public:
		VoxelColored(int _hexColor);
		~VoxelColored();
			
		int getColor();
		void setColor(int _hexColor);

	private:
		int hexColor;
};

#endif /* VOXEL_COLORED_H */ 
