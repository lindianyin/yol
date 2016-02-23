
#include "CorePrivate.h"
#include <stdlib.h>
#include <algorithm>
#include "Core/QMemory.h"
#include "Network/QSocket.h"
#include "Network/QSocketPack.h"
#include "ProtocolBasic.h"

using namespace std;

int QSocket_Packer::Send(        
    ISocketStream *piSocketStream, 
    unsigned uBufferSize, const unsigned char cbyBuffer[]
)
{
	//QLogPrintf(LOG_INFO,"..QSocket_Packer::Send %d",uBufferSize);
    int nResult  = false;
    int nRetCode = false;
    HRESULT hrRetCode = E_FAIL;
    int nAllocFlag  = false;
	IMemBlock *piBuffer = NULL;

    LOG_PROCESS_ERROR(piSocketStream);
    LOG_PROCESS_ERROR(uBufferSize > 0);
    LOG_PROCESS_ERROR(cbyBuffer);

    if (!m_piBuffer)
    {
        m_piBuffer = QCreateMemBlock(m_uPackSize);
        PROCESS_ERROR(m_piBuffer);

        nAllocFlag = true;

        hrRetCode = m_piBuffer->QueryInterface(IID_IMemBlock_Resizable, (void **)&m_piBufferReSize);
        COM_PROCESS_ERROR(hrRetCode);

        m_pbyBufferBegin = (unsigned char *)m_piBuffer->GetData();
        m_pbyBufferPos   = m_pbyBufferBegin;
        m_pbyBufferEnd   = m_pbyBufferBegin + m_piBuffer->GetSize();
    }

    if ((m_pbyBufferPos + uBufferSize) > m_pbyBufferEnd)
    {
        // if m_piBuffer remain size not enough to hold cbyBuffer
        nRetCode = FlushSend(piSocketStream);
        LOG_PROCESS_ERROR(nRetCode);

        // if m_uPackSize is less than uBufferSize
		ASSERT(m_pbyBufferPos == m_pbyBufferBegin);
		if ((m_pbyBufferBegin + uBufferSize) > m_pbyBufferEnd)
		{
			void *pvData = NULL;

			piBuffer = QCreateMemBlock(uBufferSize);
			LOG_PROCESS_ERROR(piBuffer);

			pvData = piBuffer->GetData();
			LOG_PROCESS_ERROR(pvData);

			memcpy(pvData, cbyBuffer, uBufferSize);
			nRetCode = piSocketStream->Send(piBuffer);
			LOG_PROCESS_ERROR(nRetCode == 1);

			SAFE_RELEASE(piBuffer);

			goto EXIT1;
		}
    }	

    memcpy(m_pbyBufferPos, cbyBuffer, uBufferSize);
    m_pbyBufferPos += uBufferSize;

EXIT1:
    nResult = true;
EXIT0:
    if (!nResult)
    {
		SAFE_RELEASE(piBuffer);

        if (nAllocFlag)
        {
            SAFE_RELEASE(m_piBufferReSize);
            SAFE_RELEASE(m_piBuffer);
            m_pbyBufferBegin = m_pbyBufferPos = m_pbyBufferEnd = NULL;

            nAllocFlag = false;
        }
    }
    return nResult;
}

int QSocket_Packer::FlushSend(ISocketStream *piSocketStream)
{
    int nResult  = false;
    int nRetCode = false;

    PROCESS_ERROR(piSocketStream);

    PROCESS_SUCCESS(!m_piBuffer);    // if no buffer so no contain data
    PROCESS_SUCCESS(m_pbyBufferPos == m_pbyBufferBegin); // if buffer no contain data

    nRetCode = m_piBufferReSize->SetSmallerSize(m_pbyBufferPos - m_pbyBufferBegin);
	PROCESS_ERROR(nRetCode);

    nRetCode = piSocketStream->Send(m_piBuffer);
    PROCESS_ERROR(nRetCode == 1); // when timeout and error then error

    nRetCode = Reset();
    PROCESS_ERROR(nRetCode);

EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

int QSocket_Packer::Reset()
{
    int nResult  = false;
    int nRetCode = false;

    // make sure the m_piBuffer is alloc
    if (m_piBufferReSize)
    {
        nRetCode = m_piBufferReSize->ResetSize();       // resize so can reuse m_piBuffer
        PROCESS_ERROR(nRetCode);

        m_pbyBufferPos = m_pbyBufferBegin;
    }

    nResult = true;
EXIT0:
    return nResult;
}

