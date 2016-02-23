/* -------------------------------------------------------------------------
//	文件名		：	ikscenemovectl.h
//	创建者		：	xiewen
//	创建时间	：	2009-9-27 17:39:39
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __IKSCENEMOVECTL_H__
#define __IKSCENEMOVECTL_H__

// -------------------------------------------------------------------------
class KRegion;
class KSceneObject;

struct IKSceneMoveCtl
{
public:
	virtual	BOOL	Init(KSceneObject* pSceneObj)	PURE;
	virtual	BOOL	Uninit()	PURE;

	virtual	BOOL	SetBlockingInfo(KRegion** ppRegionList, INT nRegionWidth, INT nRegionHeight)	PURE;
	virtual	BOOL	Activate()	PURE;

	virtual	BOOL	Jump(BOOL bStandJump, INT nJumpDirection, BOOL bSyncSelf) PURE;
};

extern	IKSceneMoveCtl*	g_pSceneMoveCtl;
// -------------------------------------------------------------------------

#endif /* __IKSCENEMOVECTL_H__ */
