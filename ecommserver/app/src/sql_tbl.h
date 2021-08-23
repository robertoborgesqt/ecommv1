#pragma once
//--------------------------------------------------------------------------------------------------
// sql_tbl.h
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
//--------------------------------------------------------------------------------------------------
// Definicao sql_tbl sql - base table
//
//--------------------------------------------------------------------------------------------------

#include <QObject> 
#include <qdebug.h>
#include <string>
#include <iostream>
#include <QSqlDatabase>
#include <QElapsedTimer>
#include <QSqlQuery>
#include <QTimeZone>
#include "cconcurrent.h"

#define LOG_FAILED_QUERY(query) qDebug() << __FILE__ << __LINE__ << "FAILED QUERY [" << (query).executedQuery() << "]" << (query).lastError() 
#define ERR_FAILED_QUERY(query) (query).lastError() 
#define DATE_NOW QString("DATE_SUB(now(), INTERVAL hour(timediff(now(), utc_timestamp())) Hour)")
#define STR(a) QString( "'%1'").arg(a)

using namespace std;

class sql_tbl : public CConcurrent
{ 

public:

	sql_tbl(const QString &_str_p, QSqlDatabase _connection, int _limite)
	{
		QString _str = extractQueue(_str_p);
		limite = _limite;
		QStringList strs = _str.split(" ");
		if (strs.at(0).contains("["))
		{
			QString st0 = strs.at(0).trimmed();
			st0.replace("]", "");
			QStringList st2 = st0.split("[");
			tablename = st2.at(0);
			if (st2.count() > 1)
			{
				fields = st2.at(1).split(",");
			}

		}
		else
			tablename = strs.at(0);

		if (strs.count() > 1)
		{
			for (int i = 1; i < strs.count(); i++)
			{
				QString condw = strs.at(i);
				if (condw.contains("GEODECODE"))
				{
					QStringList geof = condw.split("=");
					if (geof.count() > 1)
					{
						QStringList w_geofields = geof.at(1).split(",");
						if (w_geofields.count() > 1)
						{
							geofields.append(w_geofields.at(0));
							geofields.append(w_geofields.at(1));
						}
					}

				}
				else
					if (condw.contains("GEODIST"))
					{
						QStringList distf = condw.split("=");
						if (distf.count() > 1)
						{
							distfield = distf.at(1);
						}
					}
				else
					conds.append(condw);
			}
		}

		setConnection(_connection);
		getDateDiff();
	}
 
	sql_tbl(const QString &_str_p, QSqlDatabase _connection, int _limite, bool )
	{ 
		QString _str = extractQueue(_str_p);
		limite = _limite;
		QStringList strs = _str.split(" ");

		if (strs.at(0).contains("["))
		{
			QString st0 = strs.at(0).trimmed();
			st0.replace("]", "");
			QStringList st2 = st0.split("[");
			tablename = st2.at(0);

			if (st2.count() > 1)
			{
				QString wstr = st2.at(1).trimmed();
				if (wstr.contains("0xEnc"))
				{
					fields = st2.at(1).split(",");
					for (int x = 0; x < fields.count(); x++)
					{
						QString cmd = fields.at(x).trimmed();
						if (cmd.contains("0xEnc"))
						{
							cmd.remove("0xEnc");
							cmd.replace("^", ",");
							fields[x] = cmd.trimmed();
						}
					}
				}
				else
				fields = st2.at(1).split(",");
			}
		}
		else
			tablename = strs.at(0);

		if (strs.count() > 1)
		{
			for (int i = 1; i < strs.count(); i++)
			{
				QString condw = strs.at(i);
				if (condw.contains("GEODECODE"))
				{
					QStringList geof = condw.split("=");
					if (geof.count() > 1)
					{
						QStringList w_geofields = geof.at(1).split(",");
						if (w_geofields.count() > 1)
						{
							geofields.append(w_geofields.at(0));
							geofields.append(w_geofields.at(1));
						}
					}
				}
				else
				if (condw.contains("GEODIST"))
				{
					QStringList distf = condw.split("=");
					if (distf.count() > 1)
					{   
							distfield = distf.at(1) ; 
					}
				}
				else
					conds.append(condw);
			}
		}
		if (fields.count())
		{
		}
		setConnection(_connection);
		getDateDiff();
	}
 
	sql_tbl(const QString &_str_p, QSqlDatabase _connection, int _limite, char )
	{
		QString _str = extractQueue(_str_p);
		limite = _limite;
		QStringList strs = _str.split(" ");
		if (strs.at(0).contains("["))
		{
			QString st0 = strs.at(0).trimmed();
			st0.replace("]", "");
			QStringList st2 = st0.split("[");
			tablename = st2.at(0);
			if (st2.count() > 1)
			{
				QString wstr = st2.at(1);
				if (wstr.contains("RELAY"))
				{
					fields.append(wstr + "#");
				}
				else
					fields = st2.at(1).split(",");
			}
		}
		else
			tablename = strs.at(0);

		if (strs.count() > 1)
		{
			for (int i = 1; i < strs.count(); i++)
			{
				QString condw = strs.at(i);
				if (condw.contains("GEODECODE"))
				{
					QStringList geof = condw.split("=");
					if (geof.count() > 1)
					{
						QStringList w_geofields = geof.at(1).split(",");
						if (w_geofields.count() > 1)
						{
							geofields.append(w_geofields.at(0));
							geofields.append(w_geofields.at(1));
						}
					}

				}
				else
					conds.append(condw);
			}
		}

		setConnection(_connection);
		getDateDiff();
	}
 
	sql_tbl(const QString &_str_p, QSqlDatabase _connection, char)
	{
		QString _str = extractQueue(_str_p);
		limite = -1;

		if (_str.contains("[") == false)
		{		 
			return;
		}

		QStringList st1 = _str.split("[");

		tablename = st1.at(0);
		QString nfields = st1.at(1);
		nfields.remove(']');

		QStringList strs = nfields.split(",");

		foreach(QString fld, strs)
		{
			if (fld.trimmed().size() && fld.contains("=")) {
				QStringList fls = fld.trimmed().split("=");
				fields.append(fls.at(0));
				values.append(fls.at(1));
			}
		}

		setConnection(_connection);
		getDateDiff();
	}
	
	QSqlDatabase m_connection;

	QString extractQueue(const QString &_path)
	{
		QString ret =_path;
		if (_path.contains("|"))
		{
			ret = ret.split("|").at(0);
			QString queue_path = ret.split("|").at(1);
			QStringList queue_list = queue_path.split(",");
			if (queue_list.count() >= 3)
			{ 
				queue_name = queue_list.at(0);
				host_queue = queue_list.at(1);
				queue_port = queue_list.at(2).toInt();
			}  
		}
		return ret;
	}


	void setConnection(QSqlDatabase _connection)
	{
		m_connection = _connection;
		if (query) {
			delete query;
			query = nullptr;
		}
		connName = _connection.connectionName();
		query = new QSqlQuery(_connection); // create instance with connection		
	}

	~sql_tbl() {
		delete query;
	}

	bool executa(int *reti)
	{
		initTimer();
		qDebug() << "Exec SQL :" << sql;

		if (!query->exec() || !query->numRowsAffected())
		{
			LOG_FAILED_QUERY(*query);

			if (ERR_FAILED_QUERY(*query).type() == QSqlError::ConnectionError ||
				query->lastError().nativeErrorCode() == "2013")  // Lost connection 
				errorCode = QSqlError::ConnectionError;

			if (query->lastError().nativeErrorCode() == "1062") {
				elapsed();
				*reti = -1;
				return true;
			}
			
			elapsed();
			return false;
		}
		elapsed();
		QVariant ret = query->lastInsertId();
		*reti = ret.toInt();
		return true;
	}
	
	bool executa()
	{
		initTimer();
		qDebug() << "Exec SQL :" << sql;

		if (!query->exec() || !query->numRowsAffected())
		{
			LOG_FAILED_QUERY(*query);

			if (ERR_FAILED_QUERY(*query).type() == QSqlError::ConnectionError ||
				query->lastError().nativeErrorCode() == "2013")  // Lost connection 
				errorCode = QSqlError::ConnectionError;

			if (query->lastError().nativeErrorCode() == "1062") {
				elapsed();
				return true;
			}

			elapsed();
			return false;
		}
		elapsed();
		QString executed = query->executedQuery();
		qDebug() << "last = " << executed; 
		return true;
	}

	QString getDateDiff()
	{
		QList<QByteArray> ids = QTimeZone::availableTimeZoneIds();
		QTimeZone zone = QTimeZone("America/Sao_Paulo");
		QDateTime zoneTime = QDateTime(QDate::currentDate(), QTime::currentTime(), zone).toLocalTime();

		// Sinc pool.ntp.br
		/*
		qDebug() << " ";
		qDebug() << " ";
		qDebug() << "------------------------------------------------------";
		qDebug() << "Pais    : " << QLocale::countryToString(zone.country());
		qDebug() << "Hr Verao: " << (zone.isDaylightTime(QDateTime::currentDateTime()) ? "Sim" : "Nao");
		qDebug() << "Z Date  : " << zoneTime.date();
		qDebug() << "Z Hora  : " << zoneTime.time();
		qDebug() << "Cidade  : " << "America/Sao_Paulo";
		qDebug() << "UTC Off : " << getDifft() / 3600;
		qDebug() << "------------------------------------------------------";
		qDebug() << " ";
		qDebug() << " ";

		QTimeZone::OffsetData offset = zone.nextTransition(QDateTime::currentDateTime());

		if (offset.atUtc != QDateTime())
			qDebug() << "Next transition: " << offset.atUtc.toString();
		else
			qDebug() << "Next transition: none";*/

		return QString();
	}

	QStringList geofields;
	QString distfield;

	QString queue_name, host_queue;
	int queue_port;

	int getDifft() {
		QDateTime local(QDateTime::currentDateTime());
		QDateTime UTC(local);
		UTC.setTimeSpec(Qt::UTC);
		return local.secsTo(UTC);
	}

	int errorCode = -1;

	QString getConnectionName() {}

	void  initTimer() { et.start(); }
	void  elapsed() { qDebug() << QString().sprintf("Access DB = %.3fsec", et.elapsed() / 1000.000).toLatin1(); }

protected:
	int         difft;
	int         limite;
	QElapsedTimer et;
	QSqlQuery * query = nullptr;
	QString     sql;
	QString     tablename;
	QString     connName;
	QStringList conds, fields, values;
	QString     getLastExecutedQuery(const QSqlQuery& query) { QString str = query.lastQuery(); QMapIterator<QString, QVariant> it(query.boundValues()); while (it.hasNext()) { it.next(); str.replace(it.key(), it.value().toString()); } return str; }

};

//--------------------------------------------------------------------------------------------------

