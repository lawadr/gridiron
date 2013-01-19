/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#pragma once

#include <QtGui/qwidget.h>
#include <OGRE/Ogre.h>

class OgreWidget : public QWidget {
    Q_OBJECT

public:
    OgreWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~OgreWidget();

    QPaintEngine* paintEngine() const;

protected:
    Ogre::RenderWindow* renderWindow() const;
    bool isInitialised() const;

    virtual void initialise() = 0;

    void paintEvent(QPaintEvent* paintEvent);
    void resizeEvent(QResizeEvent* resizeEvent);
    void showEvent(QShowEvent* showEvent);

private:
    Ogre::RenderWindow* mRenderWindow;
};
