#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QObject>
#include <QLabel>

#include "interfaces/constants.h"
#include "interfaces/pluginsiteminterface.h"
#include "interfaces/pluginproxyinterface.h"

#include "pluginwidget.h"

class MyPlugin : public QObject, public PluginsItemInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "MyPlugin.json")

public:
    MyPlugin(QObject *parent = 0);

    //Dock接口函数
    const QString pluginName() const;
    void init(PluginProxyInterface *proxyInter);
    QWidget *itemWidget(const QString &itemKey);    
    QWidget *itemTipsWidget(const QString &itemKey);
    const QString itemCommand(const QString &itemKey) override;
    const QString itemContextMenu(const QString &itemKey) override;
    void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked);


private slots:
    void updateString();
    void requestContextMenu(const QString &itemKey);

private:
    PluginWidget *m_mainWidget;
    QLabel *m_tipsLabel;
    QTimer *m_refershTimer;

};

#endif // MYPLUGIN_H
