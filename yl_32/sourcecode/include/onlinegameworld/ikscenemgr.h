
#pragma once

class IKScene;
struct  KSCENE_CONFIG;

class IKSceneMgr
{
public:
	virtual	BOOL	Init()	PURE;
	virtual	BOOL	Uninit()	PURE;

	// 每帧激活已加载的场景
	virtual	BOOL	Activate()	PURE;
	virtual	INT		GetCount()	PURE; // get count of loaded scenes

	// 加载/卸载场景
	virtual	IKScene*	LoadScene(INT nMapId, INT nMapCopyIndex)	PURE;
	virtual	BOOL	UnloadScene(INT nMapId, INT nMapCopyIndex = 0)	PURE;

	// 根据ID获得场景接口
	virtual	IKScene*	GetScene(INT nMapId, INT nMapCopyIndex = 0)	PURE;

	// 遍历加载的场景
	virtual	IKScene*	FirstScene()	PURE;
	virtual	IKScene*	NextScene()	PURE;

	virtual CONST KSCENE_CONFIG* GetSceneInfo(DWORD dwMapTemplateId) PURE;

	virtual INT GetSceneType(DWORD dwMapTemplateId) PURE;

	virtual BOOL CanTeam(DWORD dwMapTemplateId) PURE;
	virtual	BOOL IsSetFightMode(DWORD dwMapTemplateId) PURE;
};

extern	IKSceneMgr*	g_pSceneMgr;
