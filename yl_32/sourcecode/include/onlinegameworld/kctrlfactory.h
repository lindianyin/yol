
#pragma once

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "onlinegamebase/performancestat.h"
#include "config/kcharacterdef.h"

template<class TCONTAINER>
struct IKCtrlCtor
{
	virtual LPVOID Create(TCONTAINER& rcContainer) = 0;
	virtual VOID Release(LPVOID pCtrl) = 0;
};

// TCONTAINER是拥有TCTRL的类，如KCharacter拥有KCharacterSkill，且KCharacterSkill构造函数包含KCharacter引用
template<class TCONTAINER, class TCTRL>
class KTCtrlCtor : public IKCtrlCtor<TCONTAINER>
{
public:
	virtual LPVOID Create(TCONTAINER& rcContainer) { return new TCTRL(rcContainer); }
	virtual VOID Release(LPVOID pCtrl) { delete reinterpret_cast<TCTRL*>(pCtrl); }
	static IKCtrlCtor<TCONTAINER>* GetInterface()
	{
		static KTCtrlCtor<TCONTAINER, TCTRL> s_Inst;
		return &s_Inst;
	}
};

#define CTRL_PERFOMANCE_STAT

#define CTRL_STAT(N) \
if (m_aCtrl[N]) \
{ \
	BEGIN_STAT("Ctrl"#N); \
	QCONFIRM(m_aCtrl[N]->Active()); \
	END_STAT(); \
}

// -------------------------------------------------------------------------
// TCTRL 必须 继承于TCTRL_BASE，并且要包含emKCtrlId，TCTRL_BASE必须要有OnInit及OnUnInit函数
// 只能先这样了，更合理的实现等c++0x的concept等功能吧
template<class TCONTAINER, class TCTRL_BASE, INT N_MAX_CTRLID>
class KTCtrlFactory
{
public:
	VOID CreateCtrls(TCONTAINER& rcContainer)
	{
		memset(m_aCtrl, 0, sizeof(m_aCtrl));

		for (INT i = 0; i < N_MAX_CTRLID; ++i)
		{
			if (m_saCtrlCtor[i])
				m_aCtrl[i] = reinterpret_cast<TCTRL_BASE*>(m_saCtrlCtor[i]->Create(rcContainer));
		}
	}

	~KTCtrlFactory()
	{
		for (INT i = 0; i < N_MAX_CTRLID; ++i)
		{
			// 没有构造器怎么会有CtrlBase实例
			ASSERT( !m_aCtrl[i] || m_saCtrlCtor[i]);
			if (m_saCtrlCtor[i] && m_aCtrl[i])
				m_saCtrlCtor[i]->Release(m_aCtrl[i]);
		}
	}

	BOOL InitCtrls()
	{
		for (INT i = 0; i < N_MAX_CTRLID; ++i)
		{
			if (m_aCtrl[i])
				QCONFIRM_RET_FALSE(m_aCtrl[i]->Init());
		}
		return TRUE;
	}

	BOOL UnInitCtrls()
	{
		for (INT i = 0; i < N_MAX_CTRLID; ++i)
		{
			if (m_aCtrl[i])
				QCONFIRM(m_aCtrl[i]->UnInit());
		}
		return TRUE;
	}

	BOOL ActiveCtrls()
	{
		/*
#ifdef CTRL_PERFOMANCE_STAT
		CTRL_STAT(0);
		CTRL_STAT(1);
		CTRL_STAT(2);
		CTRL_STAT(3);
		CTRL_STAT(4);
		CTRL_STAT(5);
		CTRL_STAT(6);
		CTRL_STAT(7);
		CTRL_STAT(8);
		CTRL_STAT(9);
		CTRL_STAT(10);
		CTRL_STAT(11);
		CTRL_STAT(12);
		CTRL_STAT(13);
		CTRL_STAT(14);
		CTRL_STAT(15);
		CTRL_STAT(16);
		CTRL_STAT(17);
		CTRL_STAT(18);
		CTRL_STAT(19);
		CTRL_STAT(20);
		CTRL_STAT(21);
		CTRL_STAT(22);
		CTRL_STAT(23);
		CTRL_STAT(24);
		CTRL_STAT(25);
		CTRL_STAT(26);
		CTRL_STAT(27);
		CTRL_STAT(29);
		CTRL_STAT(30);
		CTRL_STAT(31);
		CTRL_STAT(32);
#else
		*/
		for (INT i = 0; i < N_MAX_CTRLID; ++i)
		{
			if (m_aCtrl[i])
				QCONFIRM(m_aCtrl[i]->Active());
		}
//#endif
		return TRUE;
	}

	template<class TCTRL>
	TCTRL* GetCtrl()
	{
		QCONFIRM_RET_NULL((UINT)TCTRL::emKCtrlId < N_MAX_CTRLID);
		return static_cast<TCTRL*>(m_aCtrl[TCTRL::emKCtrlId]);
	}
	// 按CtrlId获得一个CtrlBase指针
	TCTRL_BASE* GetCtrl(INT nCtrlId)
	{
		QCONFIRM_RET_NULL(nCtrlId < N_MAX_CTRLID);
		return m_aCtrl[nCtrlId];
	}

	// 以nTraverseId值为起始获取下一个Ctrl，并将nTraverseId设为该CtrlId，若找不到返回NULL并把nTraverseId设为-1
	// nTraverseId < 0时从头开始搜索
	TCTRL_BASE* TraverseNextCtrl(INT& nTraverseId)
	{
		if (nTraverseId < -1)
			nTraverseId = -1;
		while(++nTraverseId < N_MAX_CTRLID)
		{
			if (m_aCtrl[nTraverseId])
				return m_aCtrl[nTraverseId];
		}
		nTraverseId = -1;
		return NULL;
	}

	// 注册Ctrl，应当在实体调CreateCtrls()前调用静态函数RegisterCtrl()注册Ctrl类
	// TCTRL需包含emKCtrlId枚举指定Ctrl的ID，每个Ctrl的ID应独立并小于N_MAX_CTRLID
	template<class TCONTAINER1, class TCTRL>
	static VOID RegisterCtrl()
	{
		(void)static_cast<TCTRL_BASE*>((TCTRL*)NULL); // 检查继承关系
		QCONFIRM(m_saCtrlCtor[TCTRL::emKCtrlId] == NULL);
		m_saCtrlCtor[TCTRL::emKCtrlId] = KTCtrlCtor<TCONTAINER1, TCTRL>::GetInterface();
	}
protected:
	TCTRL_BASE* m_aCtrl[N_MAX_CTRLID];
	static IKCtrlCtor<TCONTAINER>* m_saCtrlCtor[N_MAX_CTRLID];
};

template<class TCONTAINER, class TCTRL_BASE, INT N_MAX_CTRLID>
	IKCtrlCtor<TCONTAINER>* KTCtrlFactory<TCONTAINER, TCTRL_BASE, N_MAX_CTRLID>::m_saCtrlCtor[N_MAX_CTRLID] = { 0 };
// -------------------------------------------------------------------------
