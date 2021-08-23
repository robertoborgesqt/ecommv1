#pragma once
//--------------------------------------------------------------------------------------------------
// sql_tblusuario.h
// Definições do protocolo do modulo r12rf / Satcompany
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 20/08/2018 - 10:16
//--------------------------------------------------------------------------------------------------
// Definicao sql_tblusuario sql - tabela de comandos
//
//--------------------------------------------------------------------------------------------------

#include <QSqlDatabase>
#include <qdebug.h>
#include <string>
#include <iostream>
#include <QSqlQuery>
#include <QSqlError>
#include "cjson.h"
#include "sql_tbl.h"

using namespace std;

enum t_regusuario { ID_USUARIO=0, NOME, APELIDO, CARGO, DATA_NASC, SENHA, RG, CPF, DATA_CRIACAO };
#define fields_usuario QString("ID_USUARIO,NOME,APELIDO,CARGO,DATA_NASC,SENHA,RG,CPF,DATA_CRIACAO" ).split( "," )

/*  
	ID_USUARIO
	NOME
	APELIDO
	CARGO
	DATA_NASC
	SENHA
	RG
	CPF
	DATA_CRIACAO
*/


class sql_tblusuario : public sql_tbl
{

public:
    explicit sql_tblusuario(const QString &_tablename, QSqlDatabase _connection, const QString &_id="" ) :
             sql_tbl(_tablename, _connection)
    {
        setQuery(_id);	
		if (!query->prepare(sql)) {
			qDebug() << "Error: SQL Query prepare fail: " << sql;
		}
    }

    ~sql_tblusuario() {}

    QMap<QString, cjson *> execute()
    {
        QMap<QString, cjson *> list;

        if ( executa() )
        {
			if (query->first()) {
				do {

					QString _id = query->value(ID_USUARIO).toString();
					
					if (list.find(_id) == list.end()) {
						cjson *json = new cjson();
						json->add(fields_usuario[ID_USUARIO],   query->value(ID_USUARIO).toString());
						json->add(fields_usuario[NOME],         query->value(NOME).toString());
						json->add(fields_usuario[APELIDO],      query->value(APELIDO).toString());
						json->add(fields_usuario[CARGO],        query->value(CARGO).toString());
						json->add(fields_usuario[DATA_NASC],    query->value(DATA_NASC).toString());
						json->add(fields_usuario[SENHA],        query->value(SENHA).toString());
						json->add(fields_usuario[RG],           query->value(RG).toString());
						json->add(fields_usuario[CPF],          query->value(CPF).toString());
						json->add(fields_usuario[DATA_CRIACAO], query->value(DATA_CRIACAO).toString());	
						list.insert(_id, json);
					}

				} while (query->next());
			}
        } 
        return list;
    }

    void setQuery(const QString &_id="") 
	{
		if ( _id != "" )
			sql = QString("select * from %1 where %1;").arg(tablename).arg( _id );
		else
    	sql = QString("select * from %1;").arg(tablename);    
    }

    void setQueryUpdate() {
    }

    // bind variables...
    void bindValues(int porta) {
        query->bindValue(":PORTA", porta ); // porta de entrada
    }

    bool updateStatusQuery(const QString & id, int sequence, int status )
    {
		bool ret = true; 
        sqlup = QString("update %4 set "
                        "dt_envio = NOW(), "
                        "status = %3 where "
                        "id = %1 and "
                        "sequencia = %2").arg(id).arg(sequence).arg(status).arg( tablename );

		qDebug() << "UpdateStatusQuery: " << sqlup;

        if ( qr_update->exec(sqlup) == false ) 
		{
            qDebug() << qr_update->lastError();
            qDebug() << qr_update->lastQuery().toLatin1();
            ret = false;
        }
		return ret;
    }

protected:
    QSqlQuery *qr_update;
    QString sqlup;
};

//--------------------------------------------------------------------------------------------------

