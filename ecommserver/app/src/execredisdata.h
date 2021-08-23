//--------------------------------------------------------------------------------------------------
// execRedisData.h
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 07/02/2020 - 13:40
//--------------------------------------------------------------------------------------------------
// Definicao execRedisData
//--------------------------------------------------------------------------------------------------
#pragma once

#include <qobject.h>
#include <qstring.h>
#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"  
#include "cqueue_redis.h"

using namespace webserverspace;

class execRedisData : public CConcurrent
{
	Q_OBJECT

public:
	execRedisData();
	~execRedisData();
	void run();

public:
	void keys    ( const QString &, HttpRequest&, HttpResponse& );
	void get     ( const QString &, HttpRequest&, HttpResponse& );
	void range   ( const QString &, HttpRequest&, HttpResponse& );
	void viagens ( const QString &, HttpRequest&, HttpResponse& );
	void geoDist(const QString & str, HttpRequest &, HttpResponse & response);
	void eval(const QString & str, HttpRequest &, HttpResponse & response);
	cqueue_redis *redis;
	bool error_connection = false;
};

