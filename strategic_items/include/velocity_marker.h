#include "include/vector.h"

#include <QGraphicsItem>
#include <QtWidgets>

#pragma once


class VelocityMarker : public QGraphicsItem {
public:
    explicit VelocityMarker(int seconds) : mOrigin(0, 0), mSeconds(seconds), mSize(seconds/600) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updateOffset(Vector vel);

private:
    Vector mOrigin;
    qreal mSeconds;
    int mSize;
};