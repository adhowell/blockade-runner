#ifndef MINI_ENGINE_H
#define MINI_ENGINE_H
#include "engine.h"

#pragma once

class MiniEngine : public Engine {
public:
    MiniEngine(TwoDeg direction, Vector centreOfMassOffset, qreal mass, qreal inertia);
};

MiniEngine::MiniEngine(TwoDeg direction, Vector centreOfMassOffset, qreal mass, qreal inertia)
        : Engine(direction, centreOfMassOffset, mass, inertia, 1, 1.0, Profile::LIN, Size::SMALL) {}

#endif //MINI_ENGINE_H
