#pragma once
//----------------------------------------------------------------------------
// cconcurrent.h
//----------------------------------------------------------------------------
// Data        Hora      Desenvolvedor            Observacoes
//----------------------------------------------------------------------------
// 17-08-2018  09:35     Roberto Borges           Definição / Criaçao
//----------------------------------------------------------------------------

#include <QWaitCondition>
#include <QThread>
#include <QMutex>

enum lck_state { STOPPED, READING, UNDEFINED };

//----------------------------------------------------------------------------
// LockBase Class definition
//----------------------------------------------------------------------------
class CConcurrent : public QThread
{
    Q_OBJECT

public:
    CConcurrent()                 { }
   ~CConcurrent()                 { }
    void setLock()                { lock.lock(); }
    void setUnLock()              { lock.unlock(); }
    virtual void run()            { }
    void setState(lck_state st)   { state = st; }
    lck_state getState()          { return state; }

    void waiting(unsigned long value){
        setLock();
        wc.wait(&lock, value);
        setUnLock();
    }

    void done()	{
        isdone = true;
    }

protected:
    QMutex lock;
    bool isdone = false;
    lck_state state = UNDEFINED;
    QWaitCondition wc;

};
