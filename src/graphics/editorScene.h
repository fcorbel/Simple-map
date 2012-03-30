#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <glog/logging.h>
#include "../eventManager.h"
#include "OGRE/Ogre.h"
#include "../voxelMap.h"
#include "camera.h"
#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>

class EditorScene;
struct Coordinates {
	int x;
	int y;
	int z;
};


class GuiSystem
{
	public:
		GuiSystem(EditorScene *_scene, Ogre::RenderWindow *window, Ogre::SceneManager *sceneMgr);		
		~GuiSystem();

		void myGuiUpdate(std::string eventName, EventManager::Arguments args);
		void exitRequested(MyGUI::WidgetPtr sender);
		void moveMap(std::string eventName, EventManager::Arguments args);
		Coordinates stringToCoordinates(std::string); 
	private:	
		void checkSelection(std::string, EventManager::Arguments);		
		void modifyCube(std::string, EventManager::Arguments);
		void changeEditMode(std::string, EventManager::Arguments);

		bool isClearMode;
		Ogre::SceneNode *selectedNode;
		EditorScene *scene;
		MyGUI::Gui *myGUI;
		MyGUI::OgrePlatform* mPlatform;
		bool moveMapMode;
		
};

class EditorScene
{
	public:
		EditorScene(Ogre::Root* ogre, Ogre::RenderWindow* window);
		~EditorScene();
		void setMap(VoxelMap*);
		void loadMap();
		void updateMap(std::string, EventManager::Arguments);
		void rotateMap(Ogre::Radian x, Ogre::Radian y);
		void update(unsigned long);
		Camera* getCamera() { return camera; };
		Ogre::SceneManager* getSceneMgr() { return sceneMgr; };
		VoxelMap* getVoxelMap() { return voxelMap; };
		//GUI
		void clearRequested(MyGUI::WidgetPtr sender);

	private:
		//~ void loadGUI(Ogre::RenderWindow* window);
		Ogre::SceneManager *sceneMgr;
		VoxelMap *voxelMap;
		Ogre::SceneNode *mapMainNode;
		Camera *camera;
		float cubeSize;
		int selectedColour;
		//GUI
		GuiSystem *gui;
		//to move the map
		Ogre::SceneNode *mapYawNode;
		Ogre::SceneNode *mapPitchNode;
		Ogre::Radian rotX, rotY;
};

#endif /* EDITORSCENE_H */ 
