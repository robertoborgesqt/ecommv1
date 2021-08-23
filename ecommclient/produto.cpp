#include "produto.h"
#include "chttpjson.h"
#include <QDir>
#define SRV_ADDR_PRODUTO "http://127.0.0.1:9000/list&produto"

Produto::Produto(QObject *parent) :   QObject(parent)
{
    connect( this, SIGNAL( idusuarioChanged()), this, SLOT(runProcess()));
}

QString Produto::descricao()
{
    return m_descricao;
}

QString Produto::id()
{
    return m_id;
}

QString Produto::preco()
{
    return m_preco;
}

QString Produto::estoque()
{
    return m_estoque;
}

QString Produto::unidade()
{
    return m_unidade;
}

QString Produto::addressSrv()
{
    return m_addressSrv;
}

QString Produto::pathSrv()
{
    return m_pathSrv;
}

bool Produto::executeUpload()
{
    return m_executeUpload;
}

bool Produto::statusproc()
{
    return m_statusproc;
}

void Produto::setdescricao(const QString &descricao)
{
    if (descricao == m_descricao)
        return;

    m_descricao = descricao;
    emit descricaoChanged();
}

void Produto:: setid( const QString & id )
{
    m_id = id;
    emit idChanged();
}

void Produto::setunidade(const QString &unidade)
{
    if (unidade == m_unidade)
        return;

    m_unidade = unidade;
    emit unidadeChanged();
}

void Produto::setestoque(const QString &estoque)
{
    if (estoque == m_estoque)
        return;

    m_estoque = estoque;
    emit estoqueChanged();
}

void Produto::setpreco(const QString &preco)
{
    if (preco == m_preco)
        return;

    m_preco = preco;
    emit precoChanged();
}

void Produto::setaddressSrv(const QString &addressSrv)
{
    if (addressSrv == m_addressSrv)
        return;

    m_addressSrv = addressSrv;
}

void Produto::setpathSrv(const QString &pathSrv)
{
    if (pathSrv == m_pathSrv)
        return;

    m_pathSrv = pathSrv;
}

void Produto::setexecuteUpload(bool executeUpload)
{
    m_executeUpload = executeUpload;
    m_addressSrv = SRV_ADDR_PRODUTO;
    runProcess();
}

void Produto::setstatusproc(bool statusproc)
{
    m_statusproc = statusproc;
}

void Produto::prepareData()
{

}

void Produto::runProcess()
{
    cjson *sn = new cjson( m_addressSrv );
    QString listastr =  sn->getHttp() ;
    qDebug() << "Ret: " << listastr;
    emit atualizaLista(listastr);
    delete sn;
}
