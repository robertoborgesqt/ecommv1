

#include "static.h"
#include "startup.h"
#include "filelogger.h"
#include "requesthandler.h"
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <iostream>

using namespace std;

#define APPNAME "datasrv"
#define ORGANISATION "Teste"
#define DESCRIPTION "datasrv backend"

using namespace webserverspace;

FileLogger* logger;

QString ipclass;
int ipport = 0;

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

HttpListener* listener;

int main(int argc, char *argv[])
{
    QCoreApplication app( argc, argv );
    app.setApplicationName(APPNAME);
    app.setOrganizationName(ORGANISATION);
	bool ok;

	if (argc < 2) {
		ipport = 9000;
	}
    else{
	ipport = QString("%1").arg(argv[1]).toInt(&ok);
    }

	if ( !ok ) {
		cout << "Porta invalida " << argv[1];
		exit(-1);
	}

    // Find the configuration file
    QString configFileName=searchConfigFile();

    // Configure logging into a file
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat,qApp);
    logSettings->beginGroup("logging");
    logger=new FileLogger(logSettings,10000,qApp);

    // Configure and start the TCP listener
    qDebug("ServiceHelper: Starting service");
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,qApp);
    listenerSettings->beginGroup("listener");
    listener=new HttpListener(ipport, listenerSettings,new RequestHandler(qApp),qApp);
    QString host = listenerSettings->value("host").toString();
    ipclass = listenerSettings->value("ipclass").toString();
    qDebug( QString("Service has started %1:%2").arg(host).arg(ipport).toLatin1().data() );
    logger->installMsgHandler();
    return app.exec();
}




