#ifndef OIS_INPUT_H
#define OIS_INPUT_H

#include <iostream>
#include <string>
#include <stdexcept>

#include "OIS/OISInputManager.h"
#include "OIS/OISException.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISMouse.h"
#include "OIS/OISEvents.h"

#include <glog/logging.h>
#include "../options.h"
#include "../eventManager.h"



class OIS_input :
	public OIS::KeyListener,
	public OIS::MouseListener
{
	public:
		OIS_input();
		virtual ~OIS_input();
		static void version();

		bool initializeWithOgre(std::string);
		void setKeymap(Options*);
		void capture();

		// KeyListener
		virtual bool keyPressed(const OIS::KeyEvent&);
		virtual bool keyReleased(const OIS::KeyEvent&);

		// MouseListener
		virtual bool mouseMoved(const OIS::MouseEvent&);
		virtual bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID);
		virtual bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID);

	private:
		static std::string module_vers;
		OIS::InputManager *m_ois;
		OIS::Keyboard *mKeyboard;
		OIS::Mouse *mMouse;
		Options *keymap;

};

#endif

