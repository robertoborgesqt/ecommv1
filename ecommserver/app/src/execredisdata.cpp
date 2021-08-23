//--------------------------------------------------------------------------------------------------
// execRedisData.cpp
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 07/02/2020 - 13:40
//--------------------------------------------------------------------------------------------------
// Definicao execRedisData
//--------------------------------------------------------------------------------------------------

#include "execredisdata.h"

execRedisData::execRedisData()
{
	redis = new cqueue_redis(this, "10.20.3.161", 6379); 

	QTimer tm;
	tm.setInterval(3000);
	QEventLoop loop;
	connect(&tm, SIGNAL(timeout()), &loop, SLOT(quit()));
	connect(redis, SIGNAL(isconnected()), &loop, SLOT(quit()));
	connect(redis,  SIGNAL(notconnected()), &loop, SLOT(quit())); 
	redis->connectRedis(); 
	loop.exec(); 

	error_connection = !redis->redis_status; 

}
 
execRedisData::~execRedisData()
{
	delete redis;

}

void execRedisData::run()
{

}

void execRedisData::keys(const QString & str, HttpRequest&, HttpResponse& response)
{
	QByteArray ret;
	if (redis->redis_status)
	{ 
		ret = redis->getKey(str);
	}
	else
	{
		ret = "Redis error.. (offline)";
	}
	response.setHeader("Content-Type", "application/json; charset=UTF8");
	response.write(ret);
}

void execRedisData::get(const QString & str, HttpRequest&, HttpResponse& response)
{
	QByteArray ret;
	if (redis->redis_status)
	{
		ret = redis->getGet(str);
	}
	else
	{
		ret = "Redis error.. (offline)";
	}
	response.setHeader("Content-Type", "application/json; charset=UTF8");
	response.write(ret);
}
void execRedisData::range(const QString & str, HttpRequest&, HttpResponse& response)
{
	QByteArray ret;
	if (redis->redis_status)
	{
		ret = redis->getRange(str);
	}
	else
	{
		ret = "Redis error.. (offline)";
	}
	response.setHeader("Content-Type", "application/json; charset=UTF8");
	response.write(ret);
}

void execRedisData::viagens(const QString & str, HttpRequest&, HttpResponse& response)
{
	QByteArray ret;
	if (redis->redis_status)
	{
		ret = redis->getViagens(str);
	}
	else
	{
		ret = "Redis error.. (offline)";
	}
	response.setHeader("Content-Type", "application/json; charset=UTF8");
	response.write(ret);
}


void execRedisData::geoDist(const QString & str, HttpRequest&, HttpResponse& response)
{
	QByteArray ret;
	if (redis->redis_status)
	{
		ret = redis->getGeoDist(str);
	}
	else
	{
		ret = "Redis error.. (offline)";
	}
	response.setHeader("Content-Type", "application/json; charset=UTF8");
	response.write(ret);
}

void execRedisData::eval(const QString & str, HttpRequest&, HttpResponse& response)
{
	QByteArray ret;
	if (redis->redis_status)
	{
		ret = redis->eval(str);
	}
	else
	{
		ret = "Redis error.. (offline)";
	}
	response.setHeader("Content-Type", "application/json; charset=UTF8");
	response.write(ret);
}
