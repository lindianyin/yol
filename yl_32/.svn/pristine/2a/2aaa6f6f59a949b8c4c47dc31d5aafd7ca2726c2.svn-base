/* -------------------------------------------------------------------------
//	文件名		：	kinterfacemgr.h
//	创建者		：	luobaohang
//	创建时间	：	2010/7/14 10:40:29
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KINTERFACEMGR_H__
#define __KINTERFACEMGR_H__

// -------------------------------------------------------------------------
class KInterfaceMgr
{
	struct CmpRefId : public std::binary_function<IID, IID, bool>
	{
		bool operator()(CONST IID& riid1, CONST IID& riid2) CONST
		{ 
			return memcmp(&riid1, &riid2, sizeof(IID)) < 0;
		}
	};
public:
	static KInterfaceMgr* Inst();
	// 初始化接口
	BOOL Init();
	// 注册一个接口
	BOOL RegisterInterface(REFIID riid, LPVOID pvObject);
	LPVOID GetInterface(REFIID riid);
private:
	typedef std::map<IID, LPVOID, CmpRefId> MP_INTERFACE;
	MP_INTERFACE m_mpInterface;
};

// -------------------------------------------------------------------------

#endif /* __KINTERFACEMGR_H__ */
