#include "myplugin.h"
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#define MYPLUGIN_KEY    "myplugin"
long int i=0,db,ub,tt0=0,idle0=0;
int nl=3;
MyPlugin::MyPlugin(QObject *parent) :
    QObject(parent),
    m_mainWidget(new PluginWidget),
    m_tipsLabel(new QLabel),
    m_refershTimer(new QTimer(this))
{
    QLabel *label = (QLabel *)m_mainWidget;
    label->setText("↑000.00 B/s\n↓000.00 B/s");
    label->setStyleSheet("color:white;padding:0px;");
    //label->setAlignment(Qt::AlignRight);
    label->setFixedWidth(75);
    m_tipsLabel->setStyleSheet("color:white;padding:5px;");
    m_refershTimer->setInterval(1000);
    m_refershTimer->start();
    connect(m_refershTimer, &QTimer::timeout, this, &MyPlugin::updateString);
    connect(m_mainWidget, &PluginWidget::requestContextMenu, this, &MyPlugin::requestContextMenu);

}

//插件名
const QString MyPlugin::pluginName() const
{
    return "myplugin";
}

//插件初始化
void MyPlugin::init(PluginProxyInterface *proxyInter)
{
    m_proxyInter = proxyInter;
    m_proxyInter->itemAdded(this, MYPLUGIN_KEY);
}


//插件的Widget
QWidget *MyPlugin::itemWidget(const QString &itemKey)
{
    if (itemKey == MYPLUGIN_KEY)
        return m_mainWidget;
    return nullptr;
}

QWidget *MyPlugin::itemTipsWidget(const QString &itemKey)
{
    if (itemKey == MYPLUGIN_KEY) {
        //m_tipsLabel->setText("提示");
        return m_tipsLabel;
    }
    return nullptr;
}

void MBAbout()
{
    QMessageBox aboutMB(QMessageBox::NoIcon, "系统信息 2.２", "关于\n\n深度Linux系统上一款在任务栏显示网速，鼠标悬浮显示开机时间、CPU占用、内存占用、下载字节、上传字节的插件。\n作者：黄颖\nE-mail: sonichy@163.com\n主页：sonichy.96.lt\n致谢：\nlinux028@deepin.org");
    aboutMB.setIconPixmap(QPixmap(":/icon.png"));
    aboutMB.exec();
}

void MBHistory()
{
    QMessageBox historyMB(QMessageBox::NoIcon, "系统信息 2.２", "更新历史\n\n2.２ (2017-07-08)\n1.设置网速所在行。\n\n2.1 (2017-02-01)\n1.上传下载增加GB单位换算，且参数int改long，修复字节单位换算溢出BUG。\n\n2.0 (2016-12-07)\n1.增加右键菜单。\n\n1.0 (2016-11-01)\n1.把做好的Qt程序移植到DDE-DOCK。");
    historyMB.exec();
}

void MyPlugin::setNetLine()
{
    int ntl=0;
    QFile file1("/proc/net/dev");
    file1.open(QIODevice::ReadOnly);
    QTextStream TS(&file1);
    while(!TS.atEnd()){
        QString line = TS.readLine();
        QMessageBox MB(QMessageBox::NoIcon, "系统信息 2.２", line);
        MB.exec();
        ntl++;
    }
    file1.close();
    bool ok;
    int ln= QInputDialog::getInt(NULL, "设置", "网速所在行", nl, 3, 9, 1, &ok);
    if(ok){
        nl=ln;
    }
}

//点击响应
const QString MyPlugin::itemCommand(const QString &itemKey)
{
    //Q_UNUSED(itemKey);
    MBAbout();
    return "";
}

//右键菜单
const QString MyPlugin::itemContextMenu(const QString &itemKey)
{
    Q_UNUSED(itemKey);
    QList<QVariant> items;
    items.reserve(3);

    QMap<QString, QVariant> about;
    about["itemId"] = "about";
    about["itemText"] = "关于";
    about["isActive"] = true;
    items.push_back(about);

    QMap<QString, QVariant> history;
    history["itemId"] = "history";
    history["itemText"] = "更新历史";
    history["isActive"] = true;
    items.push_back(history);

    QMap<QString, QVariant> netline;
    netline["itemId"] = "netline";
    netline["itemText"] = "设置";
    netline["isActive"] = true;
    items.push_back(netline);

    QMap<QString, QVariant> menu;
    menu["items"] = items;
    menu["checkableMenu"] = false;
    menu["singleCheck"] = false;
    return QJsonDocument::fromVariant(menu).toJson();
}

void MyPlugin::invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked)
{
    Q_UNUSED(itemKey)
    //Q_UNUSED(menuId)
    Q_UNUSED(checked)

    QStringList menuitems;
    menuitems << "about" << "history" << "netline";

    switch(menuitems.indexOf(menuId)){
    case 0:
        MBAbout();
        break;
    case 1:
        MBHistory();
        break;
    case 2:
        setNetLine();
        break;
    }
}

void MyPlugin::requestContextMenu(const QString &itemKey)
{
    m_proxyInter->requestContextMenu(this, itemKey);
}

QString KB(long k){
    QString s="";
    if(k>999999){
        s=QString::number(k/(1024*1024.0),'f',2)+"GB";
    }else{
        if(k>999){
            s=QString::number(k/1024.0,'f',2)+"MB";
        }else{
            s=QString::number(k/1.0,'f',2)+"KB";
        }
    }
    return s;
}

QString BS(long b){
    QString s="";
    if(b>999999999){
        s=QString::number(b/(1024*1024*1024.0),'f',2)+"GB";
    }else{
        if(b>999999){
            s=QString::number(b/(1024*1024.0),'f',2)+"MB";
        }else{
            if(b>999){
                s=QString::number(b/1024.0,'f',2)+"KB";
            }else{
                s=QString::number(b/1.0,'f',2)+"B";
            }
        }
    }
    return s;
}

//周期更新
void MyPlugin::updateString()
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    QLabel *label = (QLabel *)m_mainWidget;
    label->setText(currentDateTime.toString("HH:mm:ss"));

    QFile file("/proc/uptime");
    file.open(QIODevice::ReadOnly);
    QString l=file.readLine();
    file.close();
    QTime t(0,0,0);
    t=t.addSecs(l.left(l.indexOf(".")).toInt());
    QString uptime="开机: "+t.toString("hh:mm:ss");
    
    file.setFileName("/proc/meminfo");
    file.open(QIODevice::ReadOnly);
    l=file.readLine();
    long mt=l.mid(l.indexOf(":")+1,l.length()-13).replace(" ","").toInt();
    l=file.readLine();
    file.close();
    long mf=l.mid(l.indexOf(":")+1,l.length()-11).replace(" ","").toInt();
    long mu=mt-mf;
    QString musage=QString::number(mu*100/mt)+"%";
    QString mem="\n内存: "+QString("%1/%2=%3").arg(KB(mu)).arg(KB(mt)).arg(musage);

    file.setFileName("/proc/stat");
    file.open(QIODevice::ReadOnly);
    l=file.readLine();
    QByteArray ba;    
    ba=l.toLatin1();
    const char *ch;
    ch=ba.constData();
    char cpu[5];
    long int user,nice,sys,idle,iowait,irq,softirq,tt;
    sscanf(ch,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
    tt=user+nice+sys+idle+iowait+irq+softirq;
    file.close();
    QString cusage="";
    if(i>0)cusage="\nCPU: "+QString::number(((tt-tt0)-(idle-idle0))*100/(tt-tt0))+"%";
    idle0=idle;
    tt0=tt;

    file.setFileName("/proc/net/dev");
    file.open(QIODevice::ReadOnly);
    for(int j=1;j<=nl;j++){
        l=file.readLine();
    }
    //l=file.readLine();
    //l=file.readLine();
    //l=file.readLine();
    file.close();
    QStringList list=l.split(QRegExp("\\s{1,}")); // 第一个\表示转义字符，\s表示空格，｛1，｝表示一个以上
    QString dss="";
    QString uss="";
    if(i>0){
        long ds=list.at(1).toLong()-db;
        long us=list.at(9).toLong()-ub;        
        dss=BS(ds)+"/s";
        uss=BS(us)+"/s";
    }
    db=list.at(1).toLong();
    ub=list.at(9).toLong();
    QString net="\n下载: "+BS(db)+"  "+dss+"\n上传: "+BS(ub)+"  "+uss;
    m_tipsLabel->setText(uptime+cusage+mem+net);
    label->setText("↑"+uss+"\n↓"+dss);

    i++;
//    if(db==0)nl++;
//    if(nl>5)nl=3;
}
