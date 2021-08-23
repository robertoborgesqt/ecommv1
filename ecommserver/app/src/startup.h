
#ifndef STARTUP_H
#define STARTUP_H

#include <QtCore/QCoreApplication>
#include "qtservice.h"
#include "httplistener.h"

using namespace webserverspace;

class Startup : public QtService<QCoreApplication>
{
public:

    Startup(int argc, char *argv[]);

protected:

    void start();
    void stop();

private:
    HttpListener* listener;

};

#endif // STARTUP_H
