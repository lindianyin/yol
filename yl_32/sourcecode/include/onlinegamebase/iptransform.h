
#pragma once

// -------------------------------------------------------------------------
// 此类是读取一个前部为65536 个 结构体 KIP_HASHTABLE 的HASH表
// 此HASH表的key 为 IP 的前2段
// 后部为整数个的 KIP_TABLE 结构体 存放的是IP记录

#define KD_IP_BUCKET_COUNT		65536				// HASH  的字段个数
#define	KD_IP_MAX_ADDR_LEN		40					// IP所对应地区


// 存储IP和IP所对应地址的数据结构
struct KIP_TABLE
{
	UINT uIp;
	CHAR szAddr[KD_IP_MAX_ADDR_LEN];
};

// HASH 表的指针 有来指向一段IP 有可能相同 就是一个IP
struct KIP_HASHTABLE
{
	INT nBeginPos;
	INT nEndPos;
};

class KIpTransForm
{
public:
	KIpTransForm();
	~KIpTransForm();

	BOOL			Init(LPCSTR pszFileName);					// 初始化装载IPshuju	
	BOOL			GetIpAddr(LPCSTR pszIp, LPSTR pszArea);		// 取IP所对应的国家地区
	BOOL			GetIpAddr(UINT uOrgIp,	LPSTR pszArea);
private:
	UINT			GetIpFromString(LPCSTR pszIp);				// 把字符的IP地址转成UINT

private:
	BOOL			m_bInit;									// 是否装载IP数据库成功
	KIP_TABLE*		m_pIpTable;									// IP 数据表 包含IP地址和所指向的具体国家地址
	KIP_HASHTABLE	m_sIpHashTable[KD_IP_BUCKET_COUNT];			// IP 数据库的HASH 表,由IP的前2段构成
};

// -------------------------------------------------------------------------

