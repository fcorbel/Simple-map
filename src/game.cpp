#include "game.h"

Game::Game()
{
	running = false;
	config = NULL;
	keymap = NULL;
	voxelMap = NULL;
	graphics = NULL;
}

Game::~Game()
{
	EventManager::unSubscribe(quitEvent);
	
	LOG(INFO) << "Writing configuration file";
	config->writeToFile("config.cfg");
	if (voxelMap) {
		delete voxelMap;
	}
	if (input) {
		delete input;
	}
	if (graphics) {
		delete graphics;
	}
	if (keymap) {
		delete keymap;
	}
	if (config) {
		delete config;
	}
	EventManager::listListeners();

}

bool Game::loadOptions()
{
	//Load general options
	std::map<std::string, std::string> defaults;
	defaults["appname"] = "MyAppName";
	defaults["fullscreen"] = "0";
	defaults["width"] = "800";
	defaults["height"] = "600";
	config = new Options(defaults);
	
	//load keymap options
	std::map<std::string, std::string> defaults_keymap;
	defaults_keymap["Escape"] = "quitGame";
	defaults_keymap["c"] = "changeBackColour";
	//for the camera movements
	defaults_keymap["Up"] = "moveForward";
	defaults_keymap["Down"] = "moveBackward";
	defaults_keymap["Left"] = "moveLeft";
	defaults_keymap["Right"] = "moveRight";
	//for the GUI
	defaults_keymap["Mb_Right"] = "rightClick";
	defaults_keymap["Mb_Left"] = "leftClick";

	keymap = new Options(defaults_keymap);
	
	return true;
}

bool Game::initialize()
{
	config->showOptions();
	keymap->showOptions();
	
	quitEvent = EventManager::subscribe("quitGame", this, &Game::shutdown);
	running = true;
	
	//start GUI (Ogre engine)
	graphics = new GraphicsOgre();
	graphics->initOgre(config);
	//start input (OIS)
	input = new OIS_input();
	input->initializeWithOgre(graphics->getWindowAttribute());
	input->setKeymap(keymap);
	
	//load map
	createNewMap(3,3,3);
	saveMap("my_first_map.txt");
	graphics->getScene()->setMap(voxelMap);
	return true;
}

void Game::shutdown(std::string event_name, EventManager::Arguments args)
{
	if (boost::any_cast<bool>(args["pressed"]) == false) {		
		running = false;
	}
}

bool Game::isRunning()
{
	return running;
}

void Game::update()
{
	//~ LOG(INFO) << "Updating game";
	input->capture();
	EventManager::processEvents();
	graphics->renderFrame();
	}

void Game::createNewMap(int x, int y, int z)
{
	if (voxelMap) {
		delete voxelMap;
	}
	voxelMap = new VoxelMap(x, y, z);
	//~ voxelMap->setAllVoxels<VoxelColored>(0x000000);
	EventManager::Arguments arg;
	EventManager::sendEvent("mapUpdated", arg);

	
	//~ setVoxelColor(1,1,1, 0xff0000);
	//~ setVoxelColor(1,0,1, 0xff0000);

}

void Game::setVoxelColor(int x, int y, int z, int color)
{
	VoxelColored *vox = (VoxelColored*)(voxelMap->getVoxel(x,y,z));
	vox->setColor(color);
	EventManager::Arguments arg;
	arg["x"] = x;
	arg["y"] = y;
	arg["z"] = z;
	EventManager::sendEvent("mapUpdated", arg);
}

bool Game::saveMap(std::string filename)
{
	return voxelMap->writeToFile(filename);
}

bool Game::loadMap(std::string filename)
{
	//TODO
	return true;
}
