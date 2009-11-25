#ifndef _HELPER_SPI_
#define _HELPER_SPI_

#include <afxmt.h>

class CAutoLock {

    // make copy constructor and assignment operator inaccessible

    CAutoLock(const CAutoLock &refAutoLock);
    CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
    CCriticalSection * m_pLock;
	BOOL m_bAlreadyUnlock;

public:
    CAutoLock(CCriticalSection * plock)
    {
        m_pLock = plock;
		m_bAlreadyUnlock = FALSE;
        m_pLock->Lock();
    };

	Unlock()
	{
		m_pLock->Unlock();
		m_bAlreadyUnlock = TRUE;
	}

    ~CAutoLock() {
        if(!m_bAlreadyUnlock) m_pLock->Unlock();
    };
};

#endif //_HELPER_SPI_
