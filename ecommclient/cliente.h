#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QJsonDocument>
#include <QUrl>
#include <QtDebug>

class Cliente : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nomeusurio       READ nomeusurio     WRITE setnomeusurio     NOTIFY nomeusurioChanged)
    Q_PROPERTY(QString sobrenome        READ sobrenome      WRITE setsobrenome      NOTIFY sobrenomeChanged)
    Q_PROPERTY(QString sexo             READ sexo           WRITE setsexo           NOTIFY sexoChanged)
    Q_PROPERTY(QString emailusuario     READ emailusuario   WRITE setemailusuario   NOTIFY emailusuarioChanged)
    Q_PROPERTY(QString foneusuario      READ foneusuario    WRITE setfoneusuario    NOTIFY foneusuarioChanged)
    Q_PROPERTY(QString idusuario        READ idusuario      WRITE setidusuario      NOTIFY idusuarioChanged)
    Q_PROPERTY(QString addressSrv       READ addressSrv     WRITE setaddressSrv     NOTIFY addressSrvChanged)
    Q_PROPERTY(QString pathSrv          READ pathSrv        WRITE setpathSrv        NOTIFY pathSrvChanged)
    Q_PROPERTY(bool    executeUpload    READ executeUpload  WRITE setexecuteUpload  NOTIFY executeUploadChanged)
    Q_PROPERTY(bool    statusproc       READ statusproc     WRITE setstatusproc     NOTIFY statusprocChanged)

public:
    explicit Cliente(QObject *parent = nullptr);

    QString nomeusurio  ();
    QString emailusuario();
    QString foneusuario ();
    QString idusuario();
    QString sobrenome();
    QString sexo();

    QString addressSrv  ();
    QString pathSrv     ();
    bool executeUpload  ();
    bool statusproc     ();

    void setnomeusurio    ( const QString & nomeusurio   );
    void setemailusuario  ( const QString & emailusuario );
    void setfoneusuario   ( const QString & foneusuario  );
    void setsexo          ( const QString & sexo  );
    void setsobrenome     ( const QString & sobrenome );
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
    void nomeusurioChanged  ();
    void sexoChanged  ();
    void sobrenomeChanged  ();
    void emailusuarioChanged();
    void foneusuarioChanged ();
    void addressSrvChanged();
    void idusuarioChanged();
    void pathSrvChanged();
    void showResume();
    void executeUploadChanged(QString id,QString nome, QString email, QString telefone  );
    void executeUploadChangedDuplicado(QString id);
    void executeUploadChangedError(QString id,QString nome, QString email, QString telefone );
    void executeUploadOffline(QString id,QString nome, QString email, QString telefone );
    void statusprocChanged();

public slots:
   void runProcess();

private:
    QString m_nomeusurio ;
    QString m_emailusuario ;
    QString m_sobrenome ;
    QString m_sexo ;
    QString m_foneusuario;
    QString m_addressSrv;
    QString m_pathSrv;
    QString m_process;
    QString m_apikey;
    QString m_idusuario;
    bool    m_executeUpload, m_statusproc;
    QUrl    m_url;

    QByteArray payloadFront, payloadBack;
};

#endif // BACKEND_H
