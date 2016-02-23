#ifndef _FSEYE_PROTOCOL_H
#define _FSEYE_PROTOCOL_H

#include "kipcstream.h"

#pragma pack(push, 1)

enum
{
	emKEYE_NAMELEN = 32,
};

struct KProtocolHead
{
	unsigned short wProtocol;
	unsigned short wServer;
};

struct KSubProtocolHead : public KProtocolHead
{
	unsigned short wSubProtocol;
};

struct g2e_python_report: public KProtocolHead
{
	unsigned int	uSessionID;
	unsigned int	uData[4];
	char			szMessage[256];
};

//==================== 文件传输 (旧版本) ========================

struct KFileTrans_ProtocolHead : public KProtocolHead 
{
	unsigned short wSubProtocol;
};

struct e2g_openfile : public KFileTrans_ProtocolHead
{
	unsigned char bFlag;
	unsigned char bFullPath;
	char szFileName[256];
};

struct g2e_openfile : public KFileTrans_ProtocolHead
{
	unsigned int nFileLen;
	int nRetCode;
};

struct e2g_readfile : public KFileTrans_ProtocolHead
{
	unsigned short nDataLen;
};

struct g2e_readfile : public KFileTrans_ProtocolHead
{
	int nRetCode;
	unsigned int nReadLen;
};

struct e2g_writefile : public KFileTrans_ProtocolHead
{
	unsigned short nDataLen;
};

struct g2e_writefile : public KFileTrans_ProtocolHead
{
	unsigned int nWritedLen;
	int nRetCode;
};

struct e2g_seekfile : public KFileTrans_ProtocolHead
{
	unsigned char bKeep;
	unsigned int nOffset;
};

struct g2e_seekfile : public KFileTrans_ProtocolHead
{
	int nRetCode;
};

struct g2e_closefile : public KFileTrans_ProtocolHead
{
	int nRetCode;
};

//==================== 文件传输 (旧版本) END ========================

struct g2e_loadplug : public KProtocolHead
{
	int nRetCode;
};

struct g2e_unloadplug : public KProtocolHead
{
	int nRetCode;
};

struct cpubaseinfo {
	char szCPUName[64];
	char szCPUVendor[64];
	char szCPUDesc[256];
	int nCPULoad;
};

struct g2e_getcpubaseinfo : public KProtocolHead
{
	unsigned short nCPUCount;	//后跟长度为nCPUCount的cpubaseinfo数组
};

struct g2e_getcpuload : public KProtocolHead
{
	unsigned short nCPUCount;	//后跟长度为nCPUCount的unsigned short数组
};

struct g2e_getmeminfo : public KProtocolHead
{
	unsigned int nTotalMem;
	unsigned int nFreeMem;
};

struct diskbaseinfo {
	char szDiskDesc[100];
	unsigned int nTotalSize;
	unsigned int nFreeSize;
};

struct g2e_getdiskinfo : public KProtocolHead
{
	unsigned short nDiskCount;
	diskbaseinfo Disk[10];
};

struct netcardinfo {
	char szCardDesc[100];
	char szIP[20];
	char szMask[20];
	char szMac[20];
};

struct g2e_getnetinfo : public KProtocolHead
{
	unsigned short	nCardCount;
	netcardinfo		Card[10];
	char			szSystemName[100];
	char			szHostName[100];
};

struct netcardload {
	unsigned int nTXSize;
	unsigned int nRXSize;
	unsigned int nTXRate;
	unsigned int nRXRate;
};

struct g2e_getcardload : public KProtocolHead
{
	unsigned short nCardCount;
	netcardload Card[10];
};

struct procinfo {
	char szProcName[50];
	unsigned int nPID;
	unsigned int nMemUse;
	unsigned int nVMSize;
	unsigned int nCPUTime;
	unsigned int nThreadCount;
};

struct g2e_getprocinfo : public KProtocolHead
{
	unsigned short nProcCount;
	procinfo Proc[80];
};

struct pluginfo {
	char szPath[50];
	char szModHAndModE[50];
	char szGUID[50];
	char szAuthor[50];
	char szVersion[50];
	char szDesc[100];
};

struct g2e_getpluginfo : public KProtocolHead
{
	int PlugCount;
	pluginfo PlugInfo[5];
};

struct e2g_exesql  : public KProtocolHead
{
	unsigned int	nSessionID;
	char			szDatabase[48];
	unsigned int	nSqlLen;
};

// 执行SQL语句：如果第一个SQL语句执行不成功则执行第二个SQL语句
struct e2g_exesql2 : public KProtocolHead
{
	unsigned int	nSessionID;
	char			szDatabase[48];
	unsigned int	nSql1Len;			// 第一个SQL语句长度
	unsigned int	nSql2Len;			// 第二个SQL语句长度
};

struct g2e_exesql : public KProtocolHead
{
	unsigned int nSessionID;
	unsigned int nRetCode;
	unsigned int nLen;
};

//已废弃，使用e2g_shellscript
struct e2g_exesyscmd : public KFileTrans_ProtocolHead
{
	unsigned short	bNeedOutput;
	unsigned short	bNeedCN;
	unsigned short	nCmdLen;
	unsigned short	nBufLen;
};

//已废弃，使用g2e_shellscript
struct g2e_exesyscmd  : public KFileTrans_ProtocolHead
{
	int ReturnCode;
	char OutputBuff[256];
};

struct e2l_SayToWorld : public KSubProtocolHead
{
	char Message[256];
};

struct l2e_GetBasicInfo : public KSubProtocolHead
{
	unsigned short PlayerCount;
	unsigned short UpTime;
};
struct e2l_ExeGMCmd : public KProtocolHead
{
	unsigned int  nSessionID;
	unsigned int uCmdLen;
};

struct l2e_ExeGMCmd : public KSubProtocolHead
{
	unsigned int nSessionID;
	int ReturnCode;
	int nResultLen;
};

struct e2g_Authentication : public KProtocolHead
{
	int X;
};

struct g2e_Authentication : public KProtocolHead
{
	char Y[64];
};

enum KE_GUARD_PLAYERCOUNT
{
	emKGUARD_PLAYERCOUNT_TOTOAL = 0,	// 总的玩家人数
	emKGUARD_PLAYERCOUNT_OFFLIVE,		// 离线托管人数
	emKGUARD_PLAYERCOUNT_COUNT
};

// 变长协议，后面跟一个INT数组，数组最大数目为emKPLAYERCOUNT_COUNT，如果人数为-1表示未知
struct l2e_PlayerCount : public KSubProtocolHead
{
	unsigned short uCount;
};

struct g2e_GetGuardDir : public KProtocolHead
{
	char GuardDir[256];
};

struct e2l_Who : public KSubProtocolHead
{
	unsigned short Offset;
};

struct l2e_Who_PlayerInfo {
	char Name[32];
};

struct l2e_Who : public KSubProtocolHead
{
	unsigned short PlayerCount;
	l2e_Who_PlayerInfo PlayerList[20];
};

struct e2l_GetGlobalVariable : public KSubProtocolHead
{
	unsigned short VariableIndex;
};

struct l2e_GetGlobalVariable : public KSubProtocolHead
{
	unsigned short VariableIndex;
	int VariableValue;
};

struct e2l_SetGlobalVariable : public KSubProtocolHead
{
	unsigned short VariableIndex;
	int VariableValue;
};

struct e2g_Key {
	unsigned char Data[128];
};

struct e2g_ConfigInfo {
	int X;
	char Y[64];
	unsigned short ConfigFileDataLength;
	char ConfigFileData[4096];
};

struct e2g_config : public KProtocolHead
{
	e2g_Key Key;
	unsigned char ConfigInfo[4166];
};

struct e2g_file : public KProtocolHead
{
	unsigned short wFileLen;
	char szPath[256];
	char szData[4096];
};

struct e2g_switchmode : public KProtocolHead 
{
	unsigned short Mode;
};

struct l2e_GetGameStartTime : public KSubProtocolHead
{
	char GameStartTime[32];
};

struct l2e_ReportError : public KSubProtocolHead
{
	int Module;
	int ErrorCode;
};

struct e2g_DeliverKey : public KProtocolHead
{
	e2g_Key Key;
};

//已废弃，使用e2g_shellscript
struct e2g_exesyscmd_large : public KFileTrans_ProtocolHead
{
	unsigned short bNeedOutput;
	unsigned short	bNeedCN;
	char Command[6144];
	char InputBuff[256];
};

struct l2e_info : public KSubProtocolHead
{
	char Info[1024];
};

struct l2e_info_large : public KSubProtocolHead
{
	char InfoLarge[4096];
};

struct l2e_getpid : public KProtocolHead
{
	int	nPid;	// 进程ID
};

struct e2g_ftpdownload : public KProtocolHead
{
	char    szFtpHost[128];
	char    szFtpUser[32];
	char    szFtpPass[32];
	char    szFtpPath[256];
    int     nFtpPort;
};

struct g2e_ftpdownload : public KProtocolHead
{
	unsigned char	byProgress;	// 当前已下载百分比(0-100)
	unsigned char	byResult;	// 处理结果，1为成功，0为失败
	unsigned short	wLen;		// 附带消息长度
};

struct e2g_roleadmin : public KProtocolHead
{
	char	szFileName[128];	// python 需要执行的角色名列表文件;格式为: 角色名 TAB 导出服务器 TAB 导入服务器
	char	szOperation[128];	// output=导出 input=导入 get=获取导出文件 rename=更名 delete=删除 命令可以组合,用逗号分隔
	char	szZoneName[30];		// zone	
	char	szDBPassword[30];	// 导入和删除时需要提供密码
	char	szChangeINI[30];	// 导入时切换到另一个服务器使用的INI
};

struct g2e_roleadmin : public KProtocolHead
{
	unsigned char	byProgress;	// 当前已处理角色数百分比(0-100)
	unsigned char	byResult;	// 处理结果，1为成功，0为失败
	unsigned short	wLen;		// 附带消息长度
};

struct e2g_roleadminhistory : public KProtocolHead
{
	char	szRoleName[30];		// 需要从历史备份中导出的角色名
	char	szBeginDate[20];	// 开始日期(YYYY-MM-DD)
	char	szEndDate[20];		// 结束日期(YYYY-MM-DD)
	char	szDBName[30];		// 数据库名
};

struct g2e_roleadminhistory : public KProtocolHead
{
	unsigned char	byProgress;	// 当前已处理角色数百分比(0-100)
	unsigned char	byResult;	// 处理结果，1为成功，0为失败
	unsigned short	wLen;		// 附带消息长度
};
struct e2g_groupmerge : public KProtocolHead
{
	int		nMergeTaskID;				// 由eyes分配的并服任务号
	int		nAction;					// 本次的操作，值由enum KE_GROUP_MERGE_ACTION定义
	char	szGatewayNameMaster[50];	//主服的gatewayName
	char	szGatewayNameSlave[50];		// 从服的gatewayName
	char	szGameDBAddressSlave[50];	// 从服的数据库IP
	char	szGameDBUserNameSlave[50];	// 从服的db user name
	char	szGameDBPasswordSlave[50];	// 从服的db password
	int		nDBOnSameMachine;			//主服和从服的游戏数据库是否在同一个物理机上,1为在同一个物理机,0为不在
	
	char	szGameDBGamecenterMaster[50];	//主服Gamecenter数据库的名称
	char	szGameDBGoddessMaster[50];		//主服Goddess数据库的名称
	char	szGameDBGamecenterSlave[50];	//从服Gamecenter数据库的名称
	char	szGameDBGoddessSlave[50];		//从服Goddess数据库的名称
};

struct g2e_groupmerge : public KProtocolHead
{
	int		        nMergeTaskID;		// 由eyes分配的并服任务号
	int	            nAction;			// 具体的操作，值由enum KE_GROUP_MERGE_ACTION定义
	int	            nStatus;			// 某一步操作的进度状态，值由enum KE_GROUP_MERGE_ACTION_STATUS定义
	unsigned short	wLen;				// 附带消息长度    
};

struct  e2g_ExeMappedGMCmd : public KProtocolHead
{
	unsigned int  nSesssionId;			// 序列号
	char szCmdKey[64];			// 指令关键字
	unsigned short  nParamLen;				// 参数长度
};

enum KE_GROUP_MERGE_ACTION
{
    emKE_CANCEL = -1,
    emKE_CONFIG_ENVIRONMENT = 0,
    emKE_EXPORT_MASTER = 1,
    emKE_EXPORTSLAVE = 2,
    emKE_MERGE = 3,
    emKE_UPDATE_LOGSERVER = 4,
    emKE_EXECUTE_ALL = 100        //执行全部流程
};

enum KE_GROUP_MERGE_ACTION_STATUS
{
    emKE_PROCESSING = 1,
    emKE_ERROR = 2,
    emKE_DONE = 4
};

struct e2g_updateserver : public KProtocolHead
{
	char			szFile[128];		// 更新包的文件名（不带路径）
	char			szCode[33];			// 文件MD5码
	unsigned char	byMode;				// 0=更新，1＝仅MD5校验
};

struct g2e_updateserver : public KProtocolHead
{
	unsigned short	nCurrentStep;	// 当前步骤（从1开始）
	unsigned short	nStepCount;		// 总步骤数
	unsigned short	nResult;		// 处理结果，1为成功，0为失败
	unsigned short	nMsgLen;		// 附带消息长度（可以为0）
};

struct c2r_gmcmd : public KProtocolHead
{
	unsigned short nCmdLen;			// 命令长度
};

struct r2c_gmcmd : public KProtocolHead
{
	char Command[6144];
};

enum KE_GMCMD_ASKER
{
	emKE_eyes,
	emKE_gmc,
	emKE_game,
	emKE_none
};

struct e2l_gmcmd : public KSubProtocolHead
{

	int				nSession;		// 会话ID
	unsigned char	byFlag;			// -1：不绑定玩家，1：szName表示帐号名，0：szName表示角色名 2：群发全服
	unsigned char	byAction;		// 0表示操作，1表示数据
	unsigned char	byDest;			// 操作的目标服务器,0表示GC,1表示GS
	unsigned char	byAsker;
	char			szName[32];		// 帐号名/角色名
	int				nCmdLen;		// 要执行命令的长度
};

struct e2l_customercmd : public KSubProtocolHead
{
	int				nSession;		// 会话ID
	unsigned char	byAction;		// 0表示操作，1表示数据
	unsigned char	byAsker;
	char			szCmdKey[32];	// 指令名字
	char			szName[32];		// 帐号名/角色名
	int				nDataLne;		// 数据长度
};

struct e2l_gmmapcmd : public KSubProtocolHead
{
	int		nSession;			// 会话ID
	unsigned char	byAsker;			// 
	char			szCmdKey[64];		// 指令名字
	unsigned short	nParamLength;		//  参数列表长度
};

struct e2l_queryrolelist : public KSubProtocolHead
{
	int				nSession;					// 会话
	char			szAccount[emKEYE_NAMELEN];	// 帐号名
};

struct l2e_rolelist : public KProtocolHead
{
	int				nSession;					// 会话
	int				nRoleInfoLen;				// 角色信息长度
	char			szRoleInfo[1];				// json格式的变长角色信息文本
};

enum KE_GMCMD_RESULT
{
	emKGMCMD_RESULT_SUCCESS,			// 成功
	emKGMCMD_RESULT_FAILURE,			// 失败
	emKGMCMD_RESULT_DISPFAIL,			// 请求无法被发送，可能的原因例如目标区服不存在/未连上
	emKGMCMD_RESULT_TIMEOUT,			// 超时
	emKGMCMD_RESULT_NONE,				// （查询的）目标不存在
	emKGMCMD_RESULT_NOTALLOW,			// 操作不被许可
	emKGMCMD_RESULT_WRONGCMD,			// 命令错误
	emKGMCMD_RESULT_PLAYER_NOT_FOUND,	// 玩家不存在
	emKGMCMD_RESULT_PLAYER_NOT_ONLINE,	// 玩家不在线
	emKGMCMD_RESULT_COUNT
};

struct l2e_gmcmd : public KProtocolHead
{
	int				nSession;		// 会话ID
	unsigned char	byAction;		// 0表示操作，1表示数据
	unsigned char	byResult;		// 操作结果，见KE_GMCMD_RESULT
	unsigned char	byAsker;		// 0表示fseye 1表示gmc
	int				nLen;			// 返回字符串长度
};

//为了将KE游戏指令相关的内容从gms中分离出来
typedef l2e_gmcmd l2g_gamescript_ke;

// GM频道信息
struct l2e_gmchn : public KProtocolHead
{
	char	szAccount[32];
	char	szRole[32];
	int		nMsgLen;
};

// python脚本给guard发送的日志协议（变长协议）
struct p2g_log : public KProtocolHead
{
	unsigned short	wLen;	// 消息长度
};

// 各个服务器对应的serverid，如果没有对应serverid则为0
struct e2g_serverid : public KProtocolHead
{
	int	aryServerId[emKGUARDSERVER_COUNT];	// 见KE_GUARDSERVER@kipcstream.h
};

struct g2e_serverpid : public KProtocolHead
{
	int nPid;				// 进程ID，0表示没有初始化，-1表示进程结束，>0表示正常
};

struct KPROTOCOL_L2G_PIDRET : public KSubProtocolHead
{
	int	nPid;		// 进程ID
};

struct e2g_shellscript : public KProtocolHead
{
	int nSessionId;
	int bCN;
	int bNeedOutput;
	int nScriptLen;
	int nInputLen;
	//此后先是长度为nScriptLen字节的szScript
	//然后是长度为nInputLen字节的szInput
};

struct g2e_shellscript : public KProtocolHead
{
	int nSessionId;
	int bDone;
	int nOutputLen;
	//此后是长度为nOutputLen字节的szOutput
};

//==================== 文件传输 (新版本) ========================

struct FileTransferProtocol : public KProtocolHead
{
    int nSessionId;
};

struct e2g_getfile_begin : public FileTransferProtocol
{
	int nPathLen;
	//此后是长度为nPathLen字节的szPath
};

struct g2e_getfile_begin : public FileTransferProtocol
{
	int nSuccess;
    INT64 i64TotalSize;
    int nErrorMessageLen;
	//此后是长度为nErrorMessageLen字节的szErrorMessage
};

struct e2g_getfile_data : public FileTransferProtocol
{
    int nBlockCount;
};

struct g2e_getfile_data : public FileTransferProtocol
{
    int nDataLen;
	//此后是长度为nDataLen字节的szData
};

struct e2g_putfile_begin : public FileTransferProtocol
{
    INT64 i64Size;
    int nPathLen;
	//此后是长度为nPathLen字节的szPath
};

struct g2e_putfile_begin : public FileTransferProtocol
{
    int nSuccess;
    int nErrorMessageLen;
    //此后是长度为nErrorMessageLen字节的szErrorMessage
};

//KE向guard发送数据
struct e2g_putfile_data : public g2e_getfile_data
{
};

//guard向KE发送数据接收ACK
struct g2e_putfile_data : public FileTransferProtocol
{
};

struct e2g_file_done : public FileTransferProtocol
{
};

struct g2e_file_done : public FileTransferProtocol
{
};

struct e2g_file_cancel : public FileTransferProtocol
{
    int nCancelMessageLen;
    //此后是长度为nCancelMessageLen字节的szCancelMessage
};

struct g2e_file_cancel : public e2g_file_cancel
{
};

//==================== 文件传输 (新版本) END ====================

struct gc2gc_gamedata : public KProtocolHead
{
	char szTo[32];	//目标gamecenter所在组的GateWayName
					//若为空字符串则表示要广播全区全服
	
	int nDataLen;	//数据为变长

	char szData[1]; //长度为nDataLen字节的szData
};

// from jx3 [7/5/2011 simon]
struct l2e_update_custom_info : public KSubProtocolHead
{
	size_t uDataLen;
	BYTE   byData[1];
};

enum ProtocolDef {
	e2l_header_def = 0,
	l2e_header_def,
	g2e_ping_def,
	e2g_ping_def,
	e2g_openfile_def,
	g2e_openfile_def,
	e2g_readfile_def,
	g2e_readfile_def,
	e2g_writefile_def,
	g2e_writefile_def,
	e2g_seekfile_def,
	g2e_seekfile_def,
	e2g_closefile_def,
	g2e_closefile_def,
	e2g_getcpubaseinfo_def,
	g2e_getcpubaseinfo_def,
	e2g_getcpuload_def,
	g2e_getcpuload_def,
	e2g_getmeminfo_def,
	g2e_getmeminfo_def,
	e2g_getdiskinfo_def,
	g2e_getdiskinfo_def,
	e2g_getnetbaseinfo_def,
	g2e_getnetinfo_def,
	e2g_getcardload_def,
	g2e_getcardload_def,
	e2g_getprocinfo_def,
	g2e_getprocinfo_def,
	e2g_getpluginfo_def,
	g2e_getpluginfo_def,
	e2g_exesql_def,
	g2e_exesql_def,
	e2g_exesyscmd_def,
	g2e_exesyscmd_def,
	e2l_SayToWorld_def,
	e2l_GetBasicInfo_def,
	l2e_GetBasicInfo_def,
	e2l_ExeGMCmd_def,				// KE向游戏服务器发送游戏指令
	l2e_ExeGMCmd_def,				// 游戏服务器返回指令的执行结果给KE
	e2g_Authentication_def,
	g2e_Authentication_def,
	e2l_PlayerCount_def,
	l2e_PlayerCount_def,
	e2g_GetGuardDir_def,
	g2e_GetGuardDir_def,
	e2g_UpdateGuard_def,
	e2l_Who_def,
	l2e_Who_def,
	e2l_GetGlobalVariable_def,
	l2e_GetGlobalVariable_def,
	e2l_SetGlobalVariable_def,
	e2g_config_def,
	e2g_file_def,
	g2e_config_def,
	e2g_switchmode_def,
	g2e_switchmode_def,
	e2l_GetGameStartTime_def,
	l2e_GetGameStartTime_def,
	l2e_ReportError_def,
	e2g_DeliverKey_def,
	e2g_exesyscmd_large_def,
	l2e_info_def,
	l2e_info_large_def,
	e2l_exit_def,					// 命令游戏服务器退出
	e2l_getpid_def,					// 获取进程ID，struct: KProtocolHead
	l2e_getpid_def,					// 获取进程ID，struct: l2e_getpid
	l2e_ping_def,					// server发给web的ping
	e2g_exesql2_def,				// 执行SQL语句：如果第一个SQL语句执行不成功则执行第二个SQL语句
	e2g_updateserver_def,			// 更新服务器命令
	g2e_updateserver_def,			// 更新服务器结果
	c2r_gmcmd_def,					// 由gmc向gamecenter(relay)发送请求协议
	r2c_gmcmd_def,
	e2l_gmcmd_def,					// 执行GM命令
	l2e_gmcmd_def,					// 游戏返回GM命令结果
	l2e_gmchn_def,					// GM频道信息
	e2l_queryrolelist_def,			// 查询角色列表
	l2e_rolelist_def,				// 角色列表
	e2g_ftpdownload_def,			// 下载更新包命令
	g2e_ftpdownload_def,			// 下载更新包结果
	e2g_updateself_def,				// 更新guard
	p2g_log_def,					// python脚本给guard发送的日志协议
	e2g_serverid_def,				// 各个服务器对应的servrerid，协议为e2g_serverid
	g2l_header_def,					// guard给server发送的协议
	l2g_header_def,					// server给guard发送的协议
	e2g_serverpid_def,				// 请求server的进程ID，协议为KProtocolHead
	g2e_serverpid_def,				// 应答server的进程ID，协议为g2e_serverpid
	e2g_groupversion_def,			// 获取服务器组的版本号 (暂时没有使用)
    g2e_groupversion_def,			// 应答服务器组的版本号 (由updateserver.py发出，guard转发给KE)
	e2g_roleadmin_def,				// 导入导出角色数据
	g2e_roleadmin_def,				// 导入导出角色数据结果
	g2e_python_report_def,
	e2g_roleadminhistory_def,		// 导出历史备份角色数据
    g2e_roleadminhistory_def,		// 导出历史备份角色数据结果
	// end of traditional JXEX

	e2l_customercmd_def,			// 发送客服指令 (没用)
	e2g_groupmerge_def,				// 并服协议
	g2e_groupmerge_def,				// 应答并服协议
	e2g_loganalyze_def,				// 服务器log监控和分析
	g2e_loganalyze_report_def,		// 分析报告
	e2g_exemappedgmcmd_def,			// KE通过guard向游戏服务器发送Key/Params形式的游戏指令(回包还是用l2e_ExeGMCmd_def)
	e2l_mappedgmcmd_def,			// guard将e2g_exemappedgmcmd_def转换成此协议发送给游戏世界
	e2g_filetrans_upload_def,		// 上传文件
	g2e_filetrans_upload_def,		// 返回上传文件结果
	e2g_filetrans_downLoad_def,		// 下载文件
	g2e_filetrans_downLoad_def,		// 返回下载文件结果
	e2g_shellscript_def,			// guard执行操作系统的命令行脚本(新机制 2010-07-16)
	g2e_shellscript_def,			// 返回命令行脚本的执行结果

	/* 新的文件传输协议 BEGIN */
    e2g_getfile_begin_def   = 106,
    g2e_getfile_begin_def   = 107,
    e2g_getfile_data_def    = 108,
    g2e_getfile_data_def    = 109,
    e2g_putfile_begin_def   = 110,
    g2e_putfile_begin_def   = 111,
    e2g_putfile_data_def    = 112,
    g2e_putfile_data_def    = 113,
    e2g_file_done_def       = 114,
    g2e_file_done_def       = 115,
    e2g_file_cancel_def     = 116,
    g2e_file_cancel_def     = 117,
	/* 新的文件传输协议 END */

	l2g_gamescript_ke_def	= 118,	//游戏返回执行脚本指令的结果给guard(此脚本指令的来源为KE)
	gc2gc_gamedata_def		= 119,	//gamecenter之间传递游戏数据，guard和KE负责转发

	l2e_update_custom_info_def = 205,     // 客户进程向Web发送自定义显示信息 // [7/5/2011 simon]

	fseye_protocol_count,
	fseye_protocol_maxcount = 255	// 防止guard和插件源代码依赖
};

// server给guard发送的子协议
enum KE_PROTOCOL_L2G
{
	emKPROTOCOL_L2G_PIDRET,			// 返回进程ID
};

// guard给server发送的子协议
enum KE_PROTOCOL_G2L
{
	emKPROTOCOL_G2L_PIDREQ,			// 询问进程ID
};

enum FSEyeResult {
	fseye_success = 0,
	guard_err,
	guard_create_client_err,
	guard_startup_client_err,
	guard_client_invalidhadle,
	guard_client_send_err,
	plug_opendll_err,
	plug_getproc_err,
	plug_creat_err,
	filetran_opening_err,
	filetran_app_err,
	filetran_cre_err,
	filetran_seek_err,
	filetran_close_err,
	mydb_err_opendb,
	mydb_err_query,
	mydb_err_dbuncon,
	servicestate_stopped,
	servicestate_starting,
	servicestate_running,
	servicestate_stopping,
	servicestate_restarting,
	db_err,
	db_transaction_started_err,
	db_transaction_not_started_err,
	db_rebuild_table_err,
	db_delete_table_err,
	db_table_not_exist_err,
	db_table_exist_err,
	db_begin_transaction_err,
	db_commit_transaction_err,
	db_rollback_transaction_err,
	db_get_table_data_err,
	db_adapter_not_init_err,
	db_add_table_data_err,
	db_update_table_data_err,
	as_err,
	as_bad_argument_err,
	as_user_not_exist_err,
	as_user_already_login_err,
	as_fm_task_complete,
	sec_err,
	sec_allow,
	sec_deny,
	sec_unknown,
	sec_not_found_in_cache,
	sec_not_enough_privilege_err,
	sec_ace_already_exist_err,
	sec_ace_not_exist_err,
	sec_user_already_login_err,
	sec_user_not_exist_err,
	sec_user_not_login_err,
	l2e_ExeGMCmd_err,
	l2e_ExeGMCmd_player_not_found_err,
	g2e_ExeSysCmd_done,
	g2e_ExeSysCmd_busy,
	g2e_ExeSysCmd_result,
	mydb_more_result,
};

#pragma pack(pop)
#endif
