//----------------------------------------------------------------------------
// CDataBase
//----------------------------------------------------------------------------
// Data        Hora      Desenvolvedor            Observacoes
//----------------------------------------------------------------------------
// 06-Jan-2014  18:16     Roberto Borges           Definição / Criaçao
//----------------------------------------------------------------------------

#include "cdatabase.h"
//----------------------------------------------------------------------------
// CDataBase Class definition 
//----------------------------------------------------------------------------
CDataBase::CDataBase( const QString &_config )
{
    time_t t;

	json = new cjson( "", _config );

    qDebug() << "Info : starting database connection " << _config;

    srand((unsigned) time(&t));

    dbName          = json->get( "db_name" );
    connName        = json->get( "name");
    connMax         = json->get( "nr_conexoes").toInt();
    m_owner			= json->get( "db_owner").trimmed();

	qDebug() << "Connection on " << dbName << " " << m_owner;
}
//----------------------------------------------------------------------------
CDataBase::~CDataBase()
{
    qDebug() << "Closing database connection on %1" << dbName;
    closeConnections();

	delete json; 

    if ( db.isOpen() )
        db.close();
}
//----------------------------------------------------------------------------
void CDataBase::run( )      
{ 
}
//----------------------------------------------------------------------------
void CDataBase::CloseDB( )      
{ 
    db.close();
}
//----------------------------------------------------------------------------
bool CDataBase::isOpen( )      
{ 
    return db.open();
}
//----------------------------------------------------------------------------
bool CDataBase::beginTransaction( )      
{ 
    return db.transaction();
}
//----------------------------------------------------------------------------
bool CDataBase::endTransaction  ( )      
{ 
    return db.commit();
}
//----------------------------------------------------------------------------
bool CDataBase::hasError( )      
{ 
    return db.lastError().type () != QSqlError::NoError;
}        
//----------------------------------------------------------------------------
QString CDataBase::getLError( )      
{ 
    return db.lastError().text();
}
//----------------------------------------------------------------------------
bool CDataBase::CloseError( )      
{ 
    if ( db.lastError().number() == 2013 || db.lastError().number() == 2006 ) {
        emit hasError( getLError() );
        return true;
    }
    else
        return false;
}        
//----------------------------------------------------------------------------
// get Database instance
//----------------------------------------------------------------------------
QSqlDatabase CDataBase::getDB( )      
{ 
    return db;
}
//----------------------------------------------------------------------------
bool CDataBase::execute( const QString &cmd, QSqlDatabase *dbd = nullptr )
{
    if ( cmd.contains("select ", Qt::CaseInsensitive ) )  {
        QSqlQuery *ret;
        ret = dbd ? open( cmd, dbd ) : open( cmd );
        if ( ret == nullptr )
            return false;
        delete ret;
        return true;
    }
    else
        return execSql( cmd, dbd );
}
//----------------------------------------------------------------------------
bool CDataBase::execute( const QStringList &cmd, QSqlDatabase *dbd = nullptr )
{
    return execSql( cmd, dbd );
}
//----------------------------------------------------------------------------
// Open database connection returning success or not
//----------------------------------------------------------------------------
bool CDataBase::initDatabase( QObject *parente )
{
    connect ( this, SIGNAL( hasError( const QString & ) ), parente, SLOT( dataBaseError( const QString & ) ) );
    QString driverName =  "QMYSQL";

    if ( !db.isDriverAvailable( driverName ) )	{
        QString msg =  QString("Fatal: Driver %1 not avail").arg(driverName);
        setError( msg );
        qDebug() << msg;
        return false;
    }
    return openDatabase( );
}
int CDataBase::getRand()
{ 
	return rand() % 1000000;
}
//----------------------------------------------------------------------------
// open a database with retrieved parameters 
//----------------------------------------------------------------------------
bool CDataBase::openDatabase( )
{
    qDebug() << QString("Allocating %1 connections for %2").arg(connMax).arg(this->dbName);

    for( int i = 0; i <= this->connMax; i++ )  {
        QString nname = QString( "%1-%2" ).arg( this->dbName ).arg( i ) ;
        CLockBase * novo = new CLockBase( nname );
        novo->setState( STOPPED );
        listcon.insert( listcon.begin(), novo );

        db = QSqlDatabase::addDatabase( this->connName, nname );
		
        db.setDatabaseName  ( json->get("db_name") );
        db.setHostName      ( json->get("db_ip")   );
        db.setPort          ( json->get("db_port").toInt() );
        db.setUserName      ( json->get("db_owner") );
        db.setPassword      ( json->get("db_passwd") );

        if ( db.open() == false )  {
            qDebug() <<  QString("Fatal: Only %1/%2 required connections allocated here." ).arg( i ).arg( connMax );
            qDebug() << "Error : " << getLError();
            connMax = i;
            return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------
// return an opened connection
//----------------------------------------------------------------------------
QString CDataBase::getConnection( QSqlDatabase *dbb )
{
    QString ret;
    setLock();
    forever
    {
        msleep(10);
        int numbersort = rand() % listcon.count();
        CLockBase *lbase = listcon.at(numbersort);
        if ( lbase->getState() == STOPPED )
        {
            lbase->setState( READING );
            ret = lbase->getNameConnection();
            *dbb = db.database(ret);
            break;
        } 
    }
    setUnLock();
    return ret;
}
//----------------------------------------------------------------------------
// free opened connections 
//----------------------------------------------------------------------------
bool CDataBase::freeConnection( QString dname )
{
    QList <CLockBase *>::iterator it;
    for( it = this->listcon.begin(); it != listcon.end(); it++ )
    {
        if ( ( *it ) != nullptr && ( *it )->getNameConnection() == dname )
        {
            ( *it )->setState( STOPPED );
            return true;
        }
    }
    return false;
}
//----------------------------------------------------------------------------
// open an database connection 
//----------------------------------------------------------------------------
QSqlQuery *CDataBase::open( const QString &cmd, QSqlDatabase *dbd )
{
    QSqlQuery *qry = new QSqlQuery( cmd, dbd ? *dbd : db );

    if ( hasError() )
    {
        setDbError( *qry, cmd  );
        emit hasError( getLError() );
        delete qry;
        qry = nullptr;
    }
    return qry;
}
//----------------------------------------------------------------------------
// Execute a SQL command
//----------------------------------------------------------------------------
bool CDataBase::execSql( const QString &sql, QSqlDatabase *dbd = nullptr )
{
    QSqlQuery *qry = new QSqlQuery( dbd ? *dbd : db );
    bool ret = qry->exec( sql );
    if ( !ret ) {
        setDbError( *qry, sql );
        emit hasError( getLError() );
    }
    delete qry;
    return ret;
}
//----------------------------------------------------------------------------
// Execute a SQL commands
//----------------------------------------------------------------------------
bool CDataBase::execSql( const QStringList &sql, QSqlDatabase *dbd )
{
    QSqlQuery *qry = new QSqlQuery( *dbd );

    dbd->transaction();

    for( int i=0; i < sql.count(); i++ ) {
        if ( !qry->exec( sql.at(i) ) ) {
            setDbError( *qry, sql.at(i));
            emit hasError( getLError() );
            dbd->rollback();
            delete qry;
            return false;
        }
    }
    dbd->commit();
    delete qry;
    return true;
}
//----------------------------------------------------------------------------        
// Set database exception on log and error std output
//----------------------------------------------------------------------------
void CDataBase::setDbError( const QSqlQuery &qry, const QString &cmd ) 
{
    QString strError = QString("Error: SQL error on <%1> ").arg(cmd) ;
    setError( strError );
    qDebug() <<  strError ;
    QString erro = qry.lastError().text();
    qDebug() << erro;
    qDebug() << QString("Error: DBError: <%1> ").arg(qry.lastError().text());
}
//----------------------------------------------------------------------------
// execute an stored procedure
//----------------------------------------------------------------------------
bool CDataBase::execProcedure( const QString &cmd, QSqlDatabase *dbd = nullptr )
{
    QSqlQuery *qry = new QSqlQuery( dbd ? *dbd : db );
    bool resexec = qry->exec( cmd );

    if ( resexec == false )
    {
        setDbError( *qry, cmd );
        emit hasError( getLError() );
        delete qry;
        qry = nullptr;
    }
    else
    {
        qry->exec( "select @P_ERRO" );
        qry->next();
        int res = qry->value(0).toInt();
        if ( res )
            qDebug() << "Info : Procedure Warning, value returned " << res ;
    }

    delete qry;
    return resexec;
}
//----------------------------------------------------------------------------
// scan connection list and destroyPtr the elements
//----------------------------------------------------------------------------
QString CDataBase::closeConnections( )
{
    QString ret;
    setLock();
    QList <CLockBase *>::iterator it;
    for( it = this->listcon.begin(); it != listcon.end(); it++ )
    {
        ret = ( *it )->getNameConnection();
        db.database(( *it )->getNameConnection()).close();
        delete ( *it );
    }
    setUnLock();
    return ret;
}

void CDataBase::connectionError(const QString & conn)
{ 
	QString ret;
	setLock();
	QList <CLockBase *>::iterator it;
	for (it = this->listcon.begin(); it != listcon.end(); it++)
	{		
		if ((*it)->getNameConnection() == conn)
		db.database((*it)->getNameConnection()).close();
		delete (*it);
	}
	setUnLock(); 
}
//----------------------------------------------------------------------------

