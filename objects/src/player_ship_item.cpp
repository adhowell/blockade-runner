#include "include/player_ship_item.h"

void PlayerShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QPen pen;
    QBrush fillBrush;
    QBrush emptyBrush;

    pen.setColor(QColor(0, 255, 0));
    fillBrush.setStyle(Qt::BrushStyle::SolidPattern);
    fillBrush.setColor(QColor(0, 255, 0));

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->rotate(*mAtan2*360.0/(M_PI*2.0));

    for (auto const &p : mPolys)
    {
        painter->drawPolygon(p);
    }
    for (auto const &e : mEngines)
    {
        painter->setBrush(emptyBrush);
        fillBrush.setColor(QColor(0, 255, 0, e->getOpacity()*255));
        if (e->enabled()) painter->setBrush(fillBrush);
        painter->drawPolygon(e->getPoly());
    }
    //painter->rotate(-mAtan2*360.0/(M_PI*2.0));
    /*
    QPolygonF poly = QPolygonF() << QPointF(qCos(mAtan2)*50.0, qSin(mAtan2)*50.0)
                                 << QPointF(qCos(mAtan2 + M_PI - 0.5)*50.0, qSin(mAtan2 + M_PI - 0.5)*50.0)
                                 << QPointF(qCos(mAtan2 + M_PI)*25.0, qSin(mAtan2 + M_PI)*25.0)
                                 << QPointF(qCos(mAtan2 + M_PI + 0.5)*50.0, qSin(mAtan2 + M_PI + 0.5)*50.0);
    painter->drawPolygon(poly);
    */
    painter->rotate(-*mAtan2*360.0/(M_PI*2.0));
    //painter->drawLine(0, 0, mV.x()*10, -mV.y()*10);
}

QRectF PlayerShipItem::boundingRect() const
{
    // Making the bounding rect too big helps with rendering fast moving PlayerShips
    return QRectF(-50, -50, 100, 100);
}