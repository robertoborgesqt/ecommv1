#pragma once

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>

class cjson : public QObject {

    Q_OBJECT

public:
    explicit cjson( const QString &, const QString &filename="" );
    cjson() {}
    ~cjson         ( );
    bool read      ( );
    void add       ( const QString &var, const QString &value );
    void remove    ( const QString &var, const QString &value );
    void setKey    ( const QString &_key ){ key = _key; }
    bool save      ( );
	QByteArray send      (const QByteArray &, const QStringList &);
    QString json   ( );
    QString get    ( const QString &var, int idx=0 );
    QString getArrayValue(const QString &var, int idx );
    QString getArrayItem(const QString &var, int idx );
    int     getArrayCount(const QString &var);
	bool exists() { return readed; }

public slots:
    void onError   (QNetworkReply::NetworkError);

private: 
	bool readed = false;

protected:
    QString str_url;
    QString key;
    QString filename;
    QFile   file;
    QJsonDocument doc;

	QJsonObject dados;
    QJsonArray array;
};

