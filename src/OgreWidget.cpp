/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "OgreWidget.h"
#include <QtGui/qevent.h>

OgreWidget::OgreWidget(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags | Qt::MSWindowsOwnDC)
    , mRenderWindow(0)
{
    QPalette colourPalette = palette();
    colourPalette.setColor(QPalette::Active, QPalette::WindowText, Qt::black);
    colourPalette.setColor(QPalette::Active, QPalette::Window, Qt::black);
    setPalette(colourPalette);

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_OpaquePaintEvent);

    setFocusPolicy(Qt::StrongFocus);
}

OgreWidget::~OgreWidget() {
    if (isInitialised()) {
        Ogre::Root* root = Ogre::Root::getSingletonPtr();
        root->destroyRenderTarget(mRenderWindow);
    }
}

QPaintEngine* OgreWidget::paintEngine() const {
    return 0;
}

Ogre::RenderWindow* OgreWidget::renderWindow() const {
    return mRenderWindow;
}

void OgreWidget::paintEvent(QPaintEvent* paintEvent) {
    if (isInitialised()) {
        mRenderWindow->update();
    }
}

void OgreWidget::resizeEvent(QResizeEvent* resizeEvent) {
    if (isInitialised()) {
        mRenderWindow->resize(width(), height());
        mRenderWindow->windowMovedOrResized();
    }
}

void OgreWidget::showEvent(QShowEvent* showEvent) {
    if (!isInitialised()) {
        static Ogre::NameGenerator windowNameGenerator("OgreWidgetWindow");

        Ogre::String title = windowNameGenerator.generate();
        unsigned int sizeX = width();
        unsigned int sizeY = height();
        bool fullScreen = false;
        unsigned int windowHandle = reinterpret_cast<unsigned int>(effectiveWinId());
        Ogre::NameValuePairList parameters;
        parameters["FSAA"] = "8";
        parameters["vsync"] = "true";
        parameters["left"] = Ogre::StringConverter::toString(x());
        parameters["top"] = Ogre::StringConverter::toString(y());
        parameters["externalWindowHandle"] = Ogre::StringConverter::toString(windowHandle);

        Ogre::Root* root = Ogre::Root::getSingletonPtr();
        mRenderWindow = root->createRenderWindow(title, sizeX, sizeY, fullScreen, &parameters);
        mRenderWindow->setAutoUpdated(false);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        initialise();
    }
}

bool OgreWidget::isInitialised() const {
    return mRenderWindow;
}
