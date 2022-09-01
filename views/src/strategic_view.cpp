#include "include/strategic_view.h"
#include "../global_config.h"
#include "include/globals.h"

#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

StrategicView::StrategicView(QGraphicsScene* scene) : QGraphicsView()
{
    setStyleSheet("border: 1px solid green");
    setScene(scene);
    ensureVisible(QRectF(0, 0, 0, 0));
}

void StrategicView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
    {
        scale(1.1, 1.1);
    } else
    {
        scale(0.91, 0.91);
    }
}

StrategicScene::StrategicScene(QWidget* parent) : QGraphicsScene(parent)
{
    initBackground();
    setSceneRect(QRectF(-100, -100, 200, 200));
    mView = new StrategicView(this);
    mPlayerSymbol = new PlayerSymbolItem({0, 0});
    addItem(mPlayerSymbol);
    for (int i = 1; i <= 6; i++)
    {
        auto velItem = new VelocityMarker(i * 10 * gTargetFramerate);
        auto accItem = new AccelerationMarker(i * 10 * gTargetFramerate);
        mVelMarkers << velItem;
        mAccMarkers << accItem;
        addItem(velItem);
        addItem(accItem);
    }
}

void StrategicScene::updatePlayer(QPointF posOffset, qreal angle, Vector vel, Vector acc)
{
    posOffset *= gScaleFactor;
    mPlayerSymbol->applyUpdate(angle, vel);

    vel *= gScaleFactor;
    acc *= gScaleFactor;
    std::for_each(mVelMarkers.cbegin(), mVelMarkers.cend(),
                  [vel](auto v){ v->updateOffset(vel); });
    std::for_each(mAccMarkers.cbegin(), mAccMarkers.cend(),
                  [vel, acc](auto v){ v->updateOffset(vel, acc); });
    mGridLines->updateOffset(posOffset);
}

/*
void StrategicScene::updateItems(QPointF offset)
{
    QList<QGraphicsItem*> forDeletion;
    for (QGraphicsItem *item: items()) {
        if (Asteroid *a = dynamic_cast<Asteroid*>(item)) {
            a->posUpdate(offset);
            a->update();
        }
        if (PhosphorGhost *g = dynamic_cast<PhosphorGhost*>(item)) {
            if (g->isDone()) forDeletion << g;
            g->applyPosOffset(offset);
            g->update();
        }
        if (Starfield *s = dynamic_cast<Starfield*>(item)) {
            s->updateOffset(offset);
            s->update();
        }
    }

    for (auto i : forDeletion) removeItem(i);

    //auto ghost = new PhosphorGhost(mPlayer->getPoly(), 20);
    //scene()->addItem(ghost);
}
*/
void StrategicScene::initBackground()
{
    setBackgroundBrush(QColor(0, 0, 15));
    mGridLines = new GridLines(200);
    addItem(mGridLines);
}

StrategicView* StrategicScene::getView() const
{
    return mView;
}