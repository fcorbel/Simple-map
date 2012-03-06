#include "editorScene.h"


EditorScene::EditorScene(Ogre::Root* ogre, Ogre::RenderWindow* window):
	cubeSize(50.0f),
	rotX(0),
	rotY(0),
	selectedColour(0),
	gui(NULL)
{
	LOG(INFO) << "== Initialization of the scene ==";
	sceneMgr = ogre->createSceneManager(Ogre::ST_GENERIC);
	mapPitchNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	mapYawNode = mapPitchNode->createChildSceneNode();
	mapMainNode = mapYawNode->createChildSceneNode();
	
	//load camera
	camera = new Camera("mainCam", sceneMgr, window);
	camera->setPosition(0, 0, 2000);
	//load GUI
	gui = new GuiSystem(this, window, sceneMgr);
	//create the basic cube entity
	Ogre::ManualObject* cube;
	cube = sceneMgr->createManualObject("cube");
	cube->setDynamic(false);
	cube->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	{
		cube->position(0,0,0);
		cube->position(cubeSize,0,0);
		cube->position(cubeSize,cubeSize,0);
		cube->position(0,cubeSize,0);
		cube->position(0,0,cubeSize);
		cube->position(cubeSize,0,cubeSize);
		cube->position(cubeSize,cubeSize,cubeSize);
		cube->position(0,cubeSize,cubeSize);
		
		//front/back
		cube->triangle(4,5,6);
		cube->triangle(6,7,4);
		cube->triangle(1,0,2);
		cube->triangle(2,0,3);
		//top/down
		cube->triangle(7,6,2);
		cube->triangle(2,3,7);
		cube->triangle(1,5,4);
		cube->triangle(4,0,1);
		//left/right
		cube->triangle(7,3,0);
		cube->triangle(0,4,7);
		cube->triangle(1,2,6);
		cube->triangle(6,5,1);
	}
	cube->end();
	cube->convertToMesh("meshCube");
	
	//create the selection marker cube
	Ogre::ManualObject* selectionMarker;
	selectionMarker = sceneMgr->createManualObject("selectionMarker");
	selectionMarker->setDynamic(false);
	selectionMarker->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	{
		selectionMarker->position(-1,-1,-1);
		selectionMarker->colour(Ogre::ColourValue(0.2, 0.5, 0.4));
		selectionMarker->position(cubeSize+1,-1,-1);
		selectionMarker->position(cubeSize+1,cubeSize+1,-1);
		selectionMarker->position(-1,cubeSize+1,-1);
		selectionMarker->position(-1,-1,cubeSize+1);
		selectionMarker->position(cubeSize+1,-1,cubeSize+1);
		selectionMarker->position(cubeSize+1,cubeSize+1,cubeSize+1);
		selectionMarker->position(-1,cubeSize+1,cubeSize+1);

		selectionMarker->index(0);
		selectionMarker->index(1);
		selectionMarker->index(1);
		selectionMarker->index(2);
		selectionMarker->index(2);
		selectionMarker->index(3);
		selectionMarker->index(3);
		selectionMarker->index(0);
		
		selectionMarker->index(4);
		selectionMarker->index(5);
		selectionMarker->index(5);
		selectionMarker->index(6);
		selectionMarker->index(6);
		selectionMarker->index(7);
		selectionMarker->index(7);
		selectionMarker->index(4);
		
		selectionMarker->index(0);
		selectionMarker->index(4);
		selectionMarker->index(1);
		selectionMarker->index(5);
		selectionMarker->index(2);
		selectionMarker->index(6);
		selectionMarker->index(3);
		selectionMarker->index(7);
	}
	selectionMarker->end();
	selectionMarker->convertToMesh("meshSelectionMarker");

	
	EventManager::subscribe("mapUpdated", this, &EditorScene::updateMap);
}

EditorScene::~EditorScene()
{
	if (gui) { delete gui; }
	if (camera) { delete camera; }

	//TODO destroy sceneMgr?
}

void EditorScene::setMap(VoxelMap *_voxelMap)
{	
	voxelMap = _voxelMap;
	//move mapMainNode so that the map is in the middle of worldNode
	mapMainNode->setPosition(voxelMap->getSizeX()*cubeSize/-2, 0, voxelMap->getSizeZ()*cubeSize/-2);
	//create scene nodes for all voxels in the map / attach the selection marker to it
	for (int i=0; i<voxelMap->getSizeX(); ++i) {
		for (int j=0; j<voxelMap->getSizeY(); ++j) {
			for (int k=0; k<voxelMap->getSizeZ(); ++k) {
				Ogre::SceneNode *newNode = mapMainNode->createChildSceneNode(Ogre::StringConverter::toString(i) + Ogre::StringConverter::toString(j) + Ogre::StringConverter::toString(k));
				newNode->setPosition(i*cubeSize, j*cubeSize, k*cubeSize);
				//create the mouse selection marker
				Ogre::Entity *ent = sceneMgr->createEntity(Ogre::StringConverter::toString(i) + Ogre::StringConverter::toString(j) + Ogre::StringConverter::toString(k) + "_marker", "meshSelectionMarker");
				ent->setVisible(false);
				newNode->attachObject(ent);
			}
		}
	}
	//show planes X,Y,Z (for the editor)
	Ogre::ManualObject *editorAxis;
	editorAxis = sceneMgr->createManualObject("editorAxis");
	editorAxis->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	{
		//vertices from plane X
		for (int i=0; i<=voxelMap->getSizeX(); ++i) {
			editorAxis->position(cubeSize*i, 0.0f, 0.0f);
			editorAxis->colour(Ogre::ColourValue::Red);			
			editorAxis->position(cubeSize*i, 0.0f, cubeSize*voxelMap->getSizeZ());
			editorAxis->colour(Ogre::ColourValue::Red);
		}
		for (int i=0; i<=voxelMap->getSizeZ(); ++i) {
			editorAxis->position(0.0f, 0.0f, cubeSize*i);
			editorAxis->colour(Ogre::ColourValue::Red);			
			editorAxis->position(cubeSize*voxelMap->getSizeZ(), 0.0f, cubeSize*i);
			editorAxis->colour(Ogre::ColourValue::Red);
		}
		//vertices from plane Y
		for (int i=0; i<=voxelMap->getSizeX(); ++i) {
			editorAxis->position(cubeSize*i, 0.0f, 0.0f);
			editorAxis->colour(Ogre::ColourValue::Blue);			
			editorAxis->position(cubeSize*i, cubeSize*voxelMap->getSizeY(), 0.0f);
			editorAxis->colour(Ogre::ColourValue::Blue);
		}
		for (int i=0; i<=voxelMap->getSizeY(); ++i) {
			editorAxis->position(0.0f, cubeSize*i, 0.0f);
			editorAxis->colour(Ogre::ColourValue::Blue);			
			editorAxis->position(cubeSize*voxelMap->getSizeX(), cubeSize*i, 0.0f);
			editorAxis->colour(Ogre::ColourValue::Blue);
		}
		//vertices from plane Z
		for (int i=0; i<=voxelMap->getSizeZ(); ++i) {
			editorAxis->position(0.0f, 0.0f, cubeSize*i);
			editorAxis->colour(Ogre::ColourValue::Green);			
			editorAxis->position(0.0f, cubeSize*voxelMap->getSizeY(), cubeSize*i);
			editorAxis->colour(Ogre::ColourValue::Green);
		}
		for (int i=0; i<=voxelMap->getSizeY(); ++i) {
			editorAxis->position(0.0f, cubeSize*i, 0.0f);
			editorAxis->colour(Ogre::ColourValue::Green);			
			editorAxis->position(0.0f, cubeSize*i, cubeSize*voxelMap->getSizeZ());
			editorAxis->colour(Ogre::ColourValue::Green);
		}	
	}
	editorAxis->end();
	mapMainNode->attachObject(editorAxis);
	
	loadMap();
}

void EditorScene::loadMap()
{
	//draw the whole voxelMap
	for (int i=0; i<voxelMap->getSizeX(); ++i) {
		for (int j=0; j<voxelMap->getSizeY(); ++j) {
			for (int k=0; k<voxelMap->getSizeZ(); ++k) {
				if (voxelMap->getVoxel(i,j,k)) {
					VoxelColored *vox = (VoxelColored*)voxelMap->getVoxel(i,j,k);
					if (vox->getType() == "colored") {
						//create a cube of the right color, attach it to his node
						LOG(INFO) << "Create new entity for cube at position: [" << i << "," << j << "," << k << "]"; 
						Ogre::Entity *newCube = sceneMgr->createEntity("meshCube");
						//set the right color for the cube
						//TODO
						//~ newCube->getSubEntity(0)->setMaterialName("test");
						LOG(INFO) << "===========================" << newCube;
						LOG(INFO) << "===========================" << newCube->getSubEntity(0)->getMaterial()->getTechnique(0)->getNumPasses();
						//~ Ogre::TextureUnitState *texState = newCube->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->createTextureUnitState();
						
						//~ newCube->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(
							//~ Ogre::LBX_SOURCE1,
							//~ Ogre::LBS_MANUAL,
							//~ Ogre::LBS_CURRENT,
							//~ Ogre::ColourValue(1.0, 0.0, 1.0));
						//~ newCube->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOpMultipassFallback(
							//~ Ogre::SBF_ONE,
							//~ Ogre::SBF_ZERO);
						//~ newCube->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->setAmbient(0, 1, 1);
						//~ newCube->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->setSelfIllumination(1, 1, 0);
						//~ newCube->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->setLightingEnabled(true);

						sceneMgr->getSceneNode(Ogre::StringConverter::toString(i) + Ogre::StringConverter::toString(j) + Ogre::StringConverter::toString(k))->attachObject(newCube);
					} else {
						LOG(WARNING) << "Unknown voxel in the map: don't know how to draw";
					}
				}
			}
		}
	}
}

void EditorScene::updateMap(std::string eventName, EventManager::Arguments args) //TODO cover every cases
{
	//update one pixel / every pixel if arg is empty
	if (args.empty()) {
		loadMap();
	} else {
		//update one pixel
		//TODO check if just a change in colour, to optimize a bit
		int x = boost::any_cast<int>(args["x"]);
		int y = boost::any_cast<int>(args["y"]);
		int z = boost::any_cast<int>(args["z"]);
		LOG(INFO) << "Updating representatgion of pixel at: " << x << " " << y << " " << z;
		Ogre::SceneNode *node = sceneMgr->getSceneNode(Ogre::StringConverter::toString(x) + Ogre::StringConverter::toString(y) + Ogre::StringConverter::toString(z));
		if (voxelMap->getVoxel(x,y,z)) {
			//have to create a new cube
			if (node->numAttachedObjects() > 1) { //if there is an entity attached (other than the selection marker)
				Ogre::Entity *entity = (Ogre::Entity*)node->getAttachedObject(0);
				node->detachObject(entity);
				sceneMgr->destroyEntity(entity);
			}
			Ogre::Entity *newCube = sceneMgr->createEntity(Ogre::StringConverter::toString(x) + Ogre::StringConverter::toString(y) + Ogre::StringConverter::toString(z) + "_cube", "meshCube");
			node->attachObject(newCube);
		} else {
			//the pixel have been deleted
			if (node->numAttachedObjects() > 1) { //if there is an entity attached (other than the selection marker)
				Ogre::Entity *entity = (Ogre::Entity*)node->getAttachedObject(0);
				node->detachObject(entity);
				sceneMgr->destroyEntity(entity);
			}
		}
	}
}

void EditorScene::rotateMap(Ogre::Radian x, Ogre::Radian y)
{
	LOG(INFO) << "Rotating the map";
	rotY = x;
	rotX = y;		
}

void EditorScene::update(unsigned long timeLapsed)
{
	camera->update(timeLapsed);
	//rotate map
	mapPitchNode->pitch(rotX*timeLapsed*0.0005);
	mapYawNode->yaw(rotY*timeLapsed*0.0005);
	rotY = 0;
	rotX = 0;
}

//~ void EditorScene::loadGUI(Ogre::RenderWindow* window)
//~ {	
	//~ button = myGUI->findWidget<MyGUI::Button>("bt_clear");
	//~ button->eventMouseButtonClick = MyGUI::newDelegate(this, &EditorScene::clearRequested);
//~ 
//~ 

//~ 
//~ }

//~ void EditorScene::clearRequested(MyGUI::WidgetPtr sender)
//~ {
	//~ //If there is a cube selected and if not void, destroy it
	//~ LOG(INFO) << "Trying to clear a cube";
//	voxelMap->deleteVoxel(0,0,0);
//~ 
//~ }

GuiSystem::GuiSystem(EditorScene *_scene, Ogre::RenderWindow *window, Ogre::SceneManager *sceneMgr):
	moveMapMode(false),
	scene(_scene),	
	isClearMode(false),
	selectedNode(NULL)
{
	LOG(INFO) << "Initialize GUI";
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(window, sceneMgr);
	myGUI = new MyGUI::Gui();	
	myGUI->initialise();
		
	MyGUI::Gui::getInstance().load("colour_slider_skin.xml");
	MyGUI::LayoutManager::getInstance().loadLayout("gui.xml");

	EventManager::subscribe("mouseMoved", this, &GuiSystem::myGuiUpdate);
	EventManager::subscribe("mousePressed", this, &GuiSystem::myGuiUpdate);
	EventManager::subscribe("mouseReleased", this, &GuiSystem::myGuiUpdate);
	EventManager::subscribe("mouseMoved", this, &GuiSystem::moveMap);
	EventManager::subscribe("rightClick", this, &GuiSystem::moveMap);
	EventManager::subscribe("mouseMoved", this, &GuiSystem::checkSelection);
	EventManager::subscribe("leftClick", this, &GuiSystem::modifyCube);

	MyGUI::ButtonPtr button = myGUI->findWidget<MyGUI::Button>("bt_exit");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &GuiSystem::exitRequested);
	LOG(INFO) << "GUI successfukky initialized";
}

GuiSystem::~GuiSystem()
{
	myGUI->shutdown();
	delete myGUI;
	myGUI = 0;
	mPlatform->shutdown();
	delete mPlatform;
	mPlatform = 0;	
}

void GuiSystem::myGuiUpdate(std::string eventName, EventManager::Arguments args)
{
	//TODO add other events
	LOG(INFO) << "Updating GUI state";
	if (eventName == "mouseMoved") {
		MyGUI::InputManager::getInstance().injectMouseMove(boost::any_cast<int>(args["Xabs"]), boost::any_cast<int>(args["Yabs"]), boost::any_cast<int>(args["Zabs"]));
	} else if (eventName == "mousePressed") {
		MyGUI::InputManager::getInstance().injectMousePress(boost::any_cast<int>(args["Xabs"]), boost::any_cast<int>(args["Yabs"]), MyGUI::MouseButton::Enum(boost::any_cast<int>(args["id"])));
	} else if (eventName == "mouseReleased") {
		MyGUI::InputManager::getInstance().injectMouseRelease(boost::any_cast<int>(args["Xabs"]), boost::any_cast<int>(args["Yabs"]), MyGUI::MouseButton::Enum(boost::any_cast<int>(args["id"])));
	}
}

void GuiSystem::exitRequested(MyGUI::WidgetPtr sender)
{
	LOG(INFO) << "Trying to exit";
	EventManager::Arguments arg;
	arg["pressed"] = false;
	EventManager::sendEvent("quitGame", arg);
}

void GuiSystem::moveMap(std::string eventName, EventManager::Arguments args)
{
	if (eventName == "rightClick") {		
		if (boost::any_cast<bool>(args["pressed"])) {
			moveMapMode = true;
		}
		if (!boost::any_cast<bool>(args["pressed"])) {
			moveMapMode = false;
		}
		return;
	}
	if (eventName == "mouseMoved" && moveMapMode) {
		scene->rotateMap(Ogre::Radian(boost::any_cast<int>(args["Xrel"]))*5, Ogre::Radian(boost::any_cast<int>(args["Yrel"]))*5);
	}
}

void GuiSystem::checkSelection(std::string eventName, EventManager::Arguments args)
{
	int height = scene->getCamera()->getCameraPtr()->getViewport()->getActualHeight();
	int width = scene->getCamera()->getCameraPtr()->getViewport()->getActualWidth();
	Ogre::Ray ray = scene->getCamera()->getCameraPtr()->getCameraToViewportRay(boost::any_cast<int>(args["Xabs"]) / float(width),boost::any_cast<int>(args["Yabs"]) / float(height));
	Ogre::RaySceneQuery *rayQuery = scene->getSceneMgr()->createRayQuery(ray);
	rayQuery->setSortByDistance(true);
	
	Ogre::RaySceneQueryResult res = rayQuery->execute();
	Ogre::RaySceneQueryResult::iterator it;

	if (res.begin() == res.end()) {
		if (selectedNode != NULL) {
			selectedNode->getAttachedObject(selectedNode->getName() + "_marker")->setVisible(false);
			selectedNode = NULL;
		}

	} else {
		for (it = res.begin(); it < res.end(); ++it) {
			if (it->movable) {
				LOG(INFO) << "Found something under the mouse: " << it->movable->getName();
				if (it->movable->getName() != "editorAxis") {
					if (selectedNode != it->movable->getParentSceneNode()) {
						if (selectedNode != NULL) {
							selectedNode->getAttachedObject(selectedNode->getName() + "_marker")->setVisible(false);
						}
						selectedNode = it->movable->getParentSceneNode();
						it->movable->setVisible(true);
					}
				}
			}
		}
	}
	
}

void GuiSystem::modifyCube(std::string eventName, EventManager::Arguments args)
{
	if (boost::any_cast<bool>(args["pressed"])) {
		if (selectedNode) {
			LOG(INFO) << "Trying to modify a cube (destroy/create/modify)";
			if (isClearMode) {
				//clear cube
				if (scene->getSceneMgr()->hasEntity(selectedNode->getName() + "_cube")) {
					scene->getVoxelMap()->deleteVoxel(selectedNode->getName());	
				}
			} else {
				//new/modify cube TODO SHiTTY if name longer than xyz, ex: xxyyzzz ???? plein de trucs zarb
				//~ LOG(INFO) << "x=" << selectedNode->getName()[0] << " y=" << selectedNode->getName()[1] << " z=" << selectedNode->getName()[2];
				//~ char xc = selectedNode->getName()[0];
				//~ char yc = selectedNode->getName()[1];
				//~ char zc = selectedNode->getName()[2];
				//~ 
				//~ int x = atoi(&xc);
				//~ int y = atoi(&yc);
				//~ int z = atoi(&zc);
				//~ LOG(INFO) << "x=" << x << " y=" << y << " z=" << z;
				Coordinates coord = stringToCoordinates(selectedNode->getName());
				VoxelColored *vox = new VoxelColored(22);
				scene->getVoxelMap()->setVoxel(vox, coord.x, coord.y, coord.z);
			}
		}
	}
}

Coordinates GuiSystem::stringToCoordinates(std::string coordString)
{
	Coordinates coord;
	
	coord.x = 1;
	coord.y = 1;
	coord.z = 1;
	
	return coord;
} 
