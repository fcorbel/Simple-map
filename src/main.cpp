//logging library
#include <glog/logging.h>
#include "game.h"

int main(int argc, char* argv[])
{
	//initialize logger
	FLAGS_logtostderr = true;
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "Program started";
	
	Game *myGame = new Game();
	myGame->loadOptions();
	myGame->initialize();
	
	while (myGame->isRunning()) {	
		myGame->update();
	}
	
	delete myGame;

	return 0;
}
