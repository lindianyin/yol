

#include "CoreDeclspec.h"

#define	_CRT_SECURE_NO_DEPRECATE
#include "Runtime/Stddef.h"

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "my_md5.h"

#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

/* Macro define region */
#define EASYCRYPT_INITCODE			"kvkaz"
#define EASYCRYPT_INITCODELEN		5
#define MAXSIZE_EASYCRYPTKEY		(2+EASYCRYPT_INITCODELEN+10)
#define SIZE_CRYPTSUMMARY			16
#define SIZE_CRYPTSUMMARYCHUNK		5
#define SIZE_MD5SUMMARY			    16

 
/* Struct define region */
struct tagEASYCRYPTKEY
{
	CHAR szEncryptKey[MAXSIZE_EASYCRYPTKEY];
	WORD wEncryptKeyLen;
};
typedef struct tagEASYCRYPTKEY EASYCRYPTKEY , *PEASYCRYPTKEY;

#ifdef WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif

static void GenCryptSummary(LPSTR pData, 
				DWORD dwDataLen, 
				LPSTR pSummary)
{
	int i;
	DWORD n;
	DWORD dwGroupCount;

	Md5_calc( pSummary , pData, dwDataLen );

	for( i=0; i<SIZE_MD5SUMMARY; i++)
	{
		pSummary[i] = ~pSummary[i];
	}

	dwGroupCount = 
		SIZE_MD5SUMMARY / SIZE_CRYPTSUMMARYCHUNK;
	if(SIZE_MD5SUMMARY % SIZE_CRYPTSUMMARYCHUNK != 0)
	{
		dwGroupCount++;
	}
	
	for( n = 0; n < dwGroupCount; n++)
	{
		char chSwap;
		int nLeftIndex, nRightIndex;		
		DWORD dwSwapCount;
		DWORD m ;

		if(n == dwGroupCount - 1)
		{
			dwSwapCount = 
				(SIZE_MD5SUMMARY % SIZE_CRYPTSUMMARYCHUNK) / 2;
		}
		else
		{
			dwSwapCount = SIZE_CRYPTSUMMARYCHUNK / 2;
		}

		for( m=0; m<dwSwapCount; m++)
		{
			nLeftIndex = n * SIZE_CRYPTSUMMARYCHUNK + m;
			nRightIndex = n * SIZE_CRYPTSUMMARYCHUNK + 
				SIZE_CRYPTSUMMARYCHUNK - m - 1;
			chSwap = pSummary[nLeftIndex];
			pSummary[nLeftIndex] = pSummary[nRightIndex];
			pSummary[nRightIndex] = chSwap;
		}
	}
}


static void CryptData(LPSTR pData, 
				DWORD dwDataLen, 
				EASYCRYPTKEY tagKey)
{
	DWORD i ;
	DWORD j ;
	char szCryptSummary[SIZE_CRYPTSUMMARY] = {0};

	GenCryptSummary(tagKey.szEncryptKey, 
				tagKey.wEncryptKeyLen, 
				szCryptSummary);

	for( i=0; 
		i<dwDataLen; 
		i += SIZE_CRYPTSUMMARY)
	{
		DWORD dwChunkSize = 
			dwDataLen-i < SIZE_CRYPTSUMMARY ? 
			dwDataLen-i : SIZE_CRYPTSUMMARY;
		for( j=0; j<dwChunkSize; j++)
		{
			pData[i+j] = 
				pData[i+j] ^ szCryptSummary[j];
		}
	}
}



int Encrypt(size_t dwEncryptDataLen, unsigned char *pEncryptData, unsigned *puCryptSign)
{
	char chSwap;
	DWORD i;
	// char strtemp[10];
	EASYCRYPTKEY tagEncryptKey;

	/*for( i = 0 ; i < dwEncryptDataLen ; i++)
		pEncryptData[i] = ~pEncryptData[i];

	return ;*/

    if (!puCryptSign)
    {
        return 0;
    }
	
	
	if(dwEncryptDataLen == 0)
	{
		return 0;
	}

	memset(&tagEncryptKey, 
		0, 
		sizeof(EASYCRYPTKEY));

	if(dwEncryptDataLen > 2)
	{
		// Generate encrypt key
		tagEncryptKey.szEncryptKey[0] = pEncryptData[0];
		tagEncryptKey.szEncryptKey[1] = pEncryptData[dwEncryptDataLen-1];
		tagEncryptKey.wEncryptKeyLen += 2;
		memcpy(tagEncryptKey.szEncryptKey + tagEncryptKey.wEncryptKeyLen, 
			EASYCRYPT_INITCODE, 
			EASYCRYPT_INITCODELEN);
		tagEncryptKey.wEncryptKeyLen += EASYCRYPT_INITCODELEN;

		/*sprintf( strtemp , "%x" , dwCryptSign );
		strncpy( tagEncryptKey.szEncryptKey+tagEncryptKey.wEncryptKeyLen ,
			    strtemp , 10 );*/

		//itoa(*puCryptSign, 
		//	tagEncryptKey.szEncryptKey+tagEncryptKey.wEncryptKeyLen, 
		//	16);
        // itoa not implemented in gcc !

        sprintf(tagEncryptKey.szEncryptKey+tagEncryptKey.wEncryptKeyLen, "%x", *puCryptSign);

		tagEncryptKey.wEncryptKeyLen += 
			strlen(tagEncryptKey.szEncryptKey + 
				tagEncryptKey.wEncryptKeyLen);

		pEncryptData[0] = ~pEncryptData[0];
		pEncryptData[dwEncryptDataLen-1] = ~pEncryptData[dwEncryptDataLen-1];

		chSwap = pEncryptData[0];
		pEncryptData[0] = pEncryptData[dwEncryptDataLen-1];
		pEncryptData[dwEncryptDataLen-1] = chSwap;

		CryptData(pEncryptData + 1, 
				dwEncryptDataLen - 2, 
				tagEncryptKey);
	}
	else
	{
		for( i=0; i<dwEncryptDataLen; i++)
			pEncryptData[i] = ~pEncryptData[i];
	}

    return 1;
}

int Decrypt(unsigned dwDecryptDataLen, unsigned char *pDecryptData, unsigned *puCryptSign)
{	
	DWORD i;
//	char strtemp[10];	
	EASYCRYPTKEY tagDecryptKey;


	/*for( i = 0 ; i < dwDecryptDataLen ; i++)
		pDecryptData[i] = ~pDecryptData[i];

	return ;*/
    if (!puCryptSign)
    {
        return 0;
    }

	if(dwDecryptDataLen == 0)
	{
		return 0;
	}

	memset(&tagDecryptKey, 
		0, 
		sizeof(EASYCRYPTKEY));
	if(dwDecryptDataLen > 2)
	{
		char chSwap;
		chSwap = pDecryptData[0];
		pDecryptData[0] = pDecryptData[dwDecryptDataLen-1];
		pDecryptData[dwDecryptDataLen-1] = chSwap;
		pDecryptData[0] = ~pDecryptData[0];
		pDecryptData[dwDecryptDataLen-1] = ~pDecryptData[dwDecryptDataLen-1];

		// Generate decrypt key
		tagDecryptKey.szEncryptKey[0] = pDecryptData[0];
		tagDecryptKey.szEncryptKey[1] = pDecryptData[dwDecryptDataLen-1];
		tagDecryptKey.wEncryptKeyLen += 2;
		memcpy(tagDecryptKey.szEncryptKey + tagDecryptKey.wEncryptKeyLen, 
			EASYCRYPT_INITCODE, 
			EASYCRYPT_INITCODELEN);
		tagDecryptKey.wEncryptKeyLen += EASYCRYPT_INITCODELEN;

		/*sprintf( strtemp , "%x" , dwCryptSign );
		strncpy( tagDecryptKey.szEncryptKey+tagDecryptKey.wEncryptKeyLen ,
			    strtemp , 10 );*/
		
		//itoa(*puCryptSign, 
		//	tagDecryptKey.szEncryptKey+tagDecryptKey.wEncryptKeyLen, 
		//	16);
        // itoa not implemented in gcc !
        sprintf(tagDecryptKey.szEncryptKey+tagDecryptKey.wEncryptKeyLen, "%x", *puCryptSign);

		tagDecryptKey.wEncryptKeyLen += 
			strlen(tagDecryptKey.szEncryptKey + 
				tagDecryptKey.wEncryptKeyLen);

		CryptData(pDecryptData + 1, 
				dwDecryptDataLen - 2, 
				tagDecryptKey);
	}
	else
	{
		for( i=0; i<dwDecryptDataLen; i++)
			pDecryptData[i] = ~pDecryptData[i];
	}

    return 1;
}

