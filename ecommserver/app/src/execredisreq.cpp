//--------------------------------------------------------------------------------------------------
// execRegisReq.cpp
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 04/12/2019 - 09:14
//--------------------------------------------------------------------------------------------------
// Definicao Redis RequestHandler
//--------------------------------------------------------------------------------------------------

#include "execredisreq.h"
#include <QVariant>
#include <QDateTime>

execRegisReq::execRegisReq()
{
	if (db == nullptr) {
		db = new CDataBase("C:/ws/etc/dbconfig.json");
		errodb = !db->initDatabase(this);
	}
}

void execRegisReq::dataBaseError(const QString &text)
{
	qDebug() << "DB Error :" << text;
}

void execRegisReq::list(const QString & str, HttpRequest& , HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection );
	QByteArray ba = tblist->executeHtml();
	db->freeConnection(connection_name);
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
    response.write(ba);
}


void execRegisReq::indexPage( HttpRequest&, HttpResponse& response)
{
	response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
	QFile file; 
	file.setFileName("c:\\ws\\home\\index.html");
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray ba = file.readAll();		
		response.write(ba);
	}	
}

void execRegisReq::insert(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection, T_INSERT_POS);
	QByteArray ba = tblist->insereDevice();
	db->freeConnection(connection_name);
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "text/html");
	response.write(ba);
}

void execRegisReq::insertcmd(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection, T_INSERT_CMD);
	QByteArray ba = tblist->insereComando();
	db->freeConnection(connection_name);
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
	response.write(ba);
}


void execRegisReq::listjson(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection);
	QByteArray ba = tblist->execute();
	db->freeConnection(connection_name);
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "application/json; charset=ISO-8859-1");
	response.write(ba);
}

void execRegisReq::insertjson(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection);
	QByteArray ba = tblist->execute();
	db->freeConnection(connection_name);
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "application/json; charset=ISO-8859-1");
	response.write(ba);
}

