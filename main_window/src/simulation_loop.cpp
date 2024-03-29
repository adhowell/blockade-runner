#include "include/simulation_loop.h"
#include "include/radar_sensor.h"
#include "include/missile.h"

#include <QtWidgets>
#include <QFrame>
#include <QDebug>


SimulationLoop::SimulationLoop(TacticalScene* tacticalScene, StrategicScene* strategicScene, ConfigScene* configScene) : QObject()
{
    mTacticalScene = tacticalScene;
    mStrategicScene = strategicScene;
    mConfigScene = configScene;
    initPlayer();

    //DEBUG
    initMissile(200000, 0);
    //initMissile(-40000, -40000);
    //initMissile(40000, -40000);
    //initMissile(40000, 40000);
    //initMissile(-40000, 40000);
}

void SimulationLoop::start()
{
    startTimer(1000/gTargetFramerate);
}

void SimulationLoop::initPlayer()
{
    mPlayer = new PlayerShip(Faction::Blue, mNextUid++);
    mObjects << mPlayer;
    mTrackProcessors << new SignalTrackProcessor(mPlayer, &mObjects);

    connect(mPlayer, &PlayerShip::displayText, this, &SimulationLoop::receiveInfoFromPlayerShip);
    connect(mPlayer, &PlayerShip::handleAddConfigComponent, mConfigScene, &ConfigScene::drawConfigComponent);
    connect(mPlayer, &PlayerShip::handleAddConfigEngine, mConfigScene, &ConfigScene::drawConfigEngine);
    connect(mPlayer, &PlayerShip::handleAddCentreOfMass, mConfigScene, &ConfigScene::drawCentreOfMass);
    connect(mPlayer, &PlayerShip::handleAddCentreOfRotation, mConfigScene, &ConfigScene::drawCentreOfRotation);
    connect(mPlayer, &PlayerShip::handleRemoveAllConfigItems, mConfigScene, &ConfigScene::deleteAllComponents);
    connect(mPlayer, &PlayerShip::handleUpdateConfigStats, mConfigScene, &ConfigScene::updateStats);
    connect(mPlayer, &WorldObject::handleAddSensors, this, &SimulationLoop::addSensors);
    connect(mPlayer, &PlayerShip::handleClearSensors, this, &SimulationLoop::clearSensors);
    connect(mConfigScene->getView(), &ConfigView::addShipPart, mPlayer, &PlayerShip::handleAddPart);
    connect(mConfigScene->getView(), &ConfigView::removeShipPart, mPlayer, &PlayerShip::handleRemovePart);

    mPlayer->handleAddPart(Component::ComponentType::Reactor, {2, 2}, TwoDeg::Up);

    mTacticalScene->addItem(mPlayer->getTacticalGraphicsItem());
}

void SimulationLoop::initMissile(qreal x, qreal y)
{
    auto missile = new Missile(Faction::Red, {x, y}, {0, 0}, -M_PI*0.5, mNextUid++);
    auto processor = new GuidanceProcessor(missile, &mObjects);
    mGuidanceProcessors << processor;
    mTrackProcessors << processor;
    mObjects << missile;
    mTacticalScene->addItem(missile->getTacticalGraphicsItem());
}

void SimulationLoop::timerEvent(QTimerEvent *event)
{
    // The player ship is always at the origin, the world moves instead
    applyPlayerInput();
    Vector playerVelocity = mPlayer->getVelVector();
    playerVelocity.flip();
    QPointF playerOffset = playerVelocity.getPosDelta(WorldObject::deltaT);

    for (const auto& object : mObjects) {
        object->updatePosition(playerOffset);
        object->updateSensors();
    }

    // Update sensors
    for (const auto& processor : mTrackProcessors) {
        processor->computeTracks();

        if (processor->getParent() == mPlayer) {
            auto tracks = processor->getTracks();
            mStrategicScene->visualiseTracks(tracks);
        }
    }
    for (const auto& processor : mGuidanceProcessors) {
        processor->guideToMostValidTarget();
    }

    mTacticalScene->updateItems(playerOffset);
    mStrategicScene->applyPlayerUpdate(playerOffset, mPlayer->getAtan2(), mPlayer->getVelVector(),
                                       mPlayer->getAccVector());

    gTimeStamp++;
}

void SimulationLoop::applyPlayerInput()
{
    mPlayer->resetMovement();
    //if (mLeftThrust) mPlayer->enableRotateLeft();
    //if (mRightThrust) mPlayer->enableRotateRight();
    if (mForwardThrust) mPlayer->enableForward();
    if (mBackwardThrust) mPlayer->enableBackward();

    mPlayer->update();
}

void SimulationLoop::setThrust(TwoDeg direction, bool isActive)
{
    switch (direction)
    {
        case TwoDeg::Up:
            mForwardThrust = isActive;
            break;
        case TwoDeg::Down:
            mBackwardThrust = isActive;
            break;
        case TwoDeg::Left:
            mLeftThrust = isActive;
            break;
        case TwoDeg::Right:
            mRightThrust = isActive;
            break;
    }
}

void SimulationLoop::addSensors(QVector<std::shared_ptr<Sensor>> sensors)
{
    for (const auto& sensor : sensors) {
        mStrategicScene->addItem(sensor->getItem());
    }
}

void SimulationLoop::clearSensors(QVector<std::shared_ptr<Sensor>> sensors)
{
    for (const auto& sensor : sensors) {
        mStrategicScene->removeItem(sensor->getItem());
    }
}

void SimulationLoop::rotate(int degrees)
{
    mPlayer->rotate(qreal(degrees));
}

void SimulationLoop::receiveInfoFromPlayerShip(const QString& text)
{
    Q_EMIT relayInfo(text);
}

void SimulationLoop::receiveWarningFromPlayerShip(const QString& text)
{
    Q_EMIT relayWarning(text);
}

void SimulationLoop::receiveErrorFromPlayerShip(const QString& text)
{
    Q_EMIT relayError(text);
}