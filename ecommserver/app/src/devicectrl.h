#pragma once 
//--------------------------------------------------------------------------------------------------
// devicectrl.h
// Definicoes do Modulo Redis / Satcompany
//--------------------------------------------------------------------------------------------------
// Autor: Roberto Borges dos Santos
// Data : 08/01/2020 - 15:39
//--------------------------------------------------------------------------------------------------
 
#include <QTimer>
#include <qobject.h> 
#include <QString> 

//--------------------------------------------------------------------------------------------------
// Device Ctrl
// Receive signal from parent class
 

typedef struct _device_ctrl
{ 
	QString id;
	bool state;
	int viagens;

} device_ctrl;