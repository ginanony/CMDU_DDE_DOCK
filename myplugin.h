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
    const QString pluginName() const override;
    void init(PluginProxyInterface *proxyInter) override;
    QWidget *itemWidget(const QString &itemKey) override;
    QWidget *itemTipsWidget(const QString &itemKey) override;
    const QString itemCommand(const QString &itemKey) override;
    const QString itemContextMenu(const QString &itemKey) override;
    void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked) override;

private slots:
    void updateString();
    void requestContextMenu(const QString &itemKey);

private:
    long int i=0,db,ub,tt0=0,idle0=0;
    int nl;
    PluginWidget *m_mainWidget;
    QLabel *m_tipsLabel, *labelStartupDuration;
    QTimer *m_refershTimer;
    void setNetLine();
    QString KB(long k);
    QString BS(long b);
};

#endif // MYPLUGIN_H
