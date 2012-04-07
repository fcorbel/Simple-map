#include "voxelMap.h"


VoxelMap::VoxelMap(int x, int y, int z):
	x_(x),
	y_(y),
	z_(z)
{
	map.resize(x);
	std::vector< std::vector< std::vector< Voxel* > > >::iterator xIt;
	std::vector< std::vector< Voxel* > >::iterator yIt;
	
	for (xIt = map.begin(); xIt != map.end(); ++xIt) {
		xIt->resize(y);
		for (yIt = xIt->begin(); yIt != xIt->end(); ++yIt) {
			yIt->resize(z);
		}
	}

	for (int i=0; i<x; ++i) {
		for (int j=0; j<y; ++j) {
			for (int k=0; k<z; ++k) {
				map[i][j][k] = NULL;
			}
		}
	}
	//~ resize(x,y,z);
}

VoxelMap::VoxelMap(std::string filename)
{
	//load a map from a file
	LOG(INFO) << "Trying to load map from file: " << filename; 

	std::ifstream input(filename.c_str());
	if (input.fail()) {
		LOG(ERROR) << "Failed to load file: " << filename;
	}
	std::string line;
	std::list<std::string> valueList;
	int x=0;
	int y=0;
	int z=0;
	int tmpX=0;
	int tmpY=0;
	int tmpZ=0;
	int i=1;
	while (getline(input, line)) {
		if (line[0] != '#') {
			//~ LOG(INFO) << "Line " << i << ": " << line[0];
			//~ ++i;
			
			if (!line.empty()) {
				++tmpZ;
				std::istringstream in(line);
				std::string value;
				//check each word in the line
				while (in >> value) {
					++tmpX;
					valueList.push_back(value);
					//~ LOG(INFO) << "Value: " << value;
				}
				if (x==0) {
					x = tmpX;
				} else {
					if (x != tmpX) {
						LOG(ERROR) << "Failed to load file (not correctly formated " << x << "!=" << tmpX << "): " << filename;
					}
				}
				tmpX = 0;
			} else {
				//we pass to the description of the under level
				++tmpY;
				if (z==0) {
					z = tmpZ;
				} else {
					if (z != tmpZ) {
						LOG(ERROR) << "Failed to load file (not correctly formated): " << filename;
					}
				}
				tmpZ = 0;		
			}
		}
	}
	y = tmpY;
	LOG(INFO) << "Size of the map to load: x=" << x << " y=" << y << " z=" << z;
	resize(x, y, z);
	
	for (int i=y-1; i>=0; --i) {
		for (int j=0; j<z; ++j) {
			for (int k=0; k<x; ++k) {		
				std::string value = valueList.front();
				LOG(INFO) << "Value = " << value;
				if (value == "NULL") {
					map[k][i][j] = NULL;
				} else {
					LOG(INFO) << "Add a voxel to the map";
					VoxelColored *vox;
					float r,g,b;
					if (hexStringToFloat(value, r, g, b)) {
						vox = new VoxelColored(r, g, b);
					} else {
						LOG(WARNING) << "Could not parse value: \"" << value << "\" to valid hex value, creating white voxel.";
						vox = new VoxelColored(1.0f, 1.0f, 1.0f);
					}
					map[k][i][j] = vox;
				}
				valueList.pop_front();
			}
		}	
	}
}

VoxelMap::~VoxelMap()
{
	for (int i=0; i<x_; ++i) {
		for (int j=0; j<y_; ++j) {
			for (int k=0; k<z_; ++k) {
				delete map[i][j][k];
			}
		}
	}
}

bool VoxelMap::resize(int x, int y, int z)
{
	LOG(INFO) << "Resizing map to : x=" << x << " y=" << y << " z=" << z;
	map.resize(x);
	std::vector< std::vector< std::vector< Voxel* > > >::iterator xIt;
	std::vector< std::vector< Voxel* > >::iterator yIt;
	
	for (xIt = map.begin(); xIt != map.end(); ++xIt) {
		xIt->resize(y);
		for (yIt = xIt->begin(); yIt != xIt->end(); ++yIt) {
			yIt->resize(z);
		}
	}
	x_ = x;
	y_ = y;
	z_ = z;
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
	if (rangeCorrect(x,y,z)) {
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
}

void VoxelMap::deleteVoxel(int x, int y, int z)
{
	if (rangeCorrect(x,y,z)) {
			if (map[x][y][z]) {
				delete map[x][y][z];
				map[x][y][z] = NULL;
			}
	}
	LOG(INFO) << "Try to delete voxel at: " << x << " " << y << " " << z;
	EventManager::Arguments arg;
	arg["x"] = x;
	arg["y"] = y;
	arg["z"] = z;
	EventManager::sendEvent("mapUpdated", arg);
}

bool VoxelMap::writeToFile(std::string filename)
{
	LOG(INFO) << "Trying to save the current map to file: " << filename;
	LOG(INFO) << "Size of the map: X=" << map.capacity() << " Y=" << map[0].capacity() << " Z=" << map[0][0].capacity();
	//TODO y and z seems to be inverted
	std::ofstream file(filename.c_str());
	if (file.is_open()) {
		file << "#representation of the voxel map\n#From top to bottom\n#Leave an empty line at the end of file\n";
		for (int i=y_-1; i>=0; --i) {
			for (int j=0; j<z_; ++j) {
				for (int k=0; k<x_; ++k) {		
					if (map[k][i][j]) {
						VoxelColored *vox = static_cast<VoxelColored*>(map[k][i][j]);
						file << vox->getColorAsHex() << " ";
					} else {
						file << "NULL ";
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

bool VoxelMap::rangeCorrect(int x, int y, int z)
{
	if (x<0 || x>=x_ || y<0 || y>=y_ || z<0 || z>=z_) {
		LOG(ERROR) << "Trying to access a voxel out of range: " << "x=" << x << "y=" << y << "z=" << z;
		exit(1);
		return false;
	}
	return true;
}

bool VoxelMap::hexStringToFloat(std::string hexString, float &red, float &green, float &blue)
{
	//check if the string is a valid hex value
	//TODO
	
	//parse values
	std::string r = hexString.substr(0, 2);
	std::string g = hexString.substr(2, 2);
	std::string b = hexString.substr(4, 2);
	
	int temp;
	std::stringstream ss;
	ss << std::hex << r;
	ss >> temp;
	red = (float)temp/255;
	ss << std::hex << g;
	ss >> temp;
	green = (float)temp/255;
	ss << std::hex << b;
	ss >> temp;
	blue = (float)temp/255;
	//~ LOG(INFO) << "r" << red << "g" << green << "b" << blue;
	return true;
}
