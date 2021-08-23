#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QJsonDocument>
#include <QUrl>
#include <QtDebug>

class Endereco : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString idusuario        READ idusuario      WRITE setidusuario      NOTIFY idusuarioChanged)
    Q_PROPERTY(QString logradouro       READ logradouro     WRITE setlogradouro     NOTIFY logradouroChanged)
    Q_PROPERTY(QString cidade           READ cidade         WRITE setcidade         NOTIFY cidadeChanged)
    Q_PROPERTY(QString uf               READ uf             WRITE setuf             NOTIFY ufChanged)
    Q_PROPERTY(QString addressSrv       READ addressSrv     WRITE setaddressSrv     NOTIFY addressSrvChanged)
    Q_PROPERTY(QString pathSrv          READ pathSrv        WRITE setpathSrv        NOTIFY pathSrvChanged)
    Q_PROPERTY(bool    executeUpload    READ executeUpload  WRITE setexecuteUpload  NOTIFY executeUploadChanged)
    Q_PROPERTY(bool    statusproc       READ statusproc     WRITE setstatusproc     NOTIFY statusprocChanged)

public:
    explicit Endereco(QObject *parent = nullptr);

    QString idusuario  ();
    QString logradouro();
    QString cidade ();
    QString uf();

    QString addressSrv  ();
    QString pathSrv     ();
    bool executeUpload  ();
    bool statusproc     ();

    void setlogradouro    ( const QString & logradouro   );
    void setcidade        ( const QString & cidade );
    void setuf            ( const QString & uf  );
    void setidusuario     ( const QString & idusuario  );

    void setaddressSrv    ( const QString & addressSrv );
    void setpathSrv       ( const QString & pathSrv );
    void setexecuteUpload ( bool executeUpload );
    void setstatusproc    ( bool statusproc );

    void prepareData();

    QString finddatabase(const QString &dbname);

    QString final_msg;
    bool final_status;

signals:
    void idusuarioChanged();
    void logradouroChanged  ();
    void cidadeChanged  ();
    void ufChanged  ();
    void addressSrvChanged();
    void pathSrvChanged();
    void showResume();
    void executeUploadChanged(QString logradouro,QString cidade, QString uf    );
    void statusprocChanged();

public slots:
   void runProcess();

private:
    QString m_logradouro ;
    QString m_cidade ;
    QString m_uf ;
    QString m_addressSrv;
    QString m_pathSrv;
    QString m_process;
    QString m_apikey;
    QString m_idusuario;
    bool    m_executeUpload, m_statusproc;
    QUrl    m_url;

    QByteArray payloadFront, payloadBack;
};


