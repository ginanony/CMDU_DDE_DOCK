#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QLabel>

class PluginWidget : public QLabel
{
    Q_OBJECT

public:
    PluginWidget(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *e);

signals:
    void requestContextMenu(const QString &itemKey) const;
};

#endif // PLUGINWIDGET_H
