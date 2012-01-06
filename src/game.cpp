#include "game.h"

Game::Game()
{
	running = false;
	config = NULL;
	keymap = NULL;
	voxel_map = NULL;
}

Game::~Game()
{
	EventManager::unSubscribe(quit_event);
	
	LOG(INFO) << "Writing configuration file";
	config->writeToFile("config.cfg");
	if (voxel_map) {
		delete voxel_map;
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
	//for the camera movements
	defaults_keymap["Up"] = "moveForward";
	defaults_keymap["Down"] = "moveBackward";
	defaults_keymap["Left"] = "moveLeft";
	defaults_keymap["Right"] = "moveRight";

	keymap = new Options(defaults_keymap);
	
	return true;
}

bool Game::initialize()
{
	config->showOptions();
	keymap->showOptions();
	
	quit_event = EventManager::subscribe("quitGame", this, &Game::shutdown);
	running = true;
	//load map
	voxel_map = new VoxelMap(10, 10, 10);
	voxel_map->writeToFile("my_first_map.txt");
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
	EventManager::processEvents();
	
	EventManager::Arguments arg;
	arg["pressed"] = false;
	EventManager::sendEvent("quitGame", arg);
}
