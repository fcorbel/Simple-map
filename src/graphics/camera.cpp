#include "camera.h"

Camera::Camera(Ogre::String cam_name, Ogre::SceneManager* scene_mgr, Ogre::RenderWindow* window)
{
	scene = scene_mgr;

	cameraNode = scene_mgr->getRootSceneNode()->createChildSceneNode();
	cameraYawNode = cameraNode->createChildSceneNode();
	cameraPitchNode = cameraYawNode->createChildSceneNode();
	cameraRollNode = cameraPitchNode->createChildSceneNode();
	camera = scene_mgr->createCamera(cam_name);
	cameraRollNode->attachObject(camera);

	viewport = window->addViewport(camera);
	viewport->setBackgroundColour(Ogre::ColourValue(0.25,0.25,0.25));
	camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));

	rotX = 0;
	rotY = 0;
	direction = Ogre::Vector3::ZERO;

	rot_speed = 0.2;
	mvt_speed = 0.6;
	
	events.push_back(EventManager::subscribe("moveForward", this, &Camera::moveForward));
	events.push_back(EventManager::subscribe("moveBackward", this, &Camera::moveBackward));
	events.push_back(EventManager::subscribe("moveLeft", this, &Camera::moveLeft));
	events.push_back(EventManager::subscribe("moveRight", this, &Camera::moveRight));
	events.push_back(EventManager::subscribe("changeBackColour", this, &Camera::changeBackgroundColour));

}

Camera::~Camera()
{
	cameraNode->removeAndDestroyAllChildren();
	scene->destroyCamera(camera);
	
	//unregister all events
	std::vector<int>::iterator it;
	for (it=events.begin(); it<events.end(); ++it) {
		EventManager::unSubscribe(*it);
	}
}

void Camera::update(float time_lapsed)
{
	//~ speed_factor = 0.01;
	//update rotation of the camera
	//~ LOG(INFO) << "rotX: " << rotX << " corrected: " << rotX*time_lapsed;
	cameraYawNode->yaw(rotX*time_lapsed);
	cameraPitchNode->pitch(rotY*time_lapsed);
//	std::cout << "rotX: " << rotX << " rotY: " << rotY << std::endl;
	rotX = 0;
	rotY = 0;
	//update position of the camera
	translate(direction*time_lapsed);
}

Ogre::Camera* Camera::getCameraPtr()
{
	return camera;
}

void Camera::setPosition(float x, float y, float z)
{
	cameraNode->setPosition(x, y, z);
}

void Camera::rotate(int x, int y, int z)
{
	rotX = Ogre::Degree(x*rot_speed);
	rotY = Ogre::Degree(y*rot_speed);
}

void Camera::moveForward(std::string event_name, EventManager::Arguments arg)
{
	if (boost::any_cast<bool>(arg["pressed"]) && (direction.z != -mvt_speed)) {
		direction.z += -mvt_speed;
	} else {
		direction.z += mvt_speed;
	}	
}

void Camera::moveBackward(std::string event_name, EventManager::Arguments arg)
{	
	if (boost::any_cast<bool>(arg["pressed"]) && (direction.z != mvt_speed)) {
		direction.z += mvt_speed;
	} else {
		direction.z += -mvt_speed;
	}
}

void Camera::moveLeft(std::string event_name, EventManager::Arguments arg)
{
	if (boost::any_cast<bool>(arg["pressed"]) && (direction.x != -mvt_speed)) {
		direction.x += -mvt_speed;
	} else {
		direction.x += mvt_speed;
	}
}

void Camera::moveRight(std::string event_name, EventManager::Arguments arg)
{
	if (boost::any_cast<bool>(arg["pressed"]) && (direction.x != mvt_speed)) {
		direction.x += mvt_speed;
	} else {
		direction.x += -mvt_speed;
	}
}

void Camera::translate(Ogre::Vector3 new_direction)
{
	cameraNode->translate(cameraYawNode->getOrientation()*cameraPitchNode->getOrientation()*new_direction, Ogre::SceneNode::TS_LOCAL);
}

void Camera::lookAt(float x, float y, float z)
{
	//~ Ogre::Vector3 target(x,y,z);
	//~ Ogre::Vector3 position = cameraNode->getPosition();
	//maybe shitty
	//~ camera->lookAt(x,y,z);	
}

void Camera::changeBackgroundColour(std::string event_name, EventManager::Arguments args)
{
	if (boost::any_cast<bool>(args["pressed"]) == false) {
		return;
	}
	if (viewport->getBackgroundColour() == Ogre::ColourValue(0.25,0.25,0.25)) {
		viewport->setBackgroundColour(Ogre::ColourValue(1,0,1));
	} else {
		viewport->setBackgroundColour(Ogre::ColourValue(0.25,0.25,0.25));
	}
}
