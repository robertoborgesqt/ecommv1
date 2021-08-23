#pragma once

#include <qobject.h>
// Redis message

class tpredis : public QObject
{
	Q_OBJECT

public:
	tpredis() {}

	QString id;
	QByteArray data;
};

