
#pragma once
// -------------------------------------------------------------------------
// 随机数发生器，能生成随机性，均匀性，随机种子敏感度均较高的32位伪随机数序列，
// 优于普通线性同余随机数发生器及gcc的递归叠加式随机数发生器
// 不同于普通线性同余，每次用不同的RandSeed初始化，伪随机序列都会发生彻底变化，而不仅仅是起始点不一样
class QRandGentor
{
	enum
	{
		ARYRAND_NUM = 11
	};
public:
	QRandGentor() { RandSeed(0); }
	// RandSeed只在游戏初始化时调用一次，勿在游戏运行过程调用
	VOID RandSeed(DWORD dwSeed)
	{
		m_dwRandSeed = dwSeed;
		for (INT i = 0; i < ARYRAND_NUM; ++i)
		{
			m_aDwRand[i] = LcgRand();
		}
	}
	DWORD Rand()
	{
		DWORD dwRand = LcgRand();
		UINT uIndex = dwRand % (ARYRAND_NUM * 4 - 3);
		return *(LPDWORD)((LPBYTE)m_aDwRand + uIndex) += dwRand;
	}
protected:
	DWORD LcgRand()
	{
		return  m_dwRandSeed = m_dwRandSeed * 374786949 + 907633383;
	}
protected:
	DWORD m_aDwRand[ARYRAND_NUM];
	DWORD m_dwRandSeed;
};

// 可用于产生随机性良好的固定伪随机数序列
inline DWORD g_HashWord( DWORD dwRandSeed )
{
	// 32位整数hash(from Thomas Wang: 《Integer Hash Function》)
	// 及格的哈希函数必要条件是N位hash的结果集必须达到最大（2^N），且对所有输入均匀分布。
	dwRandSeed = ~dwRandSeed + ~(dwRandSeed << 15); // key = (key << 15) - key - 1;
	dwRandSeed ^= dwRandSeed >> 12;
	dwRandSeed += dwRandSeed << 2;
	dwRandSeed ^= dwRandSeed >> 4;
	dwRandSeed *= 2057; // key = (key + (key << 3)) + (key << 11);
	dwRandSeed ^= dwRandSeed >> 16;
	return dwRandSeed;
}
// -------------------------------------------------------------------------

