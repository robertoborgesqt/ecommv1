//--------------------------------------------------------------------------------------------------
// httprequesthandler.h
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 23/05/2019 - 10:16
//--------------------------------------------------------------------------------------------------
// Definicao RequestHandler
//--------------------------------------------------------------------------------------------------
#pragma once

#include "httprequesthandler.h"
using namespace webserverspace;

class RequestHandler : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RequestHandler)

public:
    RequestHandler(QObject* parent=nullptr);
    void service(HttpRequest& request, HttpResponse& response);
protected: 
	QString getVar(const QString &);
};


