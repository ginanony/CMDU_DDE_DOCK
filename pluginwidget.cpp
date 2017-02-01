#include "pluginwidget.h"

#include <QMouseEvent>

#define MYPLUGIN_KEY    "myplugin"

PluginWidget::PluginWidget(QWidget *parent)
    : QLabel(parent)
{

}

void PluginWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::RightButton)
        return QWidget::mousePressEvent(e);

    const QPoint p(e->pos() - rect().center());
    if (p.manhattanLength() < std::min(width(), height()) * 0.8 * 0.5)
    {
        emit requestContextMenu(MYPLUGIN_KEY);
        return;
    }

    return QWidget::mousePressEvent(e);
}
