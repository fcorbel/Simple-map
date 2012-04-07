#ifndef GAME_H
#define GAME_H
#include <glog/logging.h>
#include <map>
#include <string>
#include "options.h"
#include "eventManager.h"
#include "voxelMap.h"
#include "voxelFactory.h"
#include "voxelColored.h"
#include "graphics/graphicsOgre.h"
#include "input/oisInput.h"


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
		bool saveMap(std::string);
		bool loadMap(std::string);
		
		
	private:
		bool running;
		Options *config;
		Options *keymap;
		VoxelMap *voxelMap;
		GraphicsOgre *graphics;
		OIS_input *input;

		//variables to manage unsubscribe of events
		int quitEvent;
};

#endif
