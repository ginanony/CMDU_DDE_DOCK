#include "myplugin.h"
#include <QLabel>
#define MYPLUGIN_KEY    "myplugin"
long int i=0,db,ub,tt0=0,idle0=0;

MyPlugin::MyPlugin(QObject *parent) :
    QObject(parent),
    m_mainWidget(new QLabel),
    m_tipsLabel(new QLabel),
    m_refershTimer(new QTimer(this))
{
    QLabel *label = (QLabel *)m_mainWidget;
    label->setText("↑000.00MB/s\n↓000.00MB/s");
    label->setStyleSheet("color:white;padding:0px;");
    //label->setAlignment(Qt::AlignRight);
    //label->setFixedWidth(100);
    m_tipsLabel->setStyleSheet("color:white;padding:5px;");
    m_refershTimer->setInterval(1000);
    m_refershTimer->start();
    connect(m_refershTimer, &QTimer::timeout, this, &MyPlugin::updateString);
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

QString g(int k){
    QString g;
    if(k>999999){
        g=QString::number(k/1048576.0,'f',2)+"GB";
    }else{
        if(k>999){
            g=QString::number(k/1024.0,'f',2)+"MB";
        }else{
            g=QString::number(k/1.0,'f',2)+"KB";
        }
    }
    return g;
}

QString m(int k){
    QString g;
    if(k>999999){
        //g=QString::number(k/1048576.0,'f',2)+"MB";
        //双引号里面是字符串，单引号里面是单个字符。
        g.sprintf("%06.2fMB",k/1048576.0);
    }else{
        if(k>999){
            //g=QString::number(k/1024.0,'f',2)+"KB";
            g.sprintf("%06.2fKB",k/1024.0);
        }else{
            //g=QString::number(k/1.0,'f',2)+"B";
            g.sprintf("%06.2fB",k/1.0);
        }
    }
    return g;
}

void MyPlugin::updateString()
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    QLabel *label = (QLabel *)m_mainWidget;
    label->setText(currentDateTime.toString("HH:mm:ss"));

    QFile file2("/proc/uptime");
    file2.open(QIODevice::ReadOnly);
    QString l=file2.readLine();
    file2.close();
    QTime t(0,0,0);
    t=t.addSecs(l.left(l.indexOf(".")).toInt());
    QString uptime="开机: "+t.toString("hh:mm:ss");
    
    QFile file("/proc/meminfo");
    file.open(QIODevice::ReadOnly);
    l=file.readLine();
    long int mt =l.mid(l.indexOf(":")+1,l.length()-13).replace(" ","").toInt();
    l=file.readLine();
    file.close();
    long int mf =l.mid(l.indexOf(":")+1,l.length()-11).replace(" ","").toInt();
    long int mu=mt-mf;
    QString musage=QString::number(mu*100/mt)+"%";
    QString mem="\n内存: "+QString("%1/%2=%3").arg(g(mu)).arg(g(mt)).arg(musage);

    QFile file3("/proc/stat");
    file3.open(QIODevice::ReadOnly);
    l=file3.readLine();
    QByteArray ba;
    //ba = l.toAscii();
    ba=l.toLatin1();
    const char *ch;
    ch=ba.constData();
    char cpu[5];
    long int user,nice,sys,idle,iowait,irq,softirq,tt;
    sscanf(ch,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
    tt=user+nice+sys+idle+iowait+irq+softirq;
    file3.close();
    QString cusage="";
    if(i>0)cusage="\nCPU: "+QString::number(((tt-tt0)-(idle-idle0))*100/(tt-tt0))+"%";
    idle0=idle;
    tt0=tt;

    QFile file4("/proc/net/dev");
    file4.open(QIODevice::ReadOnly);
    l=file4.readLine();
    l=file4.readLine();
    l=file4.readLine();
    //l=file4.readLine();
    file4.close();
    QStringList list=l.split(QRegExp("\\s{1,}")); // 第一个\表示转义字符，\s表示空格，｛1，｝表示一个以上
    QString dss="";
    QString uss="";
    if(i>0){
        long ds=list.at(1).toLong()-db;
        long us=list.at(9).toLong()-ub;
        dss=m(ds)+"/s";
        uss=m(us)+"/s";
    }
    db=list.at(1).toLong();
    ub=list.at(9).toLong();
    QString net="\n下载: "+m(list.at(1).toLong())+"  "+dss+"\n上传: "+m(list.at(9).toLong())+"  "+uss;
    m_tipsLabel->setText(uptime+cusage+mem+net);
    label->setText("↑"+uss+"\n↓"+dss);

    i++;
}
