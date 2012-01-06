#include "voxelmap.h"


VoxelMap::VoxelMap(int _x, int _y, int _z):
	x(_x),
	y(_y),
	z(_z)
{
	resize(_x,_y,_z);
}

VoxelMap::~VoxelMap()
{
	
}

bool VoxelMap::resize(int _x, int _y, int _z)
{
	map.resize(_x);
	std::vector< std::vector< std::vector< Voxel > > >::iterator xIt;
	std::vector< std::vector< Voxel > >::iterator yIt;
	
	for (xIt = map.begin(); xIt != map.end(); ++xIt) {
		xIt->resize(_y);
		for (yIt = xIt->begin(); yIt != xIt->end(); ++yIt) {
			yIt->resize(_z);
		}
	}
	x = _x;
	y = _y;
	z = _z;
	
	EventManager::sendEvent("mapUpdated", EventManager::Arguments());
	return true;
}

Voxel VoxelMap::getVoxel(int x, int y, int z)
{
	return map[x][y][z];
}

void VoxelMap::setVoxel(Voxel newVoxel, int x, int y, int z)
{
		map[x][y][z] = newVoxel;
		EventManager::Arguments arg;
		arg["x"] = x;
		arg["y"] = y;
		arg["z"] = z;
		EventManager::sendEvent("mapUpdated", arg);
}

bool VoxelMap::writeToFile(std::string filename)
{
	LOG(INFO) << "size of the map: X=" << map.capacity() << " Y=" << map[0].capacity() << " Z=" << map[0][0].capacity();
	
	std::ofstream file(filename.c_str());
	if (file.is_open()) {
		file << "#representation of the voxel map\n#From top to bottom\n\n";
		for (int i=z-1; i>-1; --i) {
			for (int j=0; j<y; ++j) {
				for (int k=0; k<x; ++k) {
					file << map[k][j][i].getType() << " ";
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
