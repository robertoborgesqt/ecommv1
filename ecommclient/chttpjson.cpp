
//--------------------------------------------------------------------------------------------------
// cjson.cpp
// Definicoes cjson http requester
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 23/02/2020 - 11:43
//--------------------------------------------------------------------------------------------------


#include "chttpjson.h"
#include <QUrlQuery>
#include <QFileInfo>
#include <QDir>

// primary constructor, usa url e filename para abertura de arquivos de cfg
cjson::cjson( const QString &_url, const QString &_filename )
    : QObject(nullptr)
    ,str_url(_url)
    ,filename(_filename)
{
    if ( filename.size() ) {
        readed = read();
        if (!readed) { // assume default data model content
            save();
            read();
        }
    }
}

// primary constructor, usa filename para abertura de arquivos
cjson::cjson( const QString& _filename, bool *ok)
    : QObject(nullptr)
    , filename(_filename)
{
    readed = read();
    *ok = readed;
}

// secondary constructor, usa apenas urls para get/post
cjson::cjson(cjson *other)
    : QObject(nullptr)
    , str_url(other->getUrl())
{
    doc = other->toDocument();
    dados = other->getData();
    array = other->getArray();
}

bool cjson::makeDir(const QString& path)
{
    QDir dir{path};

    if (!dir.exists()){
        if (!dir.mkpath(path)){
            qDebug() << "Não foi possivel criar " << path;
            return false;
        };
    }
    return true;
}

bool cjson::moveFile(const QString& _dir)
{
    QFileInfo info(filename);
    QString absfile = info.fileName();
    QString dest = _dir + absfile;
    QFile newFile;
    newFile.setFileName(dest);
    if (newFile.open(QIODevice::WriteOnly)){
        QByteArray ret = doc.toJson();
        newFile.write(ret);
        newFile.close();
        file.remove();
        return true;
    }
    return false;
}

// Leitura de arquivos
bool cjson::read()
{
    file.setFileName(filename);
    if (file.open(QIODevice::ReadOnly)){
        QJsonParseError parser;
        QByteArray buffer = file.readAll();
        doc = QJsonDocument::fromJson(buffer, &parser);

        if ( parser.error != QJsonParseError::NoError ) {
            qDebug() << parser.errorString();
            return false;
        }

        if (doc.isEmpty())	{
            qDebug() << "isEmpty json" ;
            return false;
        }

        dados = doc.object();

        array.append(dados);
        file.close();
        return true;
    }
    return false;
}

bool cjson::parseBuffer(const QByteArray& buffer)
{
        QJsonParseError parser;
        doc = QJsonDocument::fromJson(buffer, &parser);

        if (parser.error != QJsonParseError::NoError) {
            qDebug() << parser.errorString();
            return false;
        }

        if (doc.isEmpty()) {
            qDebug() << "isEmpty json";
            return false;
        }

        dados = doc.object();

        array.append(dados);
        return true;
}

// Adiciona variavel ao json..
void cjson::add( const QString &var, const QString &value )
{
    dados[var] = value;
    array.append(dados);
}

// busca variavel do json..
QString cjson::get(const QString &var, int idx )
{
    if (idx < array.count()) {
        dados = array.at(idx).toObject();
        return dados[var].toString();
    }
    return QString();
}

// retorna array
QString cjson::getArrayValue(const QString &var, int idx )
{
    QString ret;
    for( int i=0; i < array.count(); i++ ){
        dados = array.at(i).toObject();
        if ( !dados[var].isUndefined() )
        {
            QJsonObject itarr = dados[var].toObject();
            QString name = itarr.keys().at(idx);
            ret = itarr[name].toString();
            break;
        }
    }
    return ret;
}

// retorna item de array
QString cjson::getArrayItem(const QString &var, int idx )
{
    QString ret;
    for( int i=0; i < array.count(); i++ ){
        dados = array.at(i).toObject();
        if ( !dados[var].isUndefined() ){
            QJsonObject itarr = dados[var].toObject();
            ret = itarr.keys().at(idx);
            break;
        }
    }
    return ret;
}

int cjson::getArrayCount(const QString &var)
{
    int ret=0;
    for( int i=0; i < array.count(); i++ ){
        dados = array.at(i).toObject();
        if ( !dados[var].isUndefined() ){
            QJsonObject itarr = dados[var].toObject();
            ret = itarr.keys().count();
            break;
        }
    }
    return ret;
}

void cjson::remove( const QString &var, const QString &value )
{
    dados[var] = value;
    for( int i = 0; i < array.count(); i++ ){
        if ( array[i] == dados ) {
            array.removeAt(i);
            break;
        }
    }
}

QString cjson::json()
{
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}

bool cjson::save()
{
    QString ret = json();
    file.setFileName(filename);
    if ( file.open( QIODevice::WriteOnly ) ) {
        file.write(ret.toUtf8());
        array.fromStringList( ret.split('\n') );
        file.close();
        return true;
    }
    return false;
}

QByteArray cjson::send(const QByteArray &ba, const QStringList &params )
{
        QByteArray jsonString = ba;
        QByteArray postDataSize = QByteArray::number(jsonString.size());
        QUrl req(str_url+path);
        if (params.count()){
            for (int i = 0; i < params.count(); i++){
                req.setQuery(params.at(i));
            }
        }

        QNetworkRequest request(req);

        request.setRawHeader("User-Agent", "ServiceAllert");
        request.setRawHeader("X-Custom-User-Agent", "ServiceAllert");
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
        request.setRawHeader("Content-Length", postDataSize);
        QNetworkAccessManager test;

        QEventLoop loop;
        connect(&test, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
        QNetworkReply * reply = test.post(request, jsonString);
        QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
        loop.exec();

        QByteArray response = reply->readAll();
        qDebug() << response;
        return response;

}

QByteArray cjson::getHttp(const QStringList &keys)
{
    qDebug() << "getHttp " << str_url+path;
    QByteArray jsonString;
    QByteArray postDataSize = QByteArray::number(jsonString.size());

    QUrlQuery qry;
    QJsonObject obj;
    QVariantMap feed;

    for (int i = 0; i < keys.count(); i++){
        QString key = keys.at(i).split("=").at(0);
        QString val = keys.at(i).split("=").at(1);
        feed.insert(key, val);
        qry.addQueryItem(key, val);
    }

    jkeys = QJsonDocument::fromVariant(feed);
    QByteArray payload = jkeys.toJson();
    qDebug() << QVariant(payload).toString();
    QString whost = str_url + path;

    QUrl req(whost);

    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setUrl(req);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json; charset=utf-8");

    QNetworkAccessManager* restclient; //in class
    restclient = new QNetworkAccessManager(this); //constructor
    net_error = false;
    QEventLoop loop;
    QObject::connect(restclient, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply* reply = restclient->post(request, payload);
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    loop.exec();

    QByteArray ret;
    if (reply->bytesAvailable()) {
        ret = reply->readAll();
    } else {
        qWarning("Nao ha dados recebidos..");
    }

    if (ret.contains("msg_error")) {
        qWarning() << "Mensagem de error recebida " << ret;
    }

    reply->deleteLater();
    delete restclient;
    return ret;
}

QByteArray cjson::getHttp( )
{
    qDebug() << "getHttp " << str_url + path;
    QByteArray jsonString;
    QByteArray postDataSize = QByteArray::number(jsonString.size());

    QUrlQuery qry;
    QJsonObject obj;
    QVariantMap feed;

    for (int i = 0; i < keys.count(); i++){
        QString key = keys.at(i).split("=").at(0);
        QString val = keys.at(i).split("=").at(1);
        feed.insert(key, val);
        qry.addQueryItem(key, val);
    }

    jkeys = QJsonDocument::fromVariant(feed);
    QByteArray payload = jkeys.toJson();
    QString whost = str_url + path;
    QUrl req(whost);
    QNetworkRequest request;
    request.setUrl(req);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    QNetworkAccessManager* restclient; //in class
    restclient = new QNetworkAccessManager(this); //constructor
    net_error = false;
    QEventLoop loop;
    QObject::connect(restclient, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply* reply = restclient->post(request, payload);
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    loop.exec();

    QByteArray ret;
    if (!net_error)
         ret = reply->readAll();
    reply->deleteLater();
    delete restclient;
    return ret;
}

void cjson::onError(QNetworkReply::NetworkError error)
{
    net_error = true;
    qDebug() << "Network Error: " << error;
}
