#pragma once 
//--------------------------------------------------------------------------------------------------
// cqueue_redis.h
// Definicoes do protocolo do modulo r12rf / Satcompany
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 05/11/2019 - 13:49
//--------------------------------------------------------------------------------------------------

#include <qqueue.h>
#include <QTimer> 
#include <qcoreapplication.h>
#include <qobject.h> 
#include "cconcurrent.h"
#include <qmutex.h>
#include <qwaitcondition.h>
#include "redisclient.h"
#include "devicectrl.h"
#include "tpredis.h"
#include "cjson.h"


#define MASK_MS "yyyy-MM-dd hh:mm:ss:zzz"
#define MASK_DB "yyyy-MM-dd hh:mm:ss"
#define MASK_DT "yyyy-MM-ddThh:mm:ss"
#define MASK_TS "yyyyMMddhhmmss"
#define MASK_SH "dd-MM.hh:mm:ss"
#define MASK_LG "dd-MM.hh:mm:ss.zzz"
#define MASK_TM "hh:mm:ss"

//--------------------------------------------------------------------------------------------------
// cqueue_redis class definition 
// Receive signal from parent class

class cqueue_redis : public CConcurrent
{
	Q_OBJECT

public:
	cqueue_redis(CConcurrent *_parent, const QString & _addr, int _port)
	{
		port = _port;
		addr = _addr;
		parent = _parent;
	}

	void connectRedis()
	{
		qDebug() << " Connecting redis..";
		client = new RedisClient((QObject *)this); 
		connect(&tm, SIGNAL(timeout()), this, SLOT(findList()));

		tmtimeout.setInterval(30000);
		tmtimeout.setSingleShot(true);
		connect(&tmtimeout, SIGNAL(timeout()), this, SLOT(onTimeout()));

		connect(client, SIGNAL(connected()), this, SLOT(onConnected()));
		connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
		connect(client, SIGNAL(messageReceived(QString, QVariant)),
			this, SLOT(onMessageReceived(QString, QVariant)));

		client->connectToServer(addr, port);
	}

	void resetConnection()
	{
		qDebug() << " Reset connection .."; 
		disconnect(client, SIGNAL(connected()), this, SLOT(onConnected()));
		disconnect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
		disconnect(client, SIGNAL(messageReceived(QString, QVariant)),
			this, SLOT(onMessageReceived(QString, QVariant)));

		client->disconnect();
		client->deleteLater();
		client = nullptr;
		connectRedis();
	}

	~cqueue_redis()
	{ 
		client->disconnectFromServer();
		client->deleteLater();
	}


	void run()
	{
		while (!isdone)
		{
		}
	}


public:
	bool redis_status = false;


public slots:

	// List device timeout 
	void onTimeout()
	{ 
		emit notconnected();
		qDebug() << "Timeout finding devices.";
	}

	void onSendcmd(tpredis *tpr)
	{
		QByteArray dados = tpr->data;
		QString key = tpr->id;
		Command *cmdKey = Command::LPUSH(key, QStringList() << dados.data());
		client->sendCommand(cmdKey);
		cmdKey->deleteLater();
		Command *cmdExp = Command::EXPIRE(key, 60 * 60);
		client->sendCommand(cmdExp);
		cmdExp->deleteLater();
		delete tpr;
		tpr = nullptr;
	}

	void onConnected()
	{
		qDebug() << "Connected at Redis/Server";
		redis_status = true;
		emit isconnected(); 
	}
		 
	QByteArray getViagens(const QString _key)
	{
		bufferList.clear(); 
		QByteArray ba; 
		QStringList keys;
		QList<QByteArray>listret;

		if (_key.contains("*"))
		{
			getKey(_key);
			if (bufferList.count())
			{
				keys.append(bufferList);
			}
			else
			{
				return ("pattern not found");
			}
		}
		else
			keys.append(_key);

		// busca trackings

		int a=0;
		int b=9;

		QStringList alltrck;

		for (int x = 0; x < keys.count(); x++)
		{
			QString wkey = keys.at(x).split(":").at(0);
			forever
			{
				QString key = QString("%1:tracking %2-%3").arg(wkey).arg(a).arg(b);
				QStringList list = getArray2( key );
				if (!list.count())
					break;

				alltrck.append(list);

				a += 10;
				b += 10;
			}

				if (!alltrck.count())
				{
					QByteArray rba = (QString("Não há viagens para ") + _key ).toUtf8();
					listret.append(rba);
				}

			QByteArray retarray = getJsonViagem(wkey, alltrck);
			listret.append(retarray);
		}
		return listret.join("\n");
	}

	QByteArray getGeoDist(const QString _key)
	{
		bufferList.clear();
		QByteArray ba;

		// busca trackings

		int a = 0;
		int b = 9;

		QStringList alltrck;

		forever
		{
			QString key = QString("%1:tracking %2-%3").arg(_key).arg(a).arg(b);
			QStringList list = getArray2(key);
			if (!list.count())
				break;

			alltrck.append(list);

			a += 10;
			b += 10;
		}

			if (!alltrck.count())
				return (QString("Não há viagens para ") + _key).toUtf8();

		return getGeoViagem(_key, alltrck);
	}

	QByteArray eval(const QString _str)
	{
		bufferList.clear();
		qDebug() << "Finding..";
		Command *cmd = Command::EVAL(_str);
		connect(cmd, SIGNAL(replyReceived(Reply)), this, SLOT(onReplay(Reply)));

		QEventLoop loop;
		connect(client, SIGNAL(commandFinished()), &loop, SLOT(quit()));
		connect(this, SIGNAL(commandError()), &loop, SLOT(quit()));
		client->sendCommand(cmd);
		tmtimeout.start();
		return toJson();
	}

	QByteArray getKey(const QString _key)
	{ 
		bufferList.clear();
		qDebug() << "Finding..";
		Command *cmd = Command::KEYS(_key);
		connect(cmd, SIGNAL(replyReceived(Reply)), this, SLOT(onReplay(Reply)));

		QEventLoop loop;
		connect(client, SIGNAL(commandFinished()), &loop, SLOT(quit()));
		connect(this, SIGNAL(commandError()), &loop, SLOT(quit()));
		client->sendCommand(cmd);
		tmtimeout.start();
		return toJson();
	}

	QByteArray getGet(const QString _key)
	{ 
		bufferList.clear();
		qDebug() << "Finding..";
		Command *cmd = Command::GET(_key);
		connect(cmd, SIGNAL(replyReceived(Reply)), this, SLOT(onReplay(Reply)));

		QEventLoop loop;
		connect(client, SIGNAL(commandFinished()), &loop, SLOT(quit()));
		connect(this, SIGNAL(commandError()), &loop, SLOT(quit()));
		client->sendCommand(cmd);
		tmtimeout.start();
		QString ret = bufferList.join('\n');
		return ret.toLatin1();
	}

	QByteArray getRange(const QString _key)
	{
		bufferList.clear();
		qDebug() << "Range " << _key;
		QString str = moveWhere(_key);
		str = moveVars(str);
		QString key, rangea, rangeb;

		rangea = "0";
		rangeb = "999999";
		// aaaa+sss-cccc

		key = str.split(" ").at(0);

		if (str.split(" ").count() && str.contains("-"))
		{
			rangea = str.split(" ").at(1).split("-").at(0);
			rangeb = str.split(" ").at(1).split("-").at(1);
		}

		Command *cmd = Command::LRANGE(key, QStringList() << rangea << rangeb);
		connect(cmd, SIGNAL(replyReceived(Reply)), this, SLOT(onReplay(Reply)));
		QEventLoop loop;
		connect(client, SIGNAL(commandFinished()), &loop, SLOT(quit()));
		connect(this, SIGNAL(commandError()), &loop, SLOT(quit()));
		client->sendCommand(cmd);
		tmtimeout.start();
		QJsonArray arr = toArray();
		QJsonObject obj;
		obj["RETORNO"] = arr;
		QJsonDocument doc(arr);
		QByteArray ba = doc.toJson();
		return ba;
	}

	QJsonArray getArray(const QString _key)
	{
		bufferList.clear();
		qDebug() << "Range " << _key;
		QString str = moveWhere(_key);
		str = moveVars(str);
		QString key, rangea, rangeb;

		rangea = "0";
		rangeb = "999999";
		// aaaa+sss-cccc

		key = str.split(" ").at(0);

		if (str.split(" ").count() && str.contains("-"))
		{
			rangea = str.split(" ").at(1).split("-").at(0);
			rangeb = str.split(" ").at(1).split("-").at(1);
		}

		Command *cmd = Command::LRANGE(key, QStringList() << rangea << rangeb);
		connect(cmd, SIGNAL(replyReceived(Reply)), this, SLOT(onReplay(Reply)));
		QEventLoop loop;
		connect(client, SIGNAL(commandFinished()), &loop, SLOT(quit()));
		connect(this, SIGNAL(commandError()), &loop, SLOT(quit()));
		client->sendCommand(cmd);
		qApp->processEvents();
		tmtimeout.start();
		return toArray();
	}
	
	QStringList getArray2(const QString _key )
	{
		bufferList.clear();
		qDebug() << "Range " << _key;
		QString str = moveWhere(_key);
		        str = moveVars(str);
		QString key, rangea, rangeb;

		rangea = "0";
		rangeb = "999999";
		// aaaa+sss-cccc

		key = str.split(" ").at(0);

		if (str.split(" ").count() && str.contains("-"))
		{
			rangea = str.split(" ").at(1).split("-").at(0);
			rangeb = str.split(" ").at(1).split("-").at(1);
		}

		Command *cmd = Command::LRANGE(key, QStringList() << rangea << rangeb);
		connect(cmd, SIGNAL(replyReceived(Reply)), this, SLOT(onReplay(Reply)));
		QEventLoop loop;
		connect(client, SIGNAL(commandFinished()), &loop, SLOT(quit()));
		connect(this, SIGNAL(commandError()), &loop, SLOT(quit()));
		client->sendCommand(cmd);
		qApp->processEvents();
		tmtimeout.start();


		if (bufferList.size() == 0 )
			return bufferList;

		if (_key.contains(":") && !bufferList.at(0).contains('{') )
		{
			bufferList.clear();
			return bufferList;
		}

		QString ret = bufferList.join('\n');
		QStringList b = ret.split("}\n");
		QStringList c;

		for (int i = 0; i < b.count(); i++)
		{
			QString str = b.at(i);
			if (str.size()) {
				str += "}";
				c.append(str);
			}
		}
		return c;
	}

	QString moveVars(const QString & _str)
	{
		if (_str.contains("["))
		{
			QString str = _str.split("[").at(1);
			str  = str.split("]").at(0);
			vars = str.split(","); 
			return _str.split("[").at(0);
		}
		else
			return _str;
	}

	QString moveWhere(const QString & _str)
	{
		if (_str.contains("("))
		{
			QString str = _str.split("(").at(1);
			str = str.split(")").at(0);
			where = str.split(",");
			return _str.split("(").at(0);
		}
		else
			return _str;
	}
	
	QJsonArray toArray()
	{
		QJsonArray array;
		QJsonObject d;
		//int ct = 0;
		 
		QString ret = bufferList.join('\n');
		QStringList b = ret.split("}\n");
		 
		for (int i = 0; i < b.count(); ++i) 
		{  
			QString str = b.at(i) + "}";
			QJsonDocument ac = QJsonDocument::fromJson(str.toLatin1());
			if (ac.isNull())
				break;
		//	if ( chkCondition(ac.object()) )
				array.append(ac.object());
		}

		return array;
	}

	bool chkCondition(const QJsonObject &obj)
	{
		bool ret = true;
		for (int i = 0; i < where.count(); i++)
		{
			QString field = where.at(i).split("=").at(0);
			QString value = where.at(i).split("=").at(1);
			ret &= (obj[field] == value); 
		}
		return ret;
	}
	
	QByteArray toJson()
	{ 
		QJsonArray array;
		QJsonObject d;
		//int ct = 0;

		for (int var = 0; var < bufferList.count(); ++var)	{
			
			QString fieldName = "Key";
			d[fieldName] = bufferList.at(var);
			array.append(d);
		}

		QJsonObject obj;
		obj["RETORNO"] = array;
		QJsonDocument doc(array);
		QByteArray ba = doc.toJson();
		return ba;  
	}

	void onReplay(Reply r)
	{ 
		QStringList lista;

		if (r.type() == Reply::Error)
		{
			qInfo() << "Replay error..";
			resetConnection();
			emit commandError();
			return;
		}

		if (r.type() == Reply::BulkString)
		{
			lista.append(r.value().toString());
		}
		else
			 lista = r.value().toStringList();

		if (lista.count() == 0)
		{
			qWarning() <<  "No devices recv ";
			resetConnection();
			emit commandError();
		}

		//bool added = false;
		qInfo() << "adding " << lista.count() << " devices" ;

		for (int i = 0; i < lista.count(); i++)
		{ 
			QString item = lista.at(i);
			if (vars.count())
			{
				if (item.contains(":"))
				{
					for (int x = 0; x < vars.count(); x++)
					{
						if (item.contains(vars.at(x) ) )
							bufferList.append(item);
					}
				}
				else
				{
					if (item == "}")
					{
						bufferList[bufferList.count() - 1].remove(',');
					}
					bufferList.append(item);
				}
			}
			else
			bufferList.append(lista.at(i));
		} 

		emit commandFinished(); 
	}

	void onDisconnected()
	{ 
		qDebug() << "Disconnected from Redis/Server";
		redis_status = false;
	}

	void onMessageReceived(QString str, QVariant val)
	{
		qDebug() << "onMessageReceived " << str;
	}

	void onSetViagem(device_ctrl *ctr)
	{
		qDebug() << "Result " << ctr->id << " viagens = " << ctr->viagens;
		ctr->state = true;  
		it++;
		if (it != ids.constEnd())
		{
			device_ctrl *ctx = it.value();
			emit deviceRecv(ctx);
		}
	}

	void onInitSearch()
	{  
		if (index)
			it++;
		else
		{
			it = ids.begin();
			index++;
		}

		if (it != ids.constEnd())
		{
			qDebug() << "Init Search.." << it.key();
			emit deviceRecv(it.value());
		}
		else 
			index = 0; 
	}

	void saveToFile(const QString &m_key, const QStringList &arr)
	{
		QStringList lista = arr;
		QJsonObject viagem;
		QJsonArray trajeto;
		bool viagem_aberta = false;

		QString data_fim, lat_fim, lon_fim;
		QString data_ini, lat_ini, lon_ini;

		for (int i = 0; i < lista.count(); i++)
		{
			QString str = lista.at(i);

			str.remove("{");
			str.remove("}");

			str = QString("{%1}").arg(str);

			//qDebug() << "json = " << str;

			if (str.contains("timestamp") == false)
				break;

			QJsonDocument  itemDoc = QJsonDocument::fromJson(str.toLatin1());
			QJsonObject rootObject = itemDoc.object();
			QJsonValue ignicao = rootObject.value("ignicao");

			if (ignicao.toInt() == 1 && data_fim.size()) // chave ligada e tem fim de viagem
			{
				if (viagem_aberta == false) {
					viagem_aberta = true;
					trajeto = QJsonArray();
				}

				QJsonObject  itemtr;
				itemtr["timestamp"] = rootObject.value("timestamp");
				itemtr["modo"] = rootObject.value("modo");
				itemtr["latitude"] = rootObject.value("latitude");
				itemtr["longitude"] = rootObject.value("longitude");

				// Guarda ultimo como inicio de viagem...
				data_ini = itemtr["timestamp"].toString();
				lat_ini = itemtr["latitude"].toString();
				lon_ini = itemtr["longitude"].toString();

				trajeto.append(itemtr);
			}
			else
				if (ignicao.toInt() == 0 && viagem_aberta)
				{ 
					if (trajeto.count())
					{
						QString w_key = m_key;

						viagem["id_device"] = w_key.remove(QString(":tracking"), Qt::CaseSensitive);

						viagem["latitude_fim"] = lat_fim;
						viagem["longitude_fim"] = lon_fim;
						viagem["Fim"] = data_fim;

						viagem["latitude_inicio"] = lat_ini;
						viagem["longitude_inicio"] = lon_ini;
						viagem["Inicio"] = data_ini;


						QDateTime in = QDateTime().fromString(viagem["Fim"].toString(), MASK_DB);
						QDateTime ou = QDateTime().fromString(viagem["Inicio"].toString(), MASK_DB);
						viagem["Trajeto"] = trajeto;
						QJsonObject obj;
						obj["RETORNO"] = viagem;
						QJsonDocument doc(obj);
						QByteArray ba = doc.toJson();
						QString wdate = in.toString(MASK_TS);
						QString strt = QString("E:\\telemetria\\%1-%2.json").arg(m_key).arg(wdate);
						strt.remove(":tracking");
						QFile file(strt);

						if (file.open(QIODevice::WriteOnly))
						{
							file.write(ba, ba.size());
							file.close();
							filelist.append(strt);
						}
						else
							qDebug() << "Nao foi possivel gravar arquivo " << strt;

						viagem_aberta = false;
						data_fim.clear();
					}
					else
					{
						viagem_aberta = false;
						data_fim.clear();
					}

				}
				else
					if (ignicao.toInt() == 0)
					{
						lat_fim = rootObject.value("latitude").toString();
						lon_fim = rootObject.value("longitude").toString();;
						data_fim = rootObject.value("timestamp").toString();
					}

		}
	}

	QByteArray getJsonViagem(const QString &m_key, const QStringList &_list)
	{
		QStringList lista = _list;
		QJsonObject viagem;
		QJsonArray trajeto;
		QJsonArray arr;


		bool viagem_aberta = false;

		QString data_fim, lat_fim, lon_fim;
		QString data_ini, lat_ini, lon_ini;

		for (int i = 0; i < lista.count(); i++)
		{
			QString str = lista.at(i);

			str.remove("{");
			str.remove("}");

			str = QString("{%1}").arg(str);
			 
			if (str.contains("timestamp") == false)
				break;

			QJsonDocument  itemDoc = QJsonDocument::fromJson(str.toLatin1());
			QJsonObject rootObject = itemDoc.object();
			QJsonValue ignicao = rootObject.value("ignicao");

			if (ignicao.toInt() == 1 && data_fim.size()) // chave ligada e tem fim de viagem
			{
				if (viagem_aberta == false) {
					viagem_aberta = true;
					trajeto = QJsonArray();
				}

				QJsonObject  itemtr;
				itemtr["timestamp"] = rootObject.value("timestamp");
				itemtr["modo"] = rootObject.value("modo");
				itemtr["latitude"] = rootObject.value("latitude");
				itemtr["longitude"] = rootObject.value("longitude");

				// Guarda ultimo como inicio de viagem...
				data_ini = itemtr["timestamp"].toString();
				lat_ini = itemtr["latitude"].toString();
				lon_ini = itemtr["longitude"].toString();

				trajeto.append(itemtr);
			}
			else
				if (ignicao.toInt() == 0 && viagem_aberta)
				{
					if (trajeto.count())
					{
						QString w_key = m_key;

						viagem["id_device"] = w_key.remove(QString(":tracking"), Qt::CaseSensitive);

						viagem["latitude_fim"] = lat_fim;
						viagem["longitude_fim"] = lon_fim;
						viagem["Fim"] = data_fim;

						viagem["latitude_inicio"] = lat_ini;
						viagem["longitude_inicio"] = lon_ini;
						viagem["Inicio"] = data_ini; 

						QDateTime in = QDateTime().fromString(viagem["Fim"].toString(), MASK_DB);
						QDateTime ou = QDateTime().fromString(viagem["Inicio"].toString(), MASK_DB);
						viagem["Trajeto"] = trajeto;  
						arr.append(viagem); 

						viagem_aberta = false;
						data_fim.clear();
					}
					else
					{
						viagem_aberta = false;
						data_fim.clear();
					} 
				}
				else
					if (ignicao.toInt() == 0)
					{
						lat_fim = rootObject.value("latitude").toString();
						lon_fim = rootObject.value("longitude").toString();;
						data_fim = rootObject.value("timestamp").toString();
					}

		}

		if (arr.count() == 0)
		{
			viagem["id_device"] = m_key;
			viagem["status"] = "Sem viagens"; 
			arr.append(viagem);
		}

		QJsonObject obj;
		obj["RETORNO"] = arr;
		QJsonDocument doc(arr);
		QByteArray ba = doc.toJson();
		return ba;
	}

	QByteArray getGeoViagem(const QString &m_key, const QStringList &_list)
	{
		QStringList lista = _list;
		QJsonObject viagem;
		QStringList posicoes;
		QStringList datas;
		QJsonArray arr;

		bool viagem_aberta = false;

		QString data_fim, pos_fim;
		QString data_ini, pos_ini;

		for (int i = 0; i < lista.count(); i++)
		{
			QString str = lista.at(i);

			str.remove("{");
			str.remove("}");

			str = QString("{%1}").arg(str);
			
			if (str.contains("timestamp") == false)
				break;

			QJsonDocument  itemDoc = QJsonDocument::fromJson(str.toLatin1());
			QJsonObject rootObject = itemDoc.object();
			QJsonValue ignicao = rootObject.value("ignicao");

			QString latitude  = rootObject.value("latitude").toString();
			QString longitude = rootObject.value("longitude").toString();
			QString timestamp = rootObject.value("timestamp").toString();

			QString posicao   = longitude + "," + latitude;
			QString dataw     = timestamp;

			if (ignicao.toInt() == 1 && data_fim.size()) // chave ligada e tem fim de viagem
			{
				if (viagem_aberta == false) {
					viagem_aberta = true;
					posicoes.clear();
					datas.clear();
				} 

				// Guarda ultimo como inicio de viagem...
				data_ini = dataw;
				pos_ini  = posicao;  
				posicoes.append(posicao);
				datas.append(dataw);
			}
			else
				if (ignicao.toInt() == 0 && viagem_aberta)
				{
					if (posicoes.count())
					{
						QString w_key = m_key;

						viagem["id_device"] = w_key.remove(QString(":tracking"), Qt::CaseSensitive);

						viagem["posicao_final"]   = pos_fim;
						viagem["posicao_inicial"] = pos_ini;
						viagem["Fim"]    = data_fim; 
						viagem["Inicio"] = data_ini;

						QDateTime in = QDateTime().fromString(viagem["Fim"].toString(), MASK_DB);
						QDateTime ou = QDateTime().fromString(viagem["Inicio"].toString(), MASK_DB);
						viagem["posicoes"]   = posicoes.join(";");
						viagem["timestamps"] = datas.join(";");

						QJsonObject ret = calcDistances(posicoes.join(";"));
						viagem["distancias"] = ret;
						arr.append(viagem); 
						viagem_aberta = false;
						data_fim.clear();
					}
					else
					{
						viagem_aberta = false;
						data_fim.clear();
					}
				}
				else
					if (ignicao.toInt() == 0)
					{
						pos_fim  = posicao ;
						data_fim = dataw ;
					} 
		}

		if (arr.count() == 0)
		{
			viagem["id_device"] = m_key;
			viagem["status"] = "Sem viagens";
			arr.append(viagem);
		}

		QJsonObject obj;
		obj["RETORNO"] = arr;
		QJsonDocument doc(arr);
		QByteArray ba = doc.toJson();
		return ba;
	}

	//QJsonObject jdados;
	QJsonObject calcDistances(const QString &dists)
	{ 
		QByteArray ret;
		QJsonObject d;
		int ct = 0;

	    if (dists.size() > 0) {
	    	QString fld = dists;
	    	QByteArray retc = httpReqsend2(fld);
	    
	    	if (retc.contains("Erro") || retc.contains( "NOK") )
	    		return d;
	    
	    	if (retc.size())
	    	{
	    		QJsonDocument ac = QJsonDocument::fromJson(retc);
	    
	    		if (ac["DISTANCIA"].toVariant().toDouble() < 50)
					return d;
	    
	    		QJsonObject json_obj = ac.object();
	    		double distt = d["distancia"].toDouble() + ac["DISTANCIA"].toDouble();
	    		QString vdsr = QString().sprintf("%.2f", distt);
	    		d["distancia"] = vdsr;
	    		QString resw = d["distancia"].toString();
	    		d["counter"] = ct; 
	    	}
	    }
		return d;
	}

	QString getjson()
	{
		QJsonObject jdados;
		QJsonDocument doc(jdados);
		return doc.toJson(QJsonDocument::Indented);
	}

	QByteArray httpReqsend2(const QString & _str)
	{
		QString stw = _str.trimmed();
		int idx = stw.size() - 1;

		if (stw.at(idx) == ";") 
			stw.remove(idx, 1); 

		QString str_url;

		str_url = QString("https://maps.satcompany.app/match/v1/coordenadas/%1").arg(stw);

		QByteArray jsonString = getjson().toLatin1();
		QByteArray postDataSize = QByteArray::number(jsonString.size());

		QUrl req(str_url);

		//if (key.size())
		//	req.setQuery(QString("Key=%1").arg(key));

		QNetworkRequest request(req);

		QNetworkAccessManager test;
		bool urlerror = false;
		QEventLoop loop;
		connect(&test, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
		QNetworkReply * reply = test.post(request, jsonString);
		QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
		loop.exec();

		if (urlerror)
			return QByteArray();

		QByteArray response = reply->readAll();
		qDebug() << "********* Retorno = " << response;

		return response;
	}


signals:
	void errorMsg(const QString &);
	void connectionError(const QString &);
	void errorConnection(); 
	void deviceRecv(device_ctrl *);  
	void isconnected();
	void notconnected();
	void commandFinished();
	void commandError();

public: 
	QStringList vars;
	QStringList where;
	QMap<QString, device_ctrl *> ids;
	QMap<QString, device_ctrl *> ::iterator it; 
	RedisClient *client;
	quint16 port = 6379;
	QObject *parent; 
	QTimer tm;
	QTimer tmtimeout;
	QString addr;
	int index = 0;
	QStringList bufferList;
	QStringList filelist;

};
//--------------------------------------------------------------------------------------------------

