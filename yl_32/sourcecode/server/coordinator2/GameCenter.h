
#pragma once

#include "MapManager.h"
#include "RoleManager.h"
#include "ServerHandler.h"
#include "GameCenterSettings.h"
#include "RoleDB.h"
#include "GameCenterMonitor.h"
#include "GatewayAgency.h"
#include "MiscDB.h"
//#include "BattleFieldManager.h"
#include "team/TeamCenter.h"
#include "IDSequence.h"
#include "StatDataManager.h"
#include "LogServerAgency.h"
#include "BornPlaceList.h"
#include "kin/KinManager.h"
#include "battlezone/battlezonedef.h"
#include "mail2/MailManager.h"
#include "relation/relationmanager.h"
#include "auction/AuctionManager.h"
#include "arena/ArenaManager.h"
#include "rank/krankmanager.h"
#include "QRandomName/QRandomName.h"
#include "quiz/QuizManager.h"
#include "record/krecordmanager.h"
#include "escort/kescortmanager.h"
#include "PayAgency.h"
#include "GMSendAwardDB.h"
#include "openseSave.h"

class QGameCenter
{
public:
    QGameCenter();

    BOOL Init(void);

	void InitScriptEnv();

	void UnInit();

    void Run();
    
    void Activate();
    
    void Quit();

    BOOL CallMainScript(const char cszFunction[]);

    BOOL CheckQuitComplete();

	VOID OnRegularSaveProcess();

public:
    time_t                      m_nBaseTime;
    time_t                      m_nTimeNow;
    int                         m_nWorkLoop;
    int                         m_nIdleLoop;

    KGatewayAgency              m_Gateway;
	//支付模块先注释掉
	//KPayAgency					m_PayAgency;

    KGameServer                 m_GameServer;

    KMapManager		            m_MapManager;
    KRoleManager                m_RoleManager;

    KTongManager		        m_TongManager;
	QRandomName                 m_QRandomName;
    //KGFellowshipCenter	        m_FellowshipCenter;

	KRankManager				m_RankManager;

    KAuctionManager             m_AuctionManager;
	KRelationManager_GC         m_RelationManager;

    QTeamCenter                 m_TeamCenter;

    KMiscDB                     m_MiscDB;

    QGameCenterSettings      m_Settings;

    QBornPlaceList               m_BornPlaceList;
    
    KRoleDB                     m_RoleDB;

	GMSendAwardDB				m_GMSendAward;

	//开服活动
	OpenseSave					m_OpenseSave;
    //KBattleFieldManager         m_BattleFieldManager;
    
    KGameCenterEyes             m_Monitor;

    //KGPQCenter                  m_PQCenter;
    
    //KCampManager                m_CampManager;

    //KGlobalCustomDataManager    m_GlobalCustomDataManager;

    //KGlobalSystemValueManager   m_GlobalSystemValueManager;

    KStatDataManager            m_StatDataManager;

    IKIDSequence*               m_piSequence;

    //KRoadCenter                 m_RoadCenter;

    //KActivityManager            m_ActivityManager;

    //KApexProxy                  m_ApexProxy;

    //KGameCardManager            m_GameCardManager;

    KLogServerAgency            m_LogServerAgency;

    //KAntiFarmerManager          m_AntiFarmerManager;

    //KMentorCenter               m_MentorCenter;

	KMailManager                m_MailManager;

	//PVE竞技场k
	ArenaManager				m_ArenaManager;

	//答题
	QuizManager					m_QuizManager;

	// 服务器记录
	KRecordManager				m_RecordManager;

	KEscortManager				m_EscortManager;

    BOOL                        m_bQuitFlag;

    int                         m_nZoneChargeFlag;

	BOOL						m_bDevMode;

	int 						gDay;
};

extern QGameCenter* g_pGameCenter;
