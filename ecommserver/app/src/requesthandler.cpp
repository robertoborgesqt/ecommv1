//--------------------------------------------------------------------------------------------------
// httprequesthandler.cpp
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 23/05/2019 - 10:16
//--------------------------------------------------------------------------------------------------
// Definicao RequestHandler
//--------------------------------------------------------------------------------------------------

#include "requesthandler.h"
#include "filelogger.h"
#include "execsqldata.h" 
#include "sendcss.h"

extern FileLogger* logger;
extern QString ipclass; 
bool iplocal;

RequestHandler::RequestHandler(QObject* parent) :HttpRequestHandler(parent){
	qDebug() << "RequestHandler instance";
}

QString RequestHandler::getVar(const QString &path){
	return path.split("&").at(1);
}

void RequestHandler::service(HttpRequest& request, HttpResponse& response)
{
	static QMutex recursiveMutex(QMutex::Recursive);
	static QMutex nonRecursiveMutex(QMutex::NonRecursive);

	recursiveMutex.lock();
	QString host = response.getHost();
	QByteArray path = request.getPath();
	qDebug() << "Controller: path=" << path.data() << " host " << host;
	iplocal = host.startsWith(ipclass);

	if (path.contains(".html")){
		sendcss().getHtml(path.data(), request, response);
	}
	else
	if (path.contains("assets")){
		sendcss().get(path.data(), request, response);
	}
	else
	if (path.contains("bootstrap/")){
		sendcss().get(path.data(), request, response);
	}
	else
	if (path.contains("/css/")){
		sendcss().get(path.data(), request, response);
	}
	else
	if (path.contains(".ico") || path.contains(".png")){
		sendcss().get(path.data(), request, response);
	}
	else
	if (path.contains(".js") || path.contains(".png")){
		sendcss().get(path.data(), request, response);
	}
	else 
	if (path.startsWith("/list"))	{
		QString query;
		if (path.contains("&"))
		query = getVar(path); 
		execSqlData().list(query, request, response);
	}
	else
	if (path.startsWith("/insertdata"))	{
		QString query;
		if (path.contains("&"))
			query = getVar(path); 
		execSqlData().insertdata(query, request, response);
	}
	else
	if (path.startsWith("/updatedata"))	{
		QString query;
		if (path.contains("&"))
			query = getVar(path); 
		execSqlData().updatedata(query, request, response);
	}
	else
	if (path.startsWith("/deletedata"))	{
		QString query;
		if (path.contains("&"))
			query = getVar(path); 
		execSqlData().deletedata(query, request, response);
	}
	else
	if (path.startsWith("/insert"))	{
		QString query;
		if (path.contains("&"))
			query = getVar(path); 
		execSqlData().insertdata(query, request, response);
	} 
	else
	{
		qDebug() << "Requisição nao tratada  " << path;
	}

	if (logger)
		logger->clear();

	recursiveMutex.unlock();

}
