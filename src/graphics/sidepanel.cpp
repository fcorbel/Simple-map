#include "sidepanel.h"


SidePanel::SidePanel(MyGUI::Gui* myGUI):
	currentColour(MyGUI::Colour::Green),
	myGUI_(myGUI)
{
	MyGUI::Gui::getInstance().load("colour_slider_skin.xml");
	MyGUI::LayoutManager::getInstance().loadLayout("gui.xml");

	editHex = myGUI_->findWidget<MyGUI::Edit>("edit_Hex");
	editHex->eventEditTextChange = MyGUI::newDelegate(this, &SidePanel::notifyEditTextChange);		
	
	


}

SidePanel::~SidePanel()
{
	
}

void SidePanel::notifyEditTextChange(MyGUI::Edit *sender)
{
	//TODO for HEX: check value->if unvalid: red background
	//~ LOG(INFO) << sender->getName();
	if (sender->getName() == "edit_Hex") {
		std::string value = sender->getCaption();
		if (true) {
			sender->setTextColour(MyGUI::Colour::Red);
		}	
	} else {
	//TODO do for RGB	
	}	
	
	LOG(INFO) << "Value in edit text changed";
}
