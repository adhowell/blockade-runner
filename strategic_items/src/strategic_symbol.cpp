#include "include/strategic_symbol.h"


void StrategicSymbol::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QPen pen;
    pen.setColor(mColour);
    pen.setWidthF(2.0);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);

    if (mIsCurrent) {
        painter->drawRect(QRectF(-mSize, -mSize, mSize*2.0, mSize*2.0));
    }
    if (mDrawBox) {
        painter->drawRect(QRectF((-mSize)-4, (-mSize)-4, (mSize+4)*2.0, (mSize+4)*2.0));
    }
    painter->drawPolygon(mPoly);

    painter->drawLine(mVelLine);
}

QRectF StrategicSymbol::boundingRect() const
{
    // Making the bounding rect too big helps with rendering fast moving Asteroids
    return {-mSize*2, -mSize*2, mSize*4.0, mSize*4.0};
}

void StrategicSymbol::updateTrack(qreal x, qreal y, Vector velocity, Faction perceivedFaction, bool isCurrent)
{
    // Animation
    if (mLifetime == 0) {
        mAnimationLifetime = mAnimationMaxLifetime;
    }
    mIsCurrent = isCurrent;

    mVelLine = QLineF(0, 0, velocity.x(), velocity.y());

    mLifetime = mMaxLifetime;
    setPos(x, y);
    switch (perceivedFaction) {
        case Faction::Red: mColour = QColor(255, 0, 0);
        case Faction::Blue: mColour = QColor(0, 0, 255);
        case Faction::Green: mColour = QColor(0, 255, 0);
        case Faction::Unknown: mColour = QColor(255, 155, 0);
    }
}

void StrategicSymbol::updateOffset(QPointF offset)
{
    setPos(pos() + offset);
    if (mLifetime > 0) mLifetime--;
    if (mAnimationLifetime > 0) mAnimationLifetime--;
    if (mAnimationLifetime > 0 && mAnimationLifetime % 25 == 0) mDrawBox = !mDrawBox;

    mColour.setAlpha(qMax((205*mLifetime/mMaxLifetime) + 50, 0));
}
