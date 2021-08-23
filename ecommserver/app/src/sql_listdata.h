#pragma once
//--------------------------------------------------------------------------------------------------
// sql_listdata.h
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 23/05/2019 - 10:16
//--------------------------------------------------------------------------------------------------
// Definicao sql_listdata sql - lista dados 
//
//--------------------------------------------------------------------------------------------------

#include <QSqlDatabase>
#include <QNetworkRequest>
#include <QSqlRecord>
#include <qdebug.h>
#include <string>
#include <iostream>
#include <QSqlQuery>
#include <QSqlError>
#include "cjson.h"
#include "sql_tbl.h"
#include "css.h"

using namespace std;

//#define HTML_INIT(A) QString( "<h1 style=\"color:#5e9ca0; text-align: center;\">%1</h1><table class=\"blueTable\" >").arg(A).toLatin1()
#define HTML_INIT(A) QString( "<h1 style=\"color:#5e9ca0; text-align: center;\">%1</h1><table class=\"iblueTable\" style=\"width: 253px; margin-left: auto; margin-right: auto;\" border=\"1\" >").arg(A).toLatin1()
#define HTML_INITW(A) QString( "<h2 style=\"color:#ffffff; text-align: center;\">%1</h1><table class=\"iblueTable\" style=\"width:180px; margin-left: auto; margin-right: auto;\" border=\"0\" >").arg(A).toLatin1()
#define HTML_THREAD      "<thead><tr>"
#define HTML_THREAD_END  "</tr></thead>"
#define HTML_BEGIN_DIV   "<tr>"
#define HTML_END_DIV     "</tr>"
#define HTML_LINE(A)     QString( "<td> %1 </td>" ).arg(A).toLatin1()
#define HTML_LINE_CENTER(A) QString( "<td><center>%1</center></td>" ).arg(A).toLatin1()
#define HTML_BODY_BEGIN  "<tbody><tr>"
#define HTML_BODY_END    "</tr></tbody>"
#define HTML_CLOSE_TABLE "</table>"
#define HTML_CENTER      "<center>"
#define HTML_CENTER_END  "</center>"
//style = "background-color: gainsboro"
#define HTML_ADDDIV "<div class=\" wrapper\" style=\"grid-template-columns: _GRID_COLUMNS ; background-color: gainsboro \" >" 

#define HTML_ITEM_HEAD_DIV(FIELD)  QString( "<div style = \"background-color: gainsboro\"><center><b>%1</b></center></div>\n" ).arg(FIELD).toLatin1()   
#define HTML_ITEM_ADD_DIV(FIELD)   QString( "<div style = \"background-color: rgb(247, 239, 239)\"><center>%1</center></div>\n" ).arg(FIELD).toLatin1() 
#define HTML_ITEM_ADD_BUT(TBL,FIELD)   QString( " \
<div style = \"background-color: rgb(247, 239, 239)\"><center> \
<a  class =\"btn btn-sm btn-primary\" onClick =\"sel%2('%1')\" VALUE=\">\" >></a> \
</center></div>\n" ).arg(FIELD).arg(TBL).toLatin1()

#define HTML_CLOSEDIV "</div>" 

#define HTML_BUTTON_STYLED "<a class = \"btn btn-smr btn-secondary display-4\" href = \"device1.html\">teste</a>"
#define HTML_BUTTON_STYLED "<a class = \"btn btn-smr btn-secondary display-4\" href = \"device1.html\">teste</a>"
#define HTML_BUTTON_STYLED "<a class = \"btn btn-smr btn-secondary display-4\" href = \"device1.html\">teste</a>"

#define TEMPLATE_OPEN_HTML    "etc/open-page.html"
#define TEMPLATE_OPEN_HTML2   "etc/open-page2.html"
#define TEMPLATE_CLOSE_HTML   "etc/close-page.html"
#define TEMPLATE_MAP_HTML     "etc/mapa.html"
#define TEMPLATE_SECTION_HTML "etc/open-section.html"
#define TEMPLATE_SECTION_HTML2 "etc/open-sectionback.html"

#define TEMPLATE_HTML  "etc/template.html"
#define TEMPLATE_HTML2 "etc/template2.html"

enum TP_SRV { T_QUERY, T_INSERT_POS, T_INSERT_CMD, T_INSERT_DATA, T_BUSCATELE, T_UPDATE_DATA,
	          T_DELETE_DATA, T_INSERT_COMBO, T_FINDHOSTS, T_FINDHOSPEDES, T_PAREAMENTO_RES
};

#define DEF_MASK "yyyy-MM-dd hh:mm:ss"
#define SHT_MASK "MM-dd hh:mm:ss"
#define COM_MASK "dd/MM/yyyy hh:mm:ss"

extern int ipport;
extern bool iplocal;

class sql_listdata : public sql_tbl
{

public:

	explicit sql_listdata(const QString &_str, QSqlDatabase _connection, int _limite)
		: sql_tbl(_str, _connection, _limite)
	{
		setQuery();
		if (!query->prepare(sql)) {
			qDebug() << "Error: SQL Query prepare fail: " << sql;
			error = true;
		}
	}

	explicit sql_listdata(const QString &_str, QSqlDatabase _connection, int _limite, TP_SRV tp)
		: sql_tbl(_str, _connection, _limite, true)
	{
		setQuery(tp);
		if (!query->prepare(sql)) {
			qDebug() << "Error: SQL Query prepare fail: " << sql;
			error = true;
		}
	}

	explicit sql_listdata(const QString &_str, QSqlDatabase _connection, TP_SRV tp)
		: sql_tbl(_str, _connection, true)
	{
		setQuery(tp);
		if (!query->prepare(sql)) {
			qDebug() << "Error: SQL Query prepare fail: " << sql;
			error = true;
		}
	}

	explicit sql_listdata(const QString &_str, QSqlDatabase _connection, TP_SRV tp, bool )
		: sql_tbl(_str, _connection, (char) tp)
	{
		setQuery(tp);
		if (!query->prepare(sql)) {
			qDebug() << "Error: SQL Query prepare fail: " << sql;
			error = true;
		}
	}

	~sql_listdata() {}

	QByteArray execute(){
		QJsonArray arr;		 
		int itens_ret = 0;;

		if (error){
			QJsonArray arr2;
			QJsonObject dados;
			dados["status"] = "erro";
			dados["code"] = "400"; 
			arr.append(dados);
		}
		else
			if (executa()){
				if (query->first()) {
					do{
						QSqlRecord localRecord = query->record();
						QJsonArray array;
						QJsonObject d;

						for (int var = 0; var < localRecord.count(); ++var){
							QVariant t = localRecord.field(var).type();
							QString fieldName = localRecord.fieldName(var);

							if (this->distfield == fieldName)
								continue;

							if (t.userType() == QMetaType::QDateTime){
								QString dt = localRecord.value(var).toDateTime().toString(COM_MASK);
								d[fieldName] = dt;
							}
							else{
								d[fieldName] = localRecord.value(var).toString();
							}
							itens_ret++;
						}  			
						arr.append(d);
					} while (query->next());
				}
			}

		QJsonObject dados;
		if (itens_ret){
			dados["body"] = arr;
			dados["status"] = "sucesso";
			dados["code"] = "200";
		}
		else{
			dados["status"] = "erro";
			dados["code"] = "400";
		}

		QJsonDocument doc(dados);
		return doc.toJson();
	}

	QByteArray setPortHttp(const QByteArray &ba){
		QString str = ba.data();
		str.replace(":6524", QString(":%1").arg(ipport));

		if (!iplocal)
			str.replace("10.20.3.36", "189.16.50.195");
		else
			str.replace("189.16.50.195", "10.20.3.36");

		return str.toUtf8();
	}

	QByteArray getTemplate(const QString &fname){
		QByteArray ret; 
		QFile arq(fname);

		if (arq.open(QIODevice::ReadOnly)) {
			ret.append(arq.readAll());
			ret = setPortHttp(ret);
			arq.close();
		}
		return ret;
	}

    QByteArray insertdata()	{ 
		QString token = QUuid::createUuid().toString().toUpper(); 

		for (int i = 0; i < fields.count(); i++){
			QString value = values.at(i);
			if (fields.at(i) == "token")
				value = token;
			value.replace("'", "");
			query->bindValue(QString(":")+fields.at(i), value );
		}

		error = executa() == false; 

		if (error)
			token = "Erro";

		return token.toLatin1();
	}

	QByteArray updatedata()	{
		QString ret = "OK";

		for (int i = 0; i < fields.count(); i++)
		{
			QString value = values.at(i);
			value.replace("'", "");
			query->bindValue(QString(":") + fields.at(i), value);
		}

		error = executa() == false;

		if (error)
			ret = "Erro";

		return ret.toLatin1();
	}

	QByteArray deletedata()	{
		QString ret = "OK";

		for (int i = 0; i < fields.count(); i++){
			QString value = values.at(i);
			value.replace("'", "");
			query->bindValue(QString(":") + fields.at(i), value);
		}

		error = executa() == false;

		if (error)
			ret = "Erro";

		return ret.toLatin1();
	}
	 
	void setInsertTable()
	{
		QString cmd = QString("INSERT INTO %1 (").arg(tablename);
		QString cmd2 = " VALUES (";
		fields.append("token");
		values.append("");

		for (int i = 0; i < fields.count(); i++)
		{ 
			cmd  += fields.at(i); 
			cmd2 += QString(":%1").arg(fields.at(i)); 

			if (i == fields.count() - 1) {
				cmd += ") ";
				cmd2 += "); "; 
			}
			else
			{
				cmd += ", ";
				cmd2 += ", ";
			}
		}
		sql = cmd + cmd2;
	}

	void setUpdateTable()
	{
		QString cmd2 = QString("UPDATE %1 SET ").arg(tablename); 
		QString cmd;

		for (int i = 0; i < fields.count(); i++)
		{  
			if (i == fields.count() - 1) {
				if (cmd.size())
					cmd += ", ";
				cmd += fields.at(i) + " = :" + fields.at(i);
				cmd += " WHERE ";
				cmd += QString("%1").arg(fields.at(0));
				cmd += " = ";
				cmd += QString(":%1").arg(fields.at(0));
			}
			else
			{
				if (cmd.size())
					cmd += ", ";
				cmd += fields.at(i) + " = :" + fields.at(i); 
			}
		}
		sql = cmd2 + cmd;
	}

	void setDeleteTable()
	{
		QString cmd2 = QString("DELETE FROM %1 WHERE ").arg(tablename);
		QString cmd;

		for (int i = 0; i < fields.count(); i++)
		{  
			if (cmd.size())
				cmd += " AND ";
			cmd += fields.at(i) + " = :" + fields.at(i); 
		}
		sql = cmd2 + cmd;
	}

    void setQuery(TP_SRV tp=T_QUERY)
	{

		if (tp == T_INSERT_DATA){
			setInsertTable();
			return;
		}

		if (tp == T_UPDATE_DATA){
			setUpdateTable();
			return;
		}

		if (tp == T_DELETE_DATA){
			setDeleteTable();
			return;
		}

		if (conds.count())
		{
			if (fields.count())
			{
				QString sfields = fields.join(",");
				sql = QString("select %2 from %1 ").arg(tablename).arg( sfields  );
			}
			else
			sql = QString("select * from %1 ").arg(tablename);

			if (conds.count())
			{				
				if ( conds.at(0).toLower().contains("group") == false &&
					conds.at(0).toLower().contains("order") == false )
					sql += "where ";

				// Eliminar o AND...
				for (int i = 0; i < conds.count(); i++) 
					sql += conds.at(i) + " "; 
			}
		}
		else
			if (fields.count())
			{
				QString sfields = fields.join(",");
				sql = QString("select %2 from %1 ").arg(tablename).arg(sfields);
			}
			else
			sql = QString("select * from %1").arg(tablename);

		if (sql.toLower().contains("count") == false && sql.toLower().contains("limit") == false)
		{
			if ( limite > 0 )
			  sql += QString(" limit %1 ").arg(this->limite);
		}
	}

	void setQueryUpdate() 	{
	}

protected:
	QSqlQuery	*qr_update;
	QString		sqlup;
	bool		error = false;
	QString		str_url;
	QJsonObject dados;
	QString		retorno;
	bool		urlerror;

public slots:
	void onError(QNetworkReply::NetworkError )
	{
		qDebug() << "Error calling url " << str_url;
		urlerror = true;
	}
};
//--------------------------------------------------------------------------------------------------

