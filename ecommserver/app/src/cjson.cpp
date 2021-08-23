
// Fantastico!
// Simples e eficiente..

#include "cjson.h"

cjson::cjson( const QString &_url, const QString &_filename ):str_url(_url),filename(_filename)
{
    if ( filename.size() ) {
		readed = read();
		if (!readed) { // assume default data model content
            save();
            read();
        }
    }
}

cjson::~cjson() {

}

bool cjson::read()
{
    file.setFileName(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonParseError parser;
        doc = QJsonDocument::fromJson(file.readAll(), &parser);

        if ( parser.error != QJsonParseError::NoError ) {
            qDebug() << parser.errorString();
            return false;
        }

        if (doc.isEmpty())	{
            qDebug() << "isEmpty json" ;
            return false;
        }


        QByteArray ret = doc.toJson();
        dados = doc.object();

        array.append(dados);
        file.close();
        return true;
    }
    return false;
}

void cjson::add( const QString &var, const QString &value )
{
    dados[var] = value;
    array.append(dados);
}

QString cjson::get(const QString &var, int idx )
{
    if (idx < array.count()) {
        dados = array.at(idx).toObject();
        return dados[var].toString();
    }
    return QString();
}

QString cjson::getArrayValue(const QString &var, int idx )
{
    QString ret;
    for( int i=0; i < array.count(); i++ )
    {
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

QString cjson::getArrayItem(const QString &var, int idx )
{
    QString ret;
    for( int i=0; i < array.count(); i++ )
    {
        dados = array.at(i).toObject();
        if ( !dados[var].isUndefined() )
        {
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
    for( int i=0; i < array.count(); i++ )
    {
        dados = array.at(i).toObject();
        if ( !dados[var].isUndefined() )
        {
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
    for( int i = 0; i < array.count(); i++ )
    {
        if ( array[i] == dados ) {
            array.removeAt(i);
            break;
        }
    }
}

QString cjson::json()
{
    QJsonDocument doc(dados);
    return doc.toJson(QJsonDocument::Indented);
}

bool cjson::save() {
    QString ret = json();
    if ( file.open( QIODevice::WriteOnly ) ) {
        file.write(ret.toLatin1());
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
        QUrl req(str_url);

		if (params.count())
		{
			for (int i = 0; i < params.count(); i++)
			{
				req.setQuery(params.at(i)); 
			}
		} 

        QNetworkRequest request(req);

        request.setRawHeader("User-Agent", "Test");
        request.setRawHeader("X-Custom-User-Agent", "Test");
        request.setRawHeader("Content-Type", "application/json");
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

void cjson::onError(QNetworkReply::NetworkError error)
{
    qDebug() << error;

}

