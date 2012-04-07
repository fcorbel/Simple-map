#ifndef VOXEL_COLORED_H
#define VOXEL_COLORED_H

#include "voxel.h"
#include "OGRE/Ogre.h"


class VoxelColored: public Voxel
{
	public:
		VoxelColored(float red, float green, float blue);
		~VoxelColored();

		int getColorAsHex();
		void setColor(float red, float green, float blue);
		float getRed() { return red_; }
		float getGreen() { return green_; }
		float getBlue() { return blue_; }
		
	private:
		float red_;
		float green_;
		float blue_;
		
};

#endif /* VOXEL_COLORED_H */ 
