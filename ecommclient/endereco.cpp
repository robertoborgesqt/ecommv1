#include "Endereco.h"

#include "chttpjson.h"
#include <QDir>
#define SRV_ADDR_Endereco "http://127.0.0.1:9000/list&endereco"

Endereco::Endereco(QObject *parent) :   QObject(parent)
{
    connect( this, SIGNAL( idusuarioChanged()), this, SLOT(runProcess()));
}

QString Endereco::logradouro()
{
    return m_logradouro;
}

QString Endereco::idusuario()
{
    return m_idusuario;
}

QString Endereco::cidade()
{
    return m_cidade;
}

QString Endereco::uf()
{
    return m_uf;
}

QString Endereco::addressSrv()
{
    return m_addressSrv;
}

QString Endereco::pathSrv()
{
    return m_pathSrv;
}

bool Endereco::executeUpload()
{
    return true;
}

bool Endereco::statusproc()
{
    return m_statusproc;
}

void Endereco::setlogradouro(const QString &logradouro)
{
    if (logradouro == m_logradouro)
        return;

    m_logradouro = logradouro;
    emit logradouroChanged();
}

void Endereco:: setcidade( const QString & cidade )
{
    if (cidade == m_cidade)
        return;

    m_cidade = cidade;
    emit cidadeChanged();
}

void Endereco:: setidusuario( const QString & idusuario )
{
    m_idusuario = idusuario;
    m_addressSrv = QString("%1+id=%2").arg(SRV_ADDR_Endereco).arg(m_idusuario);
    runProcess();
}

void Endereco::setuf ( const QString & uf )
{
    if (uf == m_uf)
        return;

    m_uf = uf;
    emit ufChanged();
}

void Endereco::setaddressSrv(const QString &addressSrv)
{
    if (addressSrv == m_addressSrv)
        return;

    m_addressSrv = addressSrv;
}

void Endereco::setpathSrv(const QString &pathSrv)
{
    if (pathSrv == m_pathSrv)
        return;

    m_pathSrv = pathSrv;
}

void Endereco::setexecuteUpload(bool executeUpload)
{
    m_executeUpload = executeUpload;
    m_addressSrv = SRV_ADDR_Endereco;
    runProcess();
}

void Endereco::setstatusproc(bool statusproc)
{
    m_statusproc = statusproc;
}

void Endereco::prepareData()
{

}

void Endereco::runProcess()
{
    cjson *sn = new cjson( m_addressSrv );
    QByteArray ret =  sn->getHttp() ;
    qDebug() << "Ret: " << ret;
    QJsonDocument dados  = QJsonDocument::fromJson(ret);
    m_idusuario = dados["body"][0]["id"].toString();
    m_logradouro = dados["body"][0]["logradouro"].toString();
    m_cidade = dados["body"][0]["cidade"].toString();
    m_uf = dados["body"][0]["uf"].toString();
    emit executeUploadChanged(m_logradouro, m_cidade, m_uf );
    delete sn;
}
