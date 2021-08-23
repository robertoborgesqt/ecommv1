//----------------------------------------------------------------------------
// CIOData 
//----------------------------------------------------------------------------
// Data        Hora      Desenvolvedor            Observacoes                 
//----------------------------------------------------------------------------
// 24-Mai-2006 11:26     Virgilio Padovani       Definição / Criaçao 
//----------------------------------------------------------------------------

#pragma once
//----------------------------------------------------------------------------
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <math.h>
#include <QStringList>

const int i_endian = 1;
#define is_bigendian() ( (*(char*)&i_endian) == 0 )

//----------------------------------------------------------------------------

#ifndef UBYTE
#define UBYTE unsigned char 
#endif

#ifndef USINT
#define USINT unsigned int 
#endif

#ifndef SSHORT
#define SSHORT unsigned short 
#endif

#ifndef LLONG
#define LLONG  qlonglong 
#endif

#ifndef bit
#define bit unsigned int
#endif


//----------------------------------------------------------------------------

struct bit8
{
    bit bt0:1;
    bit bt1:1;
    bit bt2:1;
    bit bt3:1;
    bit bt4:1;
    bit bt5:1;
    bit bt6:1;
    bit bt7:1;
};

struct bit16
{
    bit8 MSB;
    bit8 LSB;
    operator SSHORT()
    {
        SSHORT b = 0x0000;
        b |= MSB.bt0  << 0;
        b |= MSB.bt1  << 1;
        b |= MSB.bt2  << 2;
        b |= MSB.bt3  << 3;
        return b;
    }
};

struct sint 
{
    UBYTE LSB1 : 8;
    UBYTE LSB2 : 8;
    UBYTE MSB1 : 8;
    UBYTE MSB2 : 8;
};

struct slonglong 
{
    UBYTE LSB1 : 8;
    UBYTE LSB2 : 8;
    UBYTE LSB3 : 8;
    UBYTE LSB4 : 8;
    UBYTE MSB1 : 8;
    UBYTE MSB2 : 8;
    UBYTE MSB3 : 8;
    UBYTE MSB4 : 8;
};

using namespace std; 

//----------------------------------------------------------------------------
// Rotinas comuns às classes CHeader, CData, CCrc
//----------------------------------------------------------------------------

class CIOData 
{
public:
    CIOData(){}
    ~CIOData(){}

public:
    QByteArray changeDup( const QByteArray &ba )
    {
        QByteArray ret;
        QString wb = CIOData().toHex(ba);
        ret = wb.replace( "1010", "10" ).toUtf8();
        return CIOData().toBinary(ret);
    }

    QString convertCBAText( char *str, int sz )
    {
        QString hx = CIOData().toHex( QByteArray( str, sz ) ).toUpper();
        hx.remove( "A" );
        return hx;
    }

    int  getInt( UBYTE *ptr, bool inverte=true )
    {
        int ret;
        sint nint;
        if ( inverte )
        {
            nint.LSB1 = ptr[3];
            nint.LSB2 = ptr[2];
            nint.MSB1 = ptr[1];
            nint.MSB2 = ptr[0];
        }
        else
        {
            nint.LSB1 = ptr[0];
            nint.LSB2 = ptr[1];
            nint.MSB1 = ptr[2];
            nint.MSB2 = ptr[3];
        }
        memcpy ( &ret, &nint, sizeof(int));
        return ret;
    }

    int  getInt( const QByteArray &ptr, bool inverte=true )
    {
        return getInt( (UBYTE *) ptr.data(), inverte );
    }

    UBYTE *getIntToChar( int ptr ) const
    {
        UBYTE *ret = new UBYTE[sizeof ptr];
        char *p = (char *) &ptr;
        p+=3;
        for(int i=0; i<4; i++,p-- )
            ret[i] = *p;
        return ret;
    }

    UBYTE *getDoubleToChar( double ptr ) const
    {
        UBYTE *ret = new UBYTE[sizeof ptr];
        char *p = (char *) &ptr;
        p+=7;
        for(int i=0; i<8; i++,p-- )
            ret[i] = *p;
        return ret;
    }

    UBYTE *getUIntToChar( unsigned int ptr ) const
    {
        UBYTE *ret = new UBYTE[sizeof ptr];
        char *p = (char *) &ptr;
        p+=3;
        for(int i=0; i< (int) sizeof(ptr); i++,p-- )
            ret[i] = *p;
        return ret;
    }

    UBYTE *getShortToChar( SSHORT ptr ) const
    {
        UBYTE *ret = new UBYTE[sizeof ptr];
        char *p = (char *) &ptr;
        p+=1;
        for(int i=0; i<2; i++,p-- )
            ret[i] = *p;
        return ret;
    }

    SSHORT getShort( UBYTE *ptr, bool inverte=false )
    {
        SSHORT ret;
        UBYTE str[2];
        if ( !inverte )
        {
            str[0] = ptr[1];
            str[1] = ptr[0];
        }
        else
        {
            str[0] = ptr[0];
            str[1] = ptr[1];
        }
        memcpy ( &ret, &str, 2);
        return ret;
    }

	QString getPosStr(long _pos)
	{
		QString ret = QString().sprintf("%03.5f", (float)_pos / 360000);
		return ret;
	}

    QString getPosStr(USINT _pos)
    {
        QString ret = QString().sprintf("%03.5f", (float)_pos / 360000);
        return ret;
    }

    QString getPosStr(UBYTE *_pos)
    {
        QString ret = QString().sprintf("%03.5f", (float)getInt(_pos) / 360000);
        return ret;
    }


	//	//float pos = *reinterpret_cast<const float*>(_pos);

	//	// 0xFE9DA32E << -23.223506 
	//	// long pos = 0xFE9DA32E * -1;				
	//	// D10 = D + M / 60 + D / 3600;
	//	
	//	double dec = _pos / 3600;   
	//	
	//	long pos = dec; 

	//	double rest;
	//	double degree  = modf( pos  / 360000.00000, &rest );
	//	
	//	int minutes    = modf((rest * 60), &rest);
	//	int seconds    = modf((rest * 3600), &rest);

	//	QString ret    = QString("%1.%2%3").arg(degree).arg(minutes).arg(seconds);
	//	return ret; 
	//	
	//}


    SSHORT getShort( QByteArray ptr, bool inverte=false )
    {
        if ( ptr.size() >= 2 )
            return getShort( (UBYTE *) ptr.data(), inverte );
        else
            return 0;
    }

    short getShortS( UBYTE *ptr, bool inverte=false )
    {
        short ret;
        UBYTE str[2];
        if ( !inverte )
        {
            str[0] = ptr[1];
            str[1] = ptr[0];
        }
        else
        {
            str[0] = ptr[0];
            str[1] = ptr[1];
        }
        memcpy ( &ret, &str, 2);
        return ret;
    }
	  
	int  getLong(const QByteArray &ptr, bool inverte = true)
	{
		return getLong((UBYTE *)ptr.data(), inverte);
	} 

    long getLong( UBYTE *ptr, bool inverte=true )
    {
        long ret;
        sint nint;
        if ( inverte )
        {
            nint.LSB1 = ptr[3];
            nint.LSB2 = ptr[2];
            nint.MSB1 = ptr[1];
            nint.MSB2 = ptr[0];
        }
        else
        {
            nint.LSB1 = ptr[0];
            nint.LSB2 = ptr[1];
            nint.MSB1 = ptr[2];
            nint.MSB2 = ptr[3];
        }
        memcpy ( &ret, &nint, sizeof(long));
        return ret;
    }

    float getFloat( const QByteArray &ptr )
    {
        float ret = *reinterpret_cast<const float*>(ptr.data());
        return (float) ret;
    }

    qlonglong getLongLong( UBYTE *ptr, bool inverte=true )
    {
        qlonglong ret;
        slonglong nint;

        inverte = is_bigendian();

        if ( inverte )
        {
            nint.LSB1 = ptr[7];
            nint.LSB2 = ptr[6];
            nint.LSB3 = ptr[5];
            nint.LSB4 = ptr[4];
            nint.MSB1 = ptr[3];
            nint.MSB2 = ptr[2];
            nint.MSB3 = ptr[1];
            nint.MSB4 = ptr[0];
        }
        else
        {
            nint.LSB1 = ptr[0];
            nint.LSB2 = ptr[1];
            nint.LSB3 = ptr[2];
            nint.LSB4 = ptr[3];
            nint.MSB1 = ptr[4];
            nint.MSB2 = ptr[5];
            nint.MSB3 = ptr[6];
            nint.MSB4 = ptr[7];
        }
        memcpy ( &ret, &nint, sizeof(qlonglong));
        return ret;
    }

    double getDouble( UBYTE *ptr )
    {
        double ret;
        slonglong nint;
        if ( is_bigendian() ) {
            nint.LSB1 = ptr[7];
            nint.LSB2 = ptr[6];
            nint.LSB3 = ptr[5];
            nint.LSB4 = ptr[4];
            nint.MSB1 = ptr[3];
            nint.MSB2 = ptr[2];
            nint.MSB3 = ptr[1];
            nint.MSB4 = ptr[0];
        }
        else {
            nint.LSB1 = ptr[0];
            nint.LSB2 = ptr[1];
            nint.LSB3 = ptr[2];
            nint.LSB4 = ptr[3];
            nint.MSB1 = ptr[4];
            nint.MSB2 = ptr[5];
            nint.MSB3 = ptr[6];
            nint.MSB4 = ptr[7];
        }
        memcpy ( &ret, &nint, sizeof(double));
        return ret;
    }

    double getDouble( UBYTE *ptr, bool invert )
    {
        double ret;
        slonglong nint;
        if ( invert ) {
            nint.LSB1 = ptr[7];
            nint.LSB2 = ptr[6];
            nint.LSB3 = ptr[5];
            nint.LSB4 = ptr[4];
            nint.MSB1 = ptr[3];
            nint.MSB2 = ptr[2];
            nint.MSB3 = ptr[1];
            nint.MSB4 = ptr[0];
        }
        else {
            nint.LSB1 = ptr[0];
            nint.LSB2 = ptr[1];
            nint.LSB3 = ptr[2];
            nint.LSB4 = ptr[3];
            nint.MSB1 = ptr[4];
            nint.MSB2 = ptr[5];
            nint.MSB3 = ptr[6];
            nint.MSB4 = ptr[7];
        }
        memcpy ( &ret, &nint, sizeof(double));
        return ret;
    }

    int unpack(char *buf, double *value)
    {
        union temp {
            double  value;
            char    c[8];
        } in, out;

        memcpy(in.c, buf, 8);

        for( int i =0; i < 8; i++ )
            out.c[i] = in.c[i];

        memcpy(value, &out.value, 8);
        return 8;
    }


    QByteArray getBytes( int valor )
    {
        UBYTE *usn = getIntToChar(valor);
        QByteArray ret( (char *) usn, 4 );
        delete usn;
        return ret;
    }

    QByteArray getBytes( double valor )
    {
        UBYTE *usn = getDoubleToChar(valor);
        QByteArray ret( (char *) usn, 8 );
        delete usn;
        return ret;
    }

    QByteArray getBytes( SSHORT valor, bool inv=false )
    {
        QByteArray ret;
        UBYTE *usn = getShortToChar(valor);
        if ( !inv )
            ret = QByteArray( (char *) usn, 2 );
        else
        {
            ret+= usn[1];
            ret+= usn[0];
        }

        delete usn;
        return ret;
    }

    QByteArray getBytes( unsigned int valor, bool inv=false )
    {
        QByteArray ret;
        UBYTE *usn = getUIntToChar(valor);
        if ( !inv )
            ret = QByteArray( (char *) usn, 4 );
        else
        {
            ret+= usn[3];
            ret+= usn[2];
            ret+= usn[1];
            ret+= usn[0];
        }
        delete usn;
        return ret;
    }

    QByteArray toBinary(const QByteArray &p_ba)
    {
        QByteArray ret;

        QByteArray ba = p_ba.toUpper().trimmed();

        if ( ba.size() %2 )
            return ret;

        for( int i=0; i< ba.size(); i+=2 )
        {
            int a = ba.at(i) >= 'A' ? ba.at(i) - 55 : ba.at(i) - '0';
            int b = ba.at(i+1) >= 'A' ? ba.at(i+1) - 55 : ba.at(i+1) - '0';
            ret += (UBYTE)( a * 16 + b );
        }
        return ret;
    }


	QByteArray toBinary(const QString &p_ba)
	{
		QByteArray ret;

		QByteArray ba = p_ba.toLatin1();

		if (ba.size() % 2)
			return ret;

		for (int i = 0; i < ba.size(); i += 2)
		{
			int a = ba.at(i) >= 'A' ? ba.at(i) - 55 : ba.at(i) - '0';
			int b = ba.at(i + 1) >= 'A' ? ba.at(i + 1) - 55 : ba.at(i + 1) - '0';
			ret += (UBYTE)(a * 16 + b);
		}
		return ret;
	}

    QByteArray ipToInt( const QString &ipadd )
    {
        QStringList lista = QString(ipadd).split(".");
        unsigned char s[32]={0};
        for( int i=0; i< lista.count(); i++ )
            s[i] = (unsigned char) lista[i].toInt();
        return QByteArray( (char *) s, 4 );
    }

    QByteArray toByteArray ( const QString &pStr )
    {
        QByteArray ret;
        for( int i = 0; i < pStr.length(); i+=2 )
            ret += getAscii( pStr.mid(i, 2).toUtf8() );
        return ret;
    }

    UBYTE getAscii ( const QByteArray &ba )
    {
        int a = ba.at(0) >= 'A' ? ba.at(0) - 55 : ba.at(0) - '0';
        int b = ba.at(1) >= 'A' ? ba.at(1) - 55 : ba.at(1) - '0';
        return (UBYTE) ( a * 16 + b );
    }

    int toInt ( const QByteArray &ba )
    {
        int a = ba.at(0) >= 'A' ? ba.at(0) - 55 : ba.at(0) - '0';
        int b = ba.at(1) >= 'A' ? ba.at(1) - 55 : ba.at(1) - '0';
        return  ( a * 16 + b );
    }

    QString toHex( const QByteArray &p_ba )
    {
        QString tmp;
        for( int i=0; i<p_ba.count(); i++ )
        {
            tmp += QString().sprintf( "%02X", (UBYTE) p_ba.at(i) );
        }
        return tmp;
    }

    UBYTE hex2bcd ( UBYTE x )
    {
        return ( (UBYTE)((x / 10) << 4) | ( x % 10 ) );
    }

    QByteArray toBCD(const QString &m_ba)
    {
        QByteArray m_BCD;
        QString ba = m_ba;
        if( ( m_ba.size() %2 ) == 1) {
            ba = "0";
            ba += m_ba ;
        }
        for (int i=1 ; i<ba.size() ; i++)	{
            QByteArray temp;
            temp.append( ba.at(i-1) );
            temp.append( ba.at(i++) );
            UBYTE ch = hex2bcd( (UBYTE) temp.toInt() );
            m_BCD.append(ch);
        }
        return m_BCD;
    }

    QString fromBCD( const QByteArray ba )
    {
        QString sim_str;
        for( int i=0; i < ba.count(); i++ )
        {
            UBYTE ch = (UBYTE) ba.at(i);
            sim_str += QString().sprintf( "%02X", ch );
        }
        return sim_str;
    }

    bool validDegree( float value, int baseval)
    {
        double param, intpart;
        param = value;
        modf (param , &intpart);
        if ( intpart > baseval || intpart < (baseval * -1) )
            return false;
        return true;
    }


    void dump(const QByteArray &/*buffer*/, const QString &/*name*/  )
    {
		return;
		/*
        qDebug() << "DUMP: " << name;
        QString sret = "";
        unsigned char bt;
        int  x = 0;
        int count = 15;
        QString txt;

        QString linha = "00000|";
        char bufdata[1024];

        int sizeData = buffer.size();

        int diff = sizeData % 16;
        int sizeAll = sizeData + (16 - diff);

        for( int sz=0; sz < sizeAll; sz++ ) {
            if ( sz < sizeData ) {
                bt = *buffer.mid(sz).data();
                txt   += isalnum(bt) || ispunct(bt) ||
                        QString("<>{}[] ^~´`").contains(QString("%1").arg(bt)) ? bt : '.' ;
                sprintf( bufdata, "%02X ",  bt );
                linha += bufdata;
            }
            else  {
                txt   += ' ' ;
                linha += "   ";
            }

            if ( x++ >= count )    {
                x = 0 ;
                qDebug() << QString("%1 |%2| ").arg(linha).arg(txt);
                linha = QString().sprintf( "%05d|", sz + 1 );
                txt = "";
            }
        }
        qDebug() << linha;
		*/
    }
};
//----------------------------------------------------------------------------

