#include "include/vector.h"
#include "blur.h"
#include "include/engine.h"
#include "include/component.h"
#include "player_ship_item.h"

#include <QGraphicsItem>
#include <QtMath>

#pragma once

class PlayerShip : public QObject {
    Q_OBJECT
public:
    PlayerShip();

    enum RotateState {
        Idle,
        BeforeTargetCW,
        AfterTargetCW,
        BeforeTargetCCW,
        AfterTargetCCW,
        Shutdown,
    };

    static QMap<Component, qreal> sComponentMass;

    void addBridge(int x, int y);
    void addCargo(int x, int y);
    void addRotateThruster(int x, int y);
    void addCruiseThruster(int x, int y, TwoDeg direction);

    void computeRotationalInertia();
    void computeStaticForceVectors();

    bool isGridLineFree(int x, int y, TwoDeg direction);

    void computeThrusterDirectionForce(int x, int y, TwoDeg direction);
    void computeCruiseEngineDirectionForce(int x, int y, TwoDeg direction);

    void update(qreal deltaT);

    qreal getEnergy() const { return 0.5*mM*qPow(mV.getSize(), 2.0); }
    qreal getVelocity() { return mV.getSize(); }
    Vector getVelocityVector() { return mV; }
    qreal getAtan2() { return mAtan2; }

    QGraphicsItem* getTacticalGraphicsItem() { return mTacticalGraphicsItem; }

    void resetMovement();
    void enableForward() { mForwardThrust = true; }
    void enableBackward() { mBackwardThrust = true; }
    void enableLateralLeft() { mLeftThrust = true; }
    void enableLateralRight() { mRightThrust = true; }
    void enableRotateLeft() { mRotateLeftThrust = true; }
    void enableRotateRight() { mRotateRightThrust = true; }

    void rotate(int degrees);

public Q_SLOTS:
    void receiveTextFromComponent(const QString& text);

Q_SIGNALS:
    void displayText(QString text);

private:
    bool mForwardThrust = false;
    bool mBackwardThrust = false;
    bool mLeftThrust = false;
    bool mRightThrust = false;
    bool mRotateLeftThrust = false;
    bool mRotateRightThrust = false;

    RotateState mRotateState = Idle;
    qreal mRotateTargetRad = 0;

    qreal mAtan2 = 0; // Bearing (rads)
    qreal mRotA = 0; // Rotational acceleration
    qreal mRotV = 0; // Rotational velocity
    Vector mV = Vector(0, 0); // Velocity vector
    Vector mA = Vector(0, 0); // Acceleration vector
    qreal mM = 0; // Mass
    qreal mI = 0; // Inertia
    Vector mCentreOfMass = Vector(0, 0);

    PlayerShipItem* mTacticalGraphicsItem;
    QVector<Engine*> mEngines;

    QMap<QPair<int, int>, Component*> mComponentMap;

    static int const sBlockSize = 10;
    static int const sGridSize = 5;
    static int const sGridSceneSize = 4;
};