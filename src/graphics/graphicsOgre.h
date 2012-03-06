#ifndef GRAPHICSOGRE_H
#define GRAPHICSOGRE_H

#include <OGRE/Ogre.h>
#include "../options.h"
#include "../eventManager.h"
#include "editorScene.h"

class GraphicsOgre
{
	public:
		GraphicsOgre();
		~GraphicsOgre();

		/*functions to initalize ogre*/
		bool initOgre(Options*);
		bool showAvailableRenderers();
		std::string getPluginFolder();
		bool setPluginFolder( std::string );
		std::string getWindowAttribute(); 		//OIS needs these informations for initialisation
		
		EditorScene* getScene();
		void renderFrame();
	
	private:
		Ogre::Root *ogre;
		Ogre::RenderWindow *window;
		std::string pluginFolder;
		EditorScene *scene;
		//for the FPS
		Ogre::Timer timer;
		int nbFrames;
		unsigned long lastFpsCalcul;
		unsigned long currentTime;
		int fpsLimit;
		unsigned long timeBetweenFrames;
		unsigned long lastFrameTime;
		
		void defineResources();


};



#endif
