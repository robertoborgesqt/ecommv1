#include "static.h"
#include "startup.h"
#include "filelogger.h"
#include "requesthandler.h"
#include <QDir>
#include <QFile>
#define APPNAME "datasrv"
#define ORGANISATION "Teste"
#define DESCRIPTION "datasrv backend"

FileLogger* logger;

QString ipclass;
int ipport = 0;

HttpListener* listener;

QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName(appName+".ini");

    QStringList searchList;
	searchList.append("../etc");
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../../etc"); // for development without shadow build
    searchList.append(binDir+"/../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)   {
        QFile file(dir+"/"+fileName);
        if (file.exists()){
            // found
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    foreach (QString dir, searchList){
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }

    qFatal("Cannot find config file %s",qPrintable(fileName));
    return "";
}

void Startup::start()
{
    QCoreApplication* app = application();
    app->setApplicationName(APPNAME);
    app->setOrganizationName(ORGANISATION);

    // Find the configuration file
    QString configFileName=searchConfigFile();

    // Configure logging into a file
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat,app);
    logSettings->beginGroup("logging");
    logger=new FileLogger(logSettings,10000,app);

    // Configure and start the TCP listener
    qDebug("ServiceHelper: Starting service");
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,app);
    listenerSettings->beginGroup("listener");
    listener=new HttpListener(listenerSettings,new RequestHandler(app),app);
	QString host = listenerSettings->value("host").toString();
	ipclass = listenerSettings->value("ipclass").toString();
	ipport = listenerSettings->value("port").toInt();
    qDebug( QString("Startup: Service has started %1:%2").arg(host).arg(ipport).toLatin1().data() );
    logger->installMsgHandler();
}

void Startup::stop(){
    delete listener;
    qWarning("Startup: Service has been stopped");
}


Startup::Startup(int argc, char *argv[])
    : QtService<QCoreApplication>(argc, argv, APPNAME)
{
    setServiceDescription(DESCRIPTION);
    setStartupType(QtServiceController::AutoStartup);
}



