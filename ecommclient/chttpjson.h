#pragma once

//--------------------------------------------------------------------------------------------------
// cjson.h
// Definicoes cjson http requester
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 23/02/2020 - 11:43
//--------------------------------------------------------------------------------------------------

#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QTextCodec>

using namespace std;

#define jsonparent ((cjson*)parent)

class cjson : public QObject
{
    Q_OBJECT

public:
    explicit cjson ( const QString &, const QString &filename="" );
    explicit cjson ( const QString &, bool * );
    explicit cjson ( cjson * );
    cjson () = default;
    virtual ~cjson() = default;

public:
    bool read ( );
    bool parseBuffer(const QByteArray & buffer);
    void add ( const QString &var, const QString &value );
    void remove ( const QString &var, const QString &value );
    void setKey ( const QString &_key ){ key = _key; }
    bool moveFile ( const QString&  );
    bool save ( );
    void run ( ) {};

    QJsonDocument toDocument() { return doc; }

    QByteArray send ( const QByteArray &, const QStringList & );
    QByteArray getHttp ( );
    QByteArray getHttp ( const QStringList& keys);
    QByteArray getHttp2 ( const QStringList& keys);
    QString json();
    QString getUrl() { return str_url; }
    QString get ( const QString &var, int idx=0 );
    QString getArrayValue( const QString &var, int idx );
    QString getArrayItem ( const QString &var, int idx );
    int getArrayCount( const QString &var);
    bool exists() { return readed; }
    void setPath(const QString _path) { path = _path; }

    bool makeDir(const QString& path);

    QString toString( ){
        return doc.toJson();
    }

    QString getParamKeys(const QString &name ){
        QString value = jkeys.object()[name].toString();
        return QString("%1=%2").arg(name).arg(value);
    }

    QString getDocKeys(const QString& name){
        QJsonObject obj = doc.object();
        QJsonValue val = obj[name];
        QByteArray str = val.toString().toLatin1();
        return QString{name + "=" + val.toString()};
    }

    int getIntDocKeys(const QString& name){
        int value = doc.object()[name].toInt();
        return value;
    }

    //unsigned long getUlongDocKeys(const QString& name){
    //    double value = doc.object()[name].toDouble() / 1000;
    //    unsigned long ret = static_cast<unsigned long>(value);
    //    return ret;
    //}

    unsigned long getUlongDocKeys(const QString &name){

     if (doc.object()[name].isDouble()) {
        double value = doc.object()[name].toDouble() / 1000;
        unsigned long ret = static_cast<unsigned long>(value);
        return ret;
     }

      QString str = doc.object()[name].toString();
      double value = str.toDouble() / 1000;
      auto ret = static_cast<unsigned long>(value);
      return ret;
    }

    QJsonObject getObject() const {
        return doc.object();
    }

    QJsonArray getArray() const {
        return array;
    }

    void setFilename(const QString& fname){
        filename = fname;
        file.setFileName(filename);
    }

    QJsonObject getData(){
        return dados;
    }

    QString getValue(const QString& name) { return doc.object()[name].toString(); }

    QString rootdir;

    void addkey(const QString& _key, const QVariant& _value)  {
        QString str = QString("%1=%2").arg(_key).arg(_value.toString());
        keys.append(str);
    }

    QString getKey(const QString& name){
        foreach(QString str, keys){
            if (str.split("=").at(0) == name)
                return str.split("=").at(1);
        }
        return QString();
    }

    bool net_error = false;

public slots:
    void onError   (QNetworkReply::NetworkError);
    void sslErrorOccured(QNetworkReply*, QList<QSslError>){
        qDebug() << "SSL Errors..";
    }

private:
    bool readed = false;

protected:
    QString str_url;
    QString key;
    QString filename;
    QString path;
    QFile file;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QJsonDocument doc;
    QJsonDocument jkeys;
    QJsonObject dados;
    QJsonArray  array;
    QStringList keys;
};

