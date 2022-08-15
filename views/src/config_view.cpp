#include "include/config_view.h"

#include "../objects/include/menu_grid_box.h"

ConfigView::ConfigView(QGraphicsScene* scene) : QGraphicsView()
{
    setScene(scene);
    setBackgroundBrush(QBrush(QColor(0, 0, 0)));
    ensureVisible(QRectF(0, 0, 0, 0));
}

void ConfigView::wheelEvent(QWheelEvent* event)
{
    Q_EMIT handleClose();
}

void ConfigView::mousePressEvent(QMouseEvent* event)
{
    auto box = dynamic_cast<GridBox*>(itemAt(event->pos()));
    if (box)
    {
        Q_EMIT addShipPart(mFocusComponent, box->getCoords());
        return;
    }

    auto focusTile = dynamic_cast<ComponentTile*>(itemAt(event->pos()));
    if (focusTile)
    {
        for (auto item: items())
        {
            auto tile = dynamic_cast<ComponentTile *>(item);
            if (!tile)
                continue;
            if (tile == focusTile)
                tile->setFocus(true);
            else
                tile->setFocus(false);
        }
        mFocusComponent = focusTile->getType();
    }
}

ConfigScene::ConfigScene(QWidget* parent) : QGraphicsScene(parent)
{
    setSceneRect(QRectF(-50, -50, 100, 100));
    mView = new ConfigView(this);
    connect(mView, &ConfigView::handleClose, this, &ConfigScene::handleClose);
    mView->scale(4, 4);
    mView->setRenderHint(QPainter::Antialiasing);

    // Draw grid-lines
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            addItem(new GridBox(x, y));

    // Draw component tiles
    addItem(new ComponentTile(-30, -80, Component::ComponentType::Reactor));
    addItem(new ComponentTile(-30, -55, Component::ComponentType::HeatSink));
    addItem(new ComponentTile(-100, -55, Component::ComponentType::RotateThruster));
}

ConfigView* ConfigScene::getView() const
{
    return mView;
}

void ConfigScene::handleClose()
{
    Q_EMIT close();
}

void ConfigScene::drawConfigComponent(Component* component)
{
    mComponents << addPolygon(component->getPoly(), QPen(QColor(0, 255, 0)));
}

void ConfigScene::drawConfigEngine(Engine* engine)
{
    mComponents << addPolygon(engine->getPoly(), QPen(QColor(0, 255, 0)));
}

void ConfigScene::deleteAllComponents()
{
    for (auto item : mComponents)
    {
        removeItem(item);
        delete item;
    }
    mComponents.clear();
}

