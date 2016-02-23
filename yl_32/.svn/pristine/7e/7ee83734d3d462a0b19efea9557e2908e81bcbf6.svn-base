
#pragma once


class QSocket_Packer
{
public:
    int Send(
        ISocketStream *piSocketStream, 
        unsigned uBufferSize, const unsigned char cbyBuffer[]
    );
    int FlushSend(ISocketStream *piSocketStream);
    
    int Reset();

    QSocket_Packer(unsigned uPackSize) : 
        m_uPackSize(uPackSize), 
        m_piBuffer(NULL),
        m_piBufferReSize(NULL),
        m_pbyBufferBegin(NULL), m_pbyBufferPos(NULL), m_pbyBufferEnd(NULL) 
    { 
        ASSERT(uPackSize > 0);
    }
    ~QSocket_Packer() 
    { 
        SAFE_RELEASE(m_piBufferReSize);
        SAFE_RELEASE(m_piBuffer);
        m_pbyBufferBegin = m_pbyBufferPos = m_pbyBufferEnd = NULL;
    }

private:
    unsigned                 m_uPackSize;

    IMemBlock              *m_piBuffer;
    IMemBlock_Resizable       *m_piBufferReSize;

    unsigned char           *m_pbyBufferBegin;
    unsigned char           *m_pbyBufferPos;
    unsigned char           *m_pbyBufferEnd;
};

