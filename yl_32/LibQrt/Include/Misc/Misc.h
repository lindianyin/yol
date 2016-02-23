
#pragma once
#include<stdint.h>

CORE_API_C uint32_t ngx_crc32_long(unsigned char *p, size_t len);

#define Misc_MD5 Md5_calc

CORE_API_C void Md5_calc(unsigned char md5[16], unsigned char* src, size_t src_len);

CORE_API_C void Misc_MD5String(char szMD5String[33], unsigned char* src, size_t src_len);
