#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <glog/logging.h>
#include "../eventManager.h"
#include "OGRE/Ogre.h"
#include "../voxelMap.h"
#include "camera.h"
#include "sidepanel.h"
#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>

class EditorScene;
struct Coordinates {
	int x;
	int y;
	int z;
};

struct Colour {
	float red;
	float green;
	float blue;
};

class GuiSystem
{
	public:
		GuiSystem(EditorScene *_scene, Ogre::RenderWindow *window, Ogre::SceneManager *sceneMgr);		
		~GuiSystem();

		void updateColour(std::string, EventManager::Arguments);
		void myGuiUpdate(std::string eventName, EventManager::Arguments args);
		void exitRequested(MyGUI::WidgetPtr sender);
		void moveMap(std::string eventName, EventManager::Arguments args);
		Coordinates stringToCoordinates(std::string); 
		Colour getSelectedColour() { return selectedColour; }
	private:	
		void checkSelection(std::string, EventManager::Arguments);		
		void modifyCube(std::string, EventManager::Arguments);
		void changeEditMode(std::string, EventManager::Arguments);
		
		bool isClearMode;
		Ogre::SceneNode *selectedNode;
		Colour selectedColour;		
		EditorScene *scene;
		MyGUI::Gui *myGUI;
		MyGUI::OgrePlatform* mPlatform;
		bool moveMapMode;
		//pointers to widgets
		SidePanel* sidePanel;
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

	private:
		Ogre::SceneManager *sceneMgr;
		VoxelMap *voxelMap;
		Ogre::SceneNode *mapMainNode;
		Camera *camera;
		float cubeSize;
		//GUI
		GuiSystem *gui;
		//to move the map
		Ogre::SceneNode *mapYawNode;
		Ogre::SceneNode *mapPitchNode;
		Ogre::Radian rotX, rotY;
		
		void createCube(int x, int y, int z, Colour colour);
};

#endif /* EDITORSCENE_H */ 
