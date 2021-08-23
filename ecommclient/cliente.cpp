#include "cliente.h"

#include "chttpjson.h"
#include <QDir>
#define SRV_ADDR_CLIENTE "http://127.0.0.1:9000/list&cliente"

Cliente::Cliente(QObject *parent) :   QObject(parent)
{
    connect( this, SIGNAL( idusuarioChanged()), this, SLOT(runProcess()));
}

QString Cliente::nomeusurio()
{
    return m_nomeusurio;
}

QString Cliente::idusuario()
{
    return m_idusuario;
}

QString Cliente::sobrenome()
{
    return m_sobrenome;
}

QString Cliente::sexo()
{
    return m_sexo;
}

QString Cliente::emailusuario()
{
    return m_emailusuario;
}

QString Cliente::foneusuario()
{
    return m_foneusuario;
}

QString Cliente::addressSrv()
{
    return m_addressSrv;
}

QString Cliente::pathSrv()
{
    return m_pathSrv;
}

bool Cliente::executeUpload()
{
    return true;
}

bool Cliente::statusproc()
{
    return m_statusproc;
}

void Cliente::setnomeusurio(const QString &nomeusurio)
{
    if (nomeusurio == m_nomeusurio)
        return;

    m_nomeusurio = nomeusurio;
    emit nomeusurioChanged();
}

void Cliente:: setemailusuario( const QString & emailusuario )
{
    if (emailusuario == m_emailusuario)
        return;

    m_emailusuario = emailusuario;
    emit emailusuarioChanged();
}

void Cliente:: setidusuario( const QString & idusuario )
{
    m_idusuario = idusuario;
    emit idusuarioChanged();
}

void Cliente::setfoneusuario ( const QString & foneusuario )
{
    if (foneusuario == m_foneusuario)
        return;

    m_foneusuario = foneusuario;
    emit foneusuarioChanged();
}

void Cliente::setsexo(const QString &sexo)
{
    if (sexo == m_sexo)
        return;

    m_sexo = sexo;
    emit sexoChanged();
}

void Cliente::setsobrenome(const QString &sobrenome)
{
    if (sobrenome == m_sobrenome)
        return;

    m_sobrenome = sobrenome;
    emit sobrenomeChanged();
}

void Cliente::setaddressSrv(const QString &addressSrv)
{
    if (addressSrv == m_addressSrv)
        return;

    m_addressSrv = addressSrv;
}

void Cliente::setpathSrv(const QString &pathSrv)
{
    if (pathSrv == m_pathSrv)
        return;

    m_pathSrv = pathSrv;
}

void Cliente::setexecuteUpload(bool executeUpload)
{
    m_executeUpload = executeUpload;
    m_addressSrv = SRV_ADDR_CLIENTE;
    runProcess();
}

void Cliente::setstatusproc(bool statusproc)
{
    m_statusproc = statusproc;
}

void Cliente::prepareData()
{

}

void Cliente::runProcess()
{
    cjson *sn = new cjson( m_addressSrv );
    QByteArray ret =  sn->getHttp() ;
    qDebug() << "Ret: " << ret;
    QJsonDocument dados  = QJsonDocument::fromJson(ret);
    m_idusuario = dados["body"][0]["id"].toString();
    m_nomeusurio = dados["body"][0]["nome"].toString();
    m_emailusuario = dados["body"][0]["email"].toString();
    m_emailusuario = dados["body"][0]["telefone"].toString();
    emit executeUploadChanged(m_idusuario, m_nomeusurio, m_emailusuario, m_foneusuario );
    delete sn;
}
