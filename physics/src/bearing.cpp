#include "include/bearing.h"


Bearing::Bearing(qreal angle)
{
    mAngle = angle;
    correct();
}

qreal Bearing::operator+=(qreal delta)
{
    mAngle += delta;
    correct();
    return mAngle;
}

qreal Bearing::operator+=(const Bearing& other)
{
    mAngle += other.mAngle;
    correct();
    return mAngle;
}

qreal Bearing::operator-=(qreal delta)
{
    mAngle -= delta;
    correct();
    return mAngle;
}

qreal Bearing::operator-=(const Bearing& other)
{
    mAngle -= other.mAngle;
    correct();
    return mAngle;
}

bool Bearing::withinArc(qreal arcCentre, qreal arcWidth)
{
    qreal delta = mAngle - arcCentre - arcWidth*0.5;
    if (delta > M_PI) delta = -m2Pi + delta;
    if (delta < -M_PI) delta = m2Pi + delta;

    return qAbs(delta) <= arcWidth*0.5;
}

qreal Bearing::getDelta(qreal angle)
{
    qreal delta = angle - mAngle;
    if (delta > M_PI) return -m2Pi + delta;
    if (delta < -M_PI) return m2Pi + delta;
    return delta;
}

qreal Bearing::getDelta(const Bearing& other)
{
    qreal delta = other.mAngle - mAngle;
    if (delta > M_PI) return -m2Pi + delta;
    if (delta < -M_PI) return m2Pi + delta;
    return delta;
}

void Bearing::correct()
{
    while (mAngle > m2Pi) mAngle -= m2Pi;
    while (mAngle < 0) mAngle += m2Pi;
}

qreal Bearing::correct(qreal angle)
{
    while (angle > m2Pi) angle -= m2Pi;
    while (angle < 0) angle += m2Pi;
    return angle;
}