#pragma once

#include <qobject.h>
#include <qfile.h>
#include "execsqldata.h"  

extern int ipport;
extern bool iplocal;

class sendcss : public QObject
{
	Q_OBJECT

public:
	sendcss()
	{

	}

	QByteArray setPortHttp(const QByteArray &ba)
	{
		QString str = ba.data();
		str.replace(":6524", QString(":%1").arg(ipport));

		if ( !iplocal )
		str.replace("10.20.3.36","189.16.50.195");
		  else
			str.replace("189.16.50.195", "10.20.3.36");
		 
		return str.toUtf8(); 
	}

	void get(const QString & str, HttpRequest&, HttpResponse& response)
	{  
		QByteArray ba = load(str);

		if (!ba.count())
			return;
		
		if ( str.contains( ".ico") )
			response.setHeader("Content-Type", "image/ico");
		else
		if (str.contains(".png"))
			response.setHeader("Content-Type", "image/png");
		else
		response.setHeader("Content-Type", "text/css");
		response.write(ba);
	}

	void getHtml(const QString & str, HttpRequest&, HttpResponse& response)
	{
		QByteArray ba = load(str);
		if (!ba.count())
			return;
		response.setHeader("Content-Type", "text/html");
		response.write(ba);
	}

	QByteArray load(const QString & name)
	{
		QString fname = QDir().currentPath() + "/etc"; 
		QFile f(fname+name);
		QByteArray ret;

		if (f.open(QIODevice::ReadOnly))
		{
			int sz = f.size();
			ret = f.read(sz);
			f.close();
		}
		if (name.contains(".html"))
			return setPortHttp(ret);
		else
			return ret;
	}

	QString id;
	QByteArray data;
};

