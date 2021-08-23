//--------------------------------------------------------------------------------------------------
// execSqlData.cpp
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 23/05/2019 - 10:16
//--------------------------------------------------------------------------------------------------
// Definicao RequestHandler
//--------------------------------------------------------------------------------------------------

#include "execsqldata.h"
#include <QVariant>
#include <QDateTime>

execSqlData::execSqlData()
{
	if (db == nullptr) {

		QString fname = QDir().currentPath() + "/etc";
		qDebug() << "Localconfig " << fname;

		if (QFile().exists(fname + "/dbconfig.json") == false)
		{
			exit(-1);
		}
		db = new CDataBase(fname +"/dbconfig.json");
		errodb = !db->initDatabase(this);
	}
}

execSqlData::~execSqlData()
{/*
   qDebug() << "closing data base connections";
	db->closeConnections();
	db->CloseDB();*/
	delete db; 
}

void execSqlData::dataBaseError(const QString &text)
{
	qDebug() << "DB Error :" << text;
}

void execSqlData::insertdata(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection, T_INSERT_DATA, false);
	QByteArray ba = tblist->insertdata();
	db->freeConnection(connection_name);
	delete tblist;
	tblist = nullptr;
	if (!ba.count())
	{
		ba = "Erro";
	}
	response.setHeader("Content-Type", "text/plain");
	response.write(ba);
}

void execSqlData::updatedata(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection, T_UPDATE_DATA, false);
	QByteArray ba = tblist->updatedata();
	db->freeConnection(connection_name);
	delete tblist;
	tblist = nullptr;
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "text/plain");
	response.write(ba);
}

void execSqlData::deletedata(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection, T_DELETE_DATA, false);
	QByteArray ba = tblist->deletedata();
	db->freeConnection(connection_name);
	delete tblist;
	tblist = nullptr;
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "text/plain");
	response.write(ba);
}

void execSqlData::list(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection, 0 );
	QByteArray ba = tblist->execute();
	db->freeConnection(connection_name);
	delete tblist;
	tblist = nullptr;
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "application/json; charset=UTF8");
	response.write(ba);
}

void execSqlData::insertjson(const QString & str, HttpRequest&, HttpResponse& response)
{
	connection_name = db->getConnection(&db_connection);
	tblist = new sql_listdata(str, db_connection, -1 );
	QByteArray ba = tblist->execute();
	db->freeConnection(connection_name);
	delete tblist;
	tblist = nullptr;
	if (!ba.count())
		return;
	response.setHeader("Content-Type", "application/json; charset=UTF8");
	response.write(ba);
}

