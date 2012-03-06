#include "guiSystem.h"


GuiSystem::GuiSystem(Ogre::RenderWindow *window, Ogre::SceneManager *sceneMgr):
	moveMapMode(false)
	//~ scene(_scene)
{
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

	MyGUI::ButtonPtr button = myGUI->findWidget<MyGUI::Button>("bt_exit");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &GuiSystem::exitRequested);

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
	//~ if (eventName == "mouseMoved" && moveMapMode) {
		//~ int x = Ogre::Degree(Ogre::Radian(boost::any_cast<int>(args["Xrel"]))*5);
		//~ int y = Ogre::Degree(Ogre::Radian(boost::any_cast<int>(args["Yrel"]))*5);
		//~ scene->rotateMap(x, y);
	//~ }
}
