#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QJsonDocument>
#include <QUrl>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>

class Produto : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString descricao        READ descricao      WRITE setdescricao      NOTIFY descricaoChanged)
    Q_PROPERTY(QString id               READ id             WRITE setid             NOTIFY idChanged)
    Q_PROPERTY(QString preco            READ preco          WRITE setpreco          NOTIFY precoChanged)
    Q_PROPERTY(QString estoque          READ estoque        WRITE setestoque        NOTIFY estoqueChanged)
    Q_PROPERTY(QString unidade          READ unidade        WRITE setunidade        NOTIFY unidadeChanged)
    Q_PROPERTY(QString addressSrv       READ addressSrv     WRITE setaddressSrv     NOTIFY addressSrvChanged)
    Q_PROPERTY(QString pathSrv          READ pathSrv        WRITE setpathSrv        NOTIFY pathSrvChanged)
    Q_PROPERTY(bool    executeUpload    READ executeUpload  WRITE setexecuteUpload  NOTIFY atualizaLista)
    Q_PROPERTY(bool    statusproc       READ statusproc     WRITE setstatusproc     NOTIFY statusprocChanged)

public:
    explicit Produto(QObject *parent = nullptr);

    QString descricao  ();
    QString unidade();
    QString id();
    QString preco();
    QString estoque();

    QString addressSrv  ();
    QString pathSrv     ();
    bool executeUpload  ();
    bool statusproc     ();

    void setdescricao    ( const QString & descricao   );
    void setunidade  ( const QString & unidade );
    void setestoque          ( const QString & estoque  );
    void setpreco     ( const QString & preco );
    void setid ( const QString & id  );

    void setaddressSrv    ( const QString & addressSrv );
    void setpathSrv       ( const QString & pathSrv );
    void setexecuteUpload ( bool executeUpload );
    void setstatusproc    ( bool statusproc );

    void prepareData();

    QString finddatabase(const QString &dbname);

    QString final_msg;
    bool final_status;

signals:
    void descricaoChanged  ();
    void estoqueChanged  ();
    void precoChanged  ();
    void unidadeChanged();
    void addressSrvChanged();
    void idChanged();
    void pathSrvChanged();
    void showResume();
    void atualizaLista(const QString &listastr);
    void statusprocChanged();

public slots:
   void runProcess();

private:
    QString m_descricao ;
    QString m_unidade ;
    QString m_preco ;
    QString m_estoque ;
    QString m_addressSrv;
    QString m_pathSrv;
    QString m_process;
    QString m_apikey;
    QString m_id;
    bool    m_executeUpload, m_statusproc;
    QUrl    m_url;

    QByteArray payloadFront, payloadBack;
};
