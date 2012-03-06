#include "graphicsOgre.h"

GraphicsOgre::GraphicsOgre()
{
	LOG(INFO) << "Initialization of GraphicsOgre";
	
	timer.reset();
	lastFpsCalcul = 0.0;
	currentTime = 0.0;
	nbFrames = 0;
	fpsLimit = 90;
	timeBetweenFrames = 1000 / fpsLimit;
	lastFrameTime = 0.0;

	setPluginFolder( "/usr/local/lib/OGRE" );
	ogre = new Ogre::Root( "", "" );
	window = NULL;
	scene = NULL;
}

GraphicsOgre::~GraphicsOgre()
{
	LOG(INFO) << "Shuting down GraphicsOgre";
	if (scene) {
		delete scene;
	}
	if (window) {
		LOG(INFO) << "delete window";
		window = NULL;
	}
	LOG(INFO) << "delete ogre";
	delete ogre;
	LOG(INFO) << "Shuting down complete";
}

bool GraphicsOgre::initOgre(Options *config)
{
	std::cout << "== initialization of the graphics ==" << std::endl;
	//use OpenGL
	ogre->loadPlugin(getPluginFolder() + "/" + "RenderSystem_GL");
	
	const Ogre::RenderSystemList renderList = ogre->getAvailableRenderers();
	Ogre::RenderSystemList::const_iterator r_it;
	r_it = renderList.begin();
	ogre->setRenderSystem(*r_it);
	ogre->initialise(false);
	
	//create manualy the window
	Ogre::NameValuePairList opts;
	opts.insert(Ogre::NameValuePairList::value_type("vsync", "false"));
	window = ogre->createRenderWindow(config->getValue<std::string>("appname"), config->getValue<int>("width"), config->getValue<int>("height"), false, &opts);
	
	//init resources
	defineResources();
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("General");
	
	//load a scene
	scene = new EditorScene(ogre, window);
	
	return true;
}

bool GraphicsOgre::showAvailableRenderers()
{
	const Ogre::RenderSystemList renderList = ogre->getAvailableRenderers();
	Ogre::RenderSystemList::const_iterator r_it;
	r_it = renderList.begin();
	std::cout << "== Liste des render system ==" << std::endl;
	Ogre::RenderSystem *render = NULL;
	while (r_it != renderList.end()) {
		render = *r_it;
		std::cout << render->getName() << std::endl;
		++r_it;
	}

	return true;
}

std::string GraphicsOgre::getPluginFolder()
{
	return pluginFolder;
}

bool GraphicsOgre::setPluginFolder( std::string path )
{
	pluginFolder = path;
	return true;
}

std::string GraphicsOgre::getWindowAttribute()
{
	if (!window) {
		std::cout << "Window not created, unable to get its attributes" << std::endl;
		return "";
	}
	size_t windowHandle;
	window->getCustomAttribute("WINDOW", &windowHandle);
	return Ogre::StringConverter::toString(windowHandle);
}

EditorScene* GraphicsOgre::getScene()
{
	if (!scene) { return NULL; }
	return scene;
}

void GraphicsOgre::renderFrame()
{
	Ogre::WindowEventUtilities::messagePump();
	unsigned long timeLapsed = timer.getMilliseconds() - lastFrameTime;
	if (timeLapsed >= timeBetweenFrames) {
		if (scene) {
			scene->update(timeLapsed);
		}
		ogre->renderOneFrame();
		++nbFrames;
		lastFrameTime = timer.getMilliseconds();
	}
	//calcul framerate every 2sec
	currentTime = timer.getMilliseconds();
	if ((currentTime - lastFpsCalcul) > 2000) {
		int fps = nbFrames*1000 / (currentTime - lastFpsCalcul); // *1000 because time is in ms, we want it in s
		LOG(INFO) << "FPS: " << fps;
		lastFpsCalcul = currentTime;
		nbFrames = 0;
	}
}

void GraphicsOgre::defineResources()
{
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
	Ogre::String resource_path = "./";
	cf.load(resource_path + "resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
