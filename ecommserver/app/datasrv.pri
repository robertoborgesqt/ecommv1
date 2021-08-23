# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")

INCLUDEPATH += \
./http/httpserver \
./http/logging \
./http/qtservice \
./src

HEADERS += ./http/httpserver/httpcookie.h \
    ./http/httpserver/httpglobal.h \
    ./http/httpserver/httprequest.h \
    ./http/httpserver/httpresponse.h \
    ./http/httpserver/httpsession.h \
    ./http/logging/logglobal.h \
    ./http/logging/logmessage.h \
    ./src/sql_listdata.h \
    ./src/sql_tbl.h \
    ./src/sql_tblusuario.h \
    ./src/cconcurrent.h \
    ./src/cdatabase.h \
    ./src/cjson.h \
    ./http/logging/dualfilelogger.h \
    ./http/logging/filelogger.h \
    ./http/httpserver/httpconnectionhandler.h \
    ./http/httpserver/httpconnectionhandlerpool.h \
    ./http/httpserver/httplistener.h \
    ./http/httpserver/httprequesthandler.h \
    ./http/httpserver/httpsessionstore.h \
    ./http/logging/logger.h \
    ./src/requesthandler.h \
    ./src/execsqldata.h \
    ./src/sendcss.h \
    ./http/httpserver/staticfilecontroller.h
SOURCES += ./src/cdatabase.cpp \
    ./src/cjson.cpp \
    ./http/logging/dualfilelogger.cpp \
    ./http/logging/filelogger.cpp \
    ./http/httpserver/httpconnectionhandler.cpp \
    ./http/httpserver/httpconnectionhandlerpool.cpp \
    ./http/httpserver/httpcookie.cpp \
    ./http/httpserver/httpglobal.cpp \
    ./http/httpserver/httplistener.cpp \
    ./http/httpserver/httprequest.cpp \
    ./http/httpserver/httprequesthandler.cpp \
    ./http/httpserver/httpresponse.cpp \
    ./http/httpserver/httpsession.cpp \
    ./http/httpserver/httpsessionstore.cpp \
    ./http/logging/logger.cpp \
    ./http/logging/logmessage.cpp \
    ./src/execsqldata.cpp \
    ./src/main.cpp \
    ./src/requesthandler.cpp \
    ./http/httpserver/staticfilecontroller.cpp
