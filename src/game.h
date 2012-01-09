#ifndef GAME_H
#define GAME_H
#include <glog/logging.h>
#include <map>
#include <string>
#include "options.h"
#include "eventManager.h"
#include "voxelmap.h"
#include "voxelfactory.h"
#include "voxelcolored.h"


class Game
{
	public:
		Game();
		~Game();

		bool loadOptions();
		bool initialize();
		void shutdown(std::string, EventManager::Arguments);
		bool isRunning();
		void update();
		
		void createNewMap(int, int, int);
		void setVoxelColor(int, int, int, int);
		bool saveMap(std::string);
		bool loadMap(std::string);
		
		
	private:
		bool running;
		Options *config;
		Options *keymap;
		VoxelMap *voxel_map;

		//variables to manage unsubscribe of events
		int quit_event;
};

#endif
