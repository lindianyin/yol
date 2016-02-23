
#pragma once

extern "C" int Encrypt(size_t dwEncryptDataLen, unsigned char *pEncryptData, unsigned *puCryptSign);

extern "C" int Decrypt(size_t dwDecryptDataLen, unsigned char *pDecryptData, unsigned *puCryptSign);

