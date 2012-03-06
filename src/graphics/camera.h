#ifndef CAMERA_H
#define CAMERA_H

#include "OGRE/Ogre.h"
#include <iostream>
#include "../eventManager.h"


class Camera
{
	public:
		Camera(Ogre::String cam_name, Ogre::SceneManager* scene_mgr, Ogre::RenderWindow* window);
		~Camera();

		void update(float speed_factor);
		Ogre::Camera* getCameraPtr();
		void setPosition(float x, float y, float z);
		void rotate(int x, int y, int z);
		void moveForward(std::string, EventManager::Arguments);
		void moveBackward(std::string, EventManager::Arguments);
		void moveLeft(std::string, EventManager::Arguments);
		void moveRight(std::string, EventManager::Arguments);
		void translate(Ogre::Vector3 new_direction);
		void lookAt(float x, float y, float z);
		
		void changeBackgroundColour(std::string, EventManager::Arguments);


	private:
		Ogre::Viewport* viewport;
		Ogre::SceneNode* cameraNode;
		Ogre::SceneNode* cameraYawNode;
		Ogre::SceneNode* cameraPitchNode;
		Ogre::SceneNode* cameraRollNode;
		Ogre::Camera* camera;

		Ogre::SceneManager* scene;

		Ogre::Radian rotX, rotY;
		Ogre::Vector3 direction;
		//speed of mouvements
		float rot_speed;
		float mvt_speed;

		//to manage events unregistration
		std::vector<int> events;
};

#endif
