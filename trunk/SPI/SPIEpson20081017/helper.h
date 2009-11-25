#ifndef _HELPER_SPI_
#define _HELPER_SPI_

#include <afxmt.h>
#include "fundef.h"

class CAutoLock {

    // make copy constructor and assignment operator inaccessible

    CAutoLock(const CAutoLock &refAutoLock);
    CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
    CCriticalSection * m_pLock;

public:
    CAutoLock(CCriticalSection * plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    };

    ~CAutoLock() {
        m_pLock->Unlock();
    };
};



extern pfnWFMAllocateBuffer m_pfnWFMAllocateBuffer;
extern pfnWFMAllocateMore   m_pfnWFMAllocateMore;
extern pfnWFMFreeBuffer m_pfnWFMFreeBuffe;
extern pfnWFMGetTraceLevel m_pfnWFMGetTraceLevel;
extern pfnWFMKillTimer m_pfnWFMKillTimer;
extern pfnWFMOutputTraceData m_pfnWFMOutputTraceData;
extern pfnWFMReleaseDLL m_pfnWFMReleaseDLL;
extern pfnWFMSetTimer m_pfnWFMSetTimer;


#endif //_HELPER_SPI_
