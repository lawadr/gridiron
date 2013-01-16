/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "SceneEvent.h"

const QEvent::Type SceneMouseEvent::MOVE_TYPE = static_cast<QEvent::Type>(QEvent::registerEventType());
const QEvent::Type SceneMouseEvent::PRESS_TYPE = static_cast<QEvent::Type>(QEvent::registerEventType());
const QEvent::Type SceneMouseEvent::RELEASE_TYPE = static_cast<QEvent::Type>(QEvent::registerEventType());
const QEvent::Type SceneMouseEvent::DOUBLE_CLICK_TYPE = static_cast<QEvent::Type>(QEvent::registerEventType());