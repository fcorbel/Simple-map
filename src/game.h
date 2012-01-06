#ifndef GAME_H
#define GAME_H
#include <glog/logging.h>
#include <map>
#include <string>
#include "options.h"
#include "eventManager.h"
#include "voxelmap.h"

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

	private:
		bool running;
		Options *config;
		Options *keymap;
		VoxelMap *voxel_map;

		//variables to manage unsubscribe of events
		int quit_event;
};

#endif
