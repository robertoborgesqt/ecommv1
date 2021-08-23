//--------------------------------------------------------------------------------------------------
// execSqlData.cpp
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 23/05/2019 - 10:16
//--------------------------------------------------------------------------------------------------
// Definicao execSqlData
//--------------------------------------------------------------------------------------------------
#pragma once

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include "cdatabase.h"
#include "cjson.h" 
#include "sql_listdata.h"

using namespace webserverspace;

class execSqlData: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(execSqlData)

public:
	execSqlData();
	~execSqlData();
	void insertjson ( const QString &, HttpRequest &, HttpResponse & );
	void list       ( const QString &, HttpRequest &, HttpResponse & );
	void form       ( const QString &, HttpRequest &, HttpResponse & );
	void insertdata ( const QString &, HttpRequest &, HttpResponse & );
	void updatedata ( const QString &, HttpRequest &, HttpResponse & );
	void deletedata ( const QString &, HttpRequest &, HttpResponse & );
	void indexPage  ( HttpRequest &, HttpResponse & ); 	
	sql_listdata *tblist;

protected:
	CDataBase *db = nullptr;
	QString connection_name = nullptr;
	QSqlDatabase db_connection;
	bool errodb;

public slots:
	void dataBaseError(const QString &);

}; 