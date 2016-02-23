
#pragma once

class KCharacter;
// -------------------------------------------------------------------------
class KCharacterCtrlBase
{
public:
	KCharacterCtrlBase(KCharacter& rcCharacter) 
		: m_rcCharacter(rcCharacter)
	{}
	virtual ~KCharacterCtrlBase() {}

	virtual BOOL Init() = 0;
	virtual BOOL UnInit() = 0;
	virtual BOOL OnLogin() { return TRUE; }
	virtual BOOL OnLogout() { return TRUE; }
	virtual BOOL Active() {return TRUE;}

#ifdef GAME_SERVER
	// 存盘功能（在playerctrldbdef定义并在派生类实现这对函数，就可以参与存盘和读盘）
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize)
	{ Q_Error("指定了该Ctrl要存盘，但是却未实现？"); return FALSE; }
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
	{ Q_Error("指定了该Ctrl要存盘，但是却未实现？"); return FALSE; }

	virtual BOOL OnClientReady() { return TRUE; }
#endif

	KCharacter& m_rcCharacter;
};
// -------------------------------------------------------------------------

