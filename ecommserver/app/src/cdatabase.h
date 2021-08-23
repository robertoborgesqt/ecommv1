//----------------------------------------------------------------------------
// CDataBase
//----------------------------------------------------------------------------
// Data        Hora      Desenvolvedor            Observacoes
//----------------------------------------------------------------------------
// 17-08-2018  09:35     Roberto Borges           Definição / Criaçao
//----------------------------------------------------------------------------

#pragma once


#include <iostream>
#include <string>

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QList>
#include <QStringList>
#include "cconcurrent.h" 
#include "cjson.h" 

//----------------------------------------------------------------------------
// LockBase Class definition 
//----------------------------------------------------------------------------
class CLockBase : public CConcurrent
{
    Q_OBJECT

public:
    CLockBase( const QString& n )        { name = n;    }
    ~CLockBase( )                        {  }
    QString getNameConnection()          { return name; }
    void setNameConnection( QString n )  { name = n;    }

private:
    QString name;
};

//----------------------------------------------------------------------------
// CDataBase Class definition 
//----------------------------------------------------------------------------

class CDataBase : public CConcurrent
{
    Q_OBJECT

public:
    CDataBase( const QString & );
   ~CDataBase( );

public:
    void run                ( );
    bool freeConnection     ( QString );
    bool execSql            ( const QString &, QSqlDatabase *);
    bool execSql            ( const QStringList &, QSqlDatabase *);
    void setDbError         ( const QSqlQuery &, const QString & );
    bool execProcedure      ( const QString &cmd, QSqlDatabase * );
    void CloseDB            ( );
    bool isOpen             ( );
    bool beginTransaction   ( );
    bool endTransaction     ( );
    bool hasError           ( );
    bool CloseError         ( );
    bool execute            ( const QString & , QSqlDatabase * );
    bool execute            ( const QStringList & , QSqlDatabase * );
    void setError           ( const QString &err ){ m_error = err; }
    bool initDatabase       ( QObject * );
	int  getRand            ( );           
    QString getConnection   ( QSqlDatabase * );
    QString getLError       ( );
    QSqlDatabase getDB      ( );
    QString closeConnections( );
    QString getName         ( ){ return dbName; }
    QString getOwner        ( ) { return m_owner; }
    QSqlQuery *open         ( const QString &, QSqlDatabase *dbd = nullptr );

protected:
    QSqlDatabase  db;
    QString       m_error, m_owner, connName, dbName;
    QLibrary     *libdb;
    QList <CLockBase *> listcon;
    int   connMax=0;
	cjson *json;

private: 
    bool openDatabase    ( );

signals: 
    void hasError( const QString & );

public slots:
	void connectionError(const QString &);

};
//----------------------------------------------------------------------------

