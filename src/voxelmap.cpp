#include "voxelmap.h"


VoxelMap::VoxelMap(int _x, int _y, int _z):
	x(_x),
	y(_y),
	z(_z)
{
	map.resize(_x);
	std::vector< std::vector< std::vector< Voxel* > > >::iterator xIt;
	std::vector< std::vector< Voxel* > >::iterator yIt;
	
	for (xIt = map.begin(); xIt != map.end(); ++xIt) {
		xIt->resize(_y);
		for (yIt = xIt->begin(); yIt != xIt->end(); ++yIt) {
			yIt->resize(_z);
		}
	}

	for (int i=0; i<_x; ++i) {
		for (int j=0; j<_y; ++j) {
			for (int k=0; k<_z; ++k) {
				map[i][j][k] = NULL;
			}
		}
	}
	//~ resize(_x,_y,_z);
}

VoxelMap::~VoxelMap()
{
	for (int i=0; i<x; ++i) {
		for (int j=0; j<y; ++j) {
			for (int k=0; k<z; ++k) {
				delete map[i][j][k];
			}
		}
	}
}

bool VoxelMap::resize(int _x, int _y, int _z)
{
	//~ map.resize(_x);
	//~ std::vector< std::vector< std::vector< Voxel* > > >::iterator xIt;
	//~ std::vector< std::vector< Voxel* > >::iterator yIt;
	//~ 
	//~ for (xIt = map.begin(); xIt != map.end(); ++xIt) {
		//~ xIt->resize(_y);
		//~ for (yIt = xIt->begin(); yIt != xIt->end(); ++yIt) {
			//~ yIt->resize(_z);
		//~ }
	//~ }
	//~ x = _x;
	//~ y = _y;
	//~ z = _z;
	//~ 
	//~ EventManager::sendEvent("mapUpdated", EventManager::Arguments());
	return true;
}

Voxel* VoxelMap::getVoxel(int x, int y, int z)
{
	if (map[x][y][z] == NULL) {
		LOG(WARNING) << "Trying to access a non-initialized voxel at: [" << x << "," << y << "," << z << "]";
	}
	return map[x][y][z];
}

void VoxelMap::setVoxel(Voxel* newVoxel, int x, int y, int z)
{
	if (map[x][y][z]) {
		delete map[x][y][z];
	}
	map[x][y][z] = newVoxel;
	EventManager::Arguments arg;
	arg["x"] = x;
	arg["y"] = y;
	arg["z"] = z;
	EventManager::sendEvent("mapUpdated", arg);
}

bool VoxelMap::writeToFile(std::string filename)
{

	//~ LOG(INFO) << "size of the map: X=" << map.capacity() << " Y=" << map[0].capacity() << " Z=" << map[0][0].capacity();
	
	std::ofstream file(filename.c_str());
	if (file.is_open()) {
		file << "#representation of the voxel map\n#From top to bottom\n\n";
		for (int i=z-1; i>-1; --i) {
			for (int j=0; j<y; ++j) {
				for (int k=0; k<x; ++k) {
					if (map[k][j][i]) {
						//WRN ajusted for ColoredVoxel
						VoxelColored *vox = (VoxelColored*)(map[k][j][i]);
						file << vox->getColor() << " ";
					} else {
						file << "NULL";
					}
				}
				file << "\n";
			}
			file << "\n";
		}

		file.close();
	} else {
		LOG(WARNING) << "Unable to open file: " << filename;
		return false;
	}
	
	
	return true;
}
