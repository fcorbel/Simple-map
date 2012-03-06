#ifndef GUISYSTEM_H
#define GUISYSTEM_H

#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>
#include "../eventManager.h"
#include "editorScene.h"
#include "OGRE/Ogre.h"

class GuiSystem
{
	public:
		//~ GuiSystem(EditorScene *_scene, Ogre::RenderWindow *window, Ogre::SceneManager *sceneMgr);
		GuiSystem(Ogre::RenderWindow *window, Ogre::SceneManager *sceneMgr);
		
		virtual ~GuiSystem();

		void myGuiUpdate(std::string eventName, EventManager::Arguments args);
		void exitRequested(MyGUI::WidgetPtr sender);
		void moveMap(std::string eventName, EventManager::Arguments args);

	private:
		//~ EditorScene *scene;
		MyGUI::Gui *myGUI;
		MyGUI::OgrePlatform* mPlatform;
		bool moveMapMode;
};

#endif /* GUISYSTEM_H */ 
