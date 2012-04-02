#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include <glog/logging.h>
#include <MYGUI/MyGUI.h>

class SidePanel
{
	public:
		SidePanel(MyGUI::Gui *myGUI);
		~SidePanel();
	
	private:
		void notifyEditTextChange(MyGUI::Edit *sender);	

		MyGUI::Gui *myGUI_;
		MyGUI::Colour currentColour;
		MyGUI::Edit *editHex;
		MyGUI::Edit *editRed;
		MyGUI::Edit *editGreen;
		MyGUI::Edit *editBlue;

};

#endif /* SIDEPANEL_H */ 
