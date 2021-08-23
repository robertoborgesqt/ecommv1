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

class execRegisReq : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(execRegisReq)

public:
	execRegisReq();
	void listjson   ( const QString &, HttpRequest &, HttpResponse & );
	void insertjson ( const QString &, HttpRequest &, HttpResponse & );
	void list       ( const QString &, HttpRequest &, HttpResponse & );
	void insert     ( const QString &, HttpRequest &, HttpResponse & );
	void insertcmd  ( const QString &, HttpRequest &, HttpResponse & );
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