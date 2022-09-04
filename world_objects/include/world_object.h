#include "include/vector.h"
#include "include/sensor.h"
#include "include/faction.h"
#include "include/rotation_controller.h"

#include <QtWidgets>

#pragma once


/**
 * Class that every world object must inherit from. I.e. Anything that moves in
 * the world space.
 */
class WorldObject : public QWidget
{
    Q_OBJECT
public:
    friend class SignalTrackProcessor;

    /**
     * Initialises a new object in the world.
     *
     * @param faction - Blue for player, Red for hostile, Green for civilian
     * @param uid - Unique ID
     */
    WorldObject(Faction faction, uint32_t uid) : mFaction(faction), mId(uid) {}
    ~WorldObject() override = default;

    Vector getVelVector() const { return mV; }
    Vector getAccVector() const { return mA; }
    Vector getPosVector() const { return mP; }
    QPointF getPoint() const { return {mP.x(), mP.y()}; }
    qreal getAtan2() const { return mAtan2; }
    QGraphicsItem* getTacticalGraphicsItem() { return mTacticalGraphicsItem; }
    uint32_t getId() const { return mId; }
    QVector<std::shared_ptr<Sensor>> getSensors() const { return mSensors; }

    virtual void updatePosition(qreal deltaT, QPointF offset) {}

    void updateSensors()
    {
        for (const auto& s : mSensors) s->update({mP.x(), mP.y()}, mAtan2);
    }

    void rotate(qreal degrees)
    {
        if (degrees > 0)
            mRotationController.commandNewBearing(degrees*2.0*M_PI/360.0, mMaxRightRotateAcc, mMaxLeftRotateAcc);
        else
            mRotationController.commandNewBearing(degrees*2.0*M_PI/360.0, mMaxLeftRotateAcc, mMaxRightRotateAcc);
    }

Q_SIGNALS:
    void handleAddSensors(QVector<std::shared_ptr<Sensor>>);

protected:
    Faction mFaction;
    uint32_t mId;
    qreal mAtan2 = 0; // Bearing (rads)
    qreal mRotV = 0; // Rotational velocity
    qreal mRotA = 0; // Rotational acceleration
    Vector mV = Vector(0, 0); // Velocity vector
    Vector mA = Vector(0, 0); // Acceleration vector
    Vector mP = Vector(0, 0); // Position vector
    qreal mMaxRightRotateAcc = 0;
    qreal mMaxLeftRotateAcc = 0;

    QVector<std::shared_ptr<Sensor>> mSensors;
    QGraphicsItem* mTacticalGraphicsItem = nullptr;
    RotationController mRotationController = RotationController(mRotV);
};