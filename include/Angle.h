/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#ifndef ANGLE_H
#define ANGLE_H

#include <QtCore/qpoint.h>
#include <OGRE/OgreMath.h>

class Angle {
public:
    Angle()
        : mIndex(0)
    {
    }

    Angle(char index)
        : mIndex(index & 0x03)
    {
    }

    Angle(Ogre::Degree degrees) {
        from(degrees);
    }

    Angle(Ogre::Radian radians) {
        from(radians);
    }

    Angle& operator+=(const Angle& angle) {
        mIndex += angle.mIndex;
        mIndex &= 0x03;
        return *this;
    }

    Angle& operator-=(const Angle& angle) {
        mIndex -= angle.mIndex;
        mIndex &= 0x03;
        return *this;
    }

    Angle& operator++() {
        return *this += Angle(1);
    }

    Angle& operator++(int) {
        return *this += Angle(1);
    }

    Angle& operator--() {
        return *this -= Angle(1);
    }

    Angle& operator--(int) {
        return *this -= Angle(1);
    }

    const Angle operator+(const Angle& angle) const {
        Angle result = *this;
        result += angle;
        return result;
    }

    const Angle operator-(const Angle& angle) const {
        Angle result = *this;
        result -= angle;
        return result;
    }

    Ogre::Degree toDegrees() const {
        return Ogre::Degree(mIndex * 90.0f);
    }

    Ogre::Radian toRadians() const {
        return Ogre::Radian(toDegrees());
    }

    void rotate(QPoint& point) const {
        static int matrices[4][2][2] = {{{1, 0}, {0, 1}}, {{0, -1}, {1, 0}}, {{-1, 0}, {0, -1}}, {{0, 1}, {-1, 0}}};

        QPoint newPoint;
        newPoint.setX(matrices[mIndex][0][0] * point.x() + matrices[mIndex][0][1] * point.y());
        newPoint.setY(matrices[mIndex][1][0] * point.x() + matrices[mIndex][1][1] * point.y());
        point = newPoint;
    }

private:
    char mIndex;

    void from(Ogre::Degree degrees) {
        static float multiplier = 1 / 90.0f;
        mIndex = static_cast<char>((degrees.valueDegrees() + 45.0f) * multiplier);
        mIndex &= 0x03;
    }

    void from(Ogre::Radian radians) {
        from(Ogre::Degree(radians));
    }
};

#endif