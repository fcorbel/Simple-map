#include "oisInput.h"


std::string OIS_input::module_vers = "0.1";

OIS_input::OIS_input()
{
	std::cout << "== creation OIS_input ==" << std::endl;
	m_ois = NULL;
	mKeyboard = NULL;
	mMouse =NULL;
	keymap = NULL;
	
		
}

OIS_input::~OIS_input()
{
	std::cout << "== delete OIS_input ==" << std::endl;
	if (m_ois) {
		if (mKeyboard) {
			m_ois->destroyInputObject(mKeyboard);
			mKeyboard = 0;
		}
		if (mMouse) {
			m_ois->destroyInputObject(mMouse);
			mMouse = 0;
		}
		OIS::InputManager::destroyInputSystem(m_ois);
	}
	keymap = NULL;
	std::cout << "== delete OIS_input complete==" << std::endl;

}

void OIS_input::version()
{
	std::cout << "version: " << module_vers << std::endl;
}

bool OIS_input::initializeWithOgre( std::string windowData )
{
/*windowData comes from Ogre::RenderWindow, should be a string
*/
	OIS::ParamList pl;
	pl.insert(std::make_pair(std::string("WINDOW"),windowData));
	m_ois = OIS::InputManager::createInputSystem( pl );

	mKeyboard = (OIS::Keyboard*)m_ois->createInputObject( OIS::OISKeyboard, true);
	mMouse = (OIS::Mouse*)m_ois->createInputObject( OIS::OISMouse, true);
	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
	
	//for the GUI
	const OIS::MouseState &mouseState = mMouse->getMouseState();
	mouseState.width = 800;
	mouseState.height = 600;

	return true;
}

void OIS_input::setKeymap(Options* new_keymap)
{
	keymap = new_keymap;
}

void OIS_input::capture()
{	
	//Call capture to update both buffered and unbuffered devices
	if(mMouse != NULL) {
		mMouse->capture();
	} else {
		throw std::logic_error("mouse not initialized");
	}
	if(mKeyboard != NULL) {
		mKeyboard->capture();
	} else {
		throw std::logic_error("keyboard not initialized");
	}
}

bool OIS_input::keyPressed(const OIS::KeyEvent &evt)
{
	std::string key_name;
	key_name = ((OIS::Keyboard*)(evt.device))->getAsString(evt.key);
	LOG(INFO) << "key pressed: " << key_name;	
	std::string message;
	message = keymap->getValue<std::string>(key_name);
	if (message != "") {
		EventManager::Arguments arg;
		arg["pressed"] = true;
		EventManager::sendEvent(message, arg);
	} else {
		EventManager::Arguments arg;
		arg["key"] = (int)evt.key;
		arg["text"] = evt.text;
		std::string message("keyPressed");
		EventManager::sendEvent(message, arg);		
	}
	return true;
}

bool OIS_input::keyReleased(const OIS::KeyEvent &evt)
{
	std::string key_name;
	key_name = ((OIS::Keyboard*)(evt.device))->getAsString(evt.key);
	LOG(INFO) << "key released: " << key_name;	
	std::string message;
	message = keymap->getValue<std::string>(key_name);
	if (message != "") {
		EventManager::Arguments arg;
		arg["pressed"] = false;
		EventManager::sendEvent(message, arg);
	} else {
		EventManager::Arguments arg;
		arg["key"] = (int)evt.key;
		arg["text"] = evt.text;
		std::string message("keyReleased");
		EventManager::sendEvent(message, arg);		
	}

	return true;
}

bool OIS_input::mouseMoved(const OIS::MouseEvent &evt)
{
	LOG(INFO) << "Mouse moved";
	const OIS::MouseState& s = evt.state;
	EventManager::Arguments arg;
	arg["Xabs"] = s.X.abs;
	arg["Yabs"] = s.Y.abs;
	arg["Zabs"] = s.Z.abs;
	arg["Xrel"] = s.X.rel;
	arg["Yrel"] = s.Y.rel;
	arg["Zrel"] = s.Z.rel;
	EventManager::sendEvent("mouseMoved", arg);

	return true;
}

bool OIS_input::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	//for MyGUI
	const OIS::MouseState& s = evt.state;
	EventManager::Arguments arg;
	arg["Xabs"] = s.X.abs;
	arg["Yabs"] = s.Y.abs;
	arg["Zabs"] = s.Z.abs;
	arg["Xrel"] = s.X.rel;
	arg["Yrel"] = s.Y.rel;
	arg["Zrel"] = s.Z.rel;
	arg["id"] = (int)id;
	EventManager::sendEvent("mousePressed", arg);

	LOG(INFO) << "Mouse button pressed: " << id;
	std::string buttonName;
	switch(id) {
		case 0:
			buttonName = "Mb_Left";
			break;
		case 1:
			buttonName = "Mb_Right";
			break;
		default:
			break;
	}
	std::string message;
	message = keymap->getValue<std::string>(buttonName);
	if (message != "") {
		EventManager::Arguments arg;
		arg["pressed"] = true;
		EventManager::sendEvent(message, arg);
	}

	
	return true;
}

bool OIS_input::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	//for MyGUI
	const OIS::MouseState& s = evt.state;
	EventManager::Arguments arg;
	arg["Xabs"] = s.X.abs;
	arg["Yabs"] = s.Y.abs;
	arg["Zabs"] = s.Z.abs;
	arg["Xrel"] = s.X.rel;
	arg["Yrel"] = s.Y.rel;
	arg["Zrel"] = s.Z.rel;
	arg["id"] = (int)id;
	EventManager::sendEvent("mouseReleased", arg);

	LOG(INFO) << "Mouse button released: " << id;
	std::string buttonName;
	switch(id) {
		case 0:
			buttonName = "Mb_Left";
			break;
		case 1:
			buttonName = "Mb_Right";
			break;
		default:
			break;
	}
	std::string message;
	message = keymap->getValue<std::string>(buttonName);
	if (message != "") {
		EventManager::Arguments arg;
		arg["pressed"] = false;
		EventManager::sendEvent(message, arg);
	}
	return true;
}
