/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "EditorWindow.h"

#include <QtGui/qapplication.h>
#include <QtCore/qsettings.h>

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	app.setOrganizationName("TestOrganisation");
	app.setApplicationName("TestApplication");

	QSettings::setDefaultFormat(QSettings::IniFormat);

	QIcon::setThemeSearchPaths(QStringList(QDir::current().absoluteFilePath("icons")));
	QIcon::setThemeName("Oxygen");

	Ogre::Root* root = new Ogre::Root;
	#if	OGRE_DEBUG_MODE
		root->loadPlugin("RenderSystem_GL_d");
		//root->loadPlugin("RenderSystem_Direct3D9_d");
	#else
		root->loadPlugin("RenderSystem_GL");
		//root_->loadPlugin("RenderSystem_Direct3D9");
	#endif
	const Ogre::RenderSystemList& renderSystemList = root->getAvailableRenderers();
	assert(!renderSystemList.empty());
	root->setRenderSystem(renderSystemList.front());
	root->initialise(false);

	Ogre::ResourceGroupManager* resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
	resourceGroupManager->addResourceLocation("data/Meshes", "FileSystem");
	resourceGroupManager->addResourceLocation("data/Materials", "FileSystem");
	resourceGroupManager->addResourceLocation("data/Textures", "FileSystem");
	resourceGroupManager->addResourceLocation("data/Programs", "FileSystem");

	Ogre::MaterialManager* materialManager = Ogre::MaterialManager::getSingletonPtr();
	materialManager->setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	materialManager->setDefaultAnisotropy(8);

	EditorWindow* editor = new EditorWindow();
	app.setWindowIcon(QIcon::fromTheme("application-icon"));
	editor->show();

	int result = app.exec();

	delete editor;
	delete root;

	return result;
}
