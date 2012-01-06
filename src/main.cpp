//logging library
#include <glog/logging.h>
#include "game.h"

int main(int argc, char* argv[])
{
	//initialize logger
	FLAGS_logtostderr = true;
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "Program started";
	
	Game *my_game = new Game();
	my_game->loadOptions();
	my_game->initialize();
	
	while (my_game->isRunning()) {	
		LOG(INFO) << "Updating game";
		my_game->update();
	}
	
	delete my_game;

	LOG(INFO) << "Program stoped";
	return 0;
}
