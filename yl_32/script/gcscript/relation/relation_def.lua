-- 文件名　：relation_def.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：人际关系脚本宏定义

Relation.TASK_GROUP						= 2067;	-- 师徒相关任务group
Relation.TASK_ID_SHITU_BAIHUTANG 		= 1; -- 白虎堂
Relation.TASK_ID_SHITU_BATTLE 			= 2; -- 宋金战场
Relation.TASK_ID_SHITU_FACTION 			= 3; -- 战神无双(门派竞技)
Relation.TASK_ID_SHITU_WANTED 			= 4; -- 通缉任务
Relation.TASK_ID_SHITU_YIJUN 			= 5; -- 义军任务
Relation.TASK_ID_SHITU_CHUANGONG_COUNT 	= 6; -- 记录完家本周完成师徒传功的次数
Relation.TASK_ID_SHITU_CHUANGONG_TIME	= 7; -- 记录玩家上次传功的时间
Relation.TASK_ID_SHITU_BUFF_TIME 		= 12; -- 弟子上次领取师徒buff的日期
Relation.TASKID_LASTAPPTRAIN_DATE		= 10;	-- 上次申请拜师的日期
Relation.TASKID_APPTRAIN_COUNT			= 11;	-- 当天申请拜师的次数
Relation.MAX_APPTRAIN_COUNT				= 3;	-- 每天最多可以申请拜师的次数
Relation.STUDENT_MINILEVEL				= 20;	-- 弟子最低等级
Relation.STUDENT_MAXLEVEL				= 60;	-- 弟子最高等级
Relation.MAX_STUDENCOUNT				= 3;	-- 当前弟子的最大数目
Relation.GAPMINILEVEL					= 30;	-- 拜师时，师徒之间的最小等级差
Relation.TEACHER_NIMIEVEL				= 80;	-- 师傅的最低等级

Relation.nMax_ChuanGong_Time 	= 4;		-- 每周最多能够传功的次数


Relation.emKEADDRELATION_FAIL	= 0	-- 添加关系失败
Relation.emKEADDRELATION_SUCCESS = 1	-- 添加关系成功
Relation.emKEHAVE_RELATION		= 2	-- 已经有关系了
Relation.emKEPLAYER_NOTONLINE	= 3	-- 对方不在线的时时不能添加为好友

Relation.TIME_NOTIFYONEYEAR		= 7;	-- 在密友关系一年到期前7天，每次上线给出提示

Relation.LOG_TYPE_ADDRELATION	= 1;	-- log类型，新加人际关系
Relation.LOG_TYPE_DELRELATION	= 2;	-- log类型，删除人际关系

Relation.szInfoMsg = "";				-- 有关人际操作的返回信息


--Player.emKPLAYERRELATION_TYPE_TMPFRIEND		= 0;		-- 临时好友，单向关系，A把B加为临时好友
--Player.emKPLAYERRELATION_TYPE_BLACKLIST		= 1;		-- 黑名单，单向关系，A把B加入黑名单
--Player.emKPLAYERRELATION_TYPE_BIDFRIEND		= 2;		-- 普通好友, 对等双向关系，A和B互为好友
--Player.emKPLAYERRELATION_TYPE_SIBLING		= 3;		-- 结拜（兄弟、姐妹），对等双向关系，A和B互为结拜（兄弟、姐妹）
--Player.emKPLAYERRELATION_TYPE_ENEMEY		= 4;		-- 仇人，不对等双向关系，A曾经被B杀死
--Player.emKPLAYERRELATION_TYPE_TRAINING		= 5;		-- 师徒，不对等双向关系，A是B的师父（未出师）
--Player.emKPLAYERRELATION_TYPE_TRAINED		= 6;		-- 师徒，不对等双向关系，A是B的师父（已出师）
--Player.emKPLAYERRELATION_TYPE_COUPLE		= 7;		-- 夫妻，不对等双向关系，A是B的丈夫
--Player.emKPLAYERRELATION_TYPE_INTRODUCTION	= 8;		-- 介绍，不对等双向关系，A是B的介绍人
--Player.emKPLAYERRELATION_TYPE_BUDDY			= 9;		-- 指定密友，双向对等关系，A和B互为密友同时也互为普通好友
--Player.emKPLAYERRELATION_TYPE_COUNT			= 10;
--Player.emKPLAYERRELATION_TYPE_GLOBALFRIEND	= 100;		-- 跨服好友，此关系只用人际面板来显示，并不使用人际关系的逻辑


Relation.emKPLAYERRELATION_TYPE_TMPFRIEND		= 0;		-- 临时好友，单向关系，A把B加为临时好友
Relation.emKPLAYERRELATION_TYPE_BLACKLIST		= 1;		-- 黑名单，单向关系，A把B加入黑名单
Relation.emKPLAYERRELATION_TYPE_BIDFRIEND		= 2;		-- 普通好友, 对等双向关系，A和B互为好友
Relation.emKPLAYERRELATION_TYPE_SIBLING			= 3;		-- 结拜（兄弟、姐妹），对等双向关系，A和B互为结拜（兄弟、姐妹）
Relation.emKPLAYERRELATION_TYPE_ENEMEY			= 4;		-- 仇人，不对等双向关系，A曾经被B杀死
Relation.emKPLAYERRELATION_TYPE_TRAINING		= 5;		-- 师徒，不对等双向关系，A是B的师父（未出师）
Relation.emKPLAYERRELATION_TYPE_TRAINED			= 6;		-- 师徒，不对等双向关系，A是B的师父（已出师）
Relation.emKPLAYERRELATION_TYPE_COUPLE			= 7;		-- 夫妻，不对等双向关系，A是B的丈夫
Relation.emKPLAYERRELATION_TYPE_INTRODUCTION	= 8;		-- 介绍，不对等双向关系，A是B的介绍人
Relation.emKPLAYERRELATION_TYPE_BUDDY			= 9;		-- 指定密友，双向对等关系，A和B互为密友同时也互为普通好友
Relation.emKPLAYERRELATION_TYPE_COUNT			= 10;
Relation.emKPLAYERRELATION_TYPE_GLOBALFRIEND	= 100;		-- 跨服好友，此关系只用人际面板来显示，并不使用人际关系的逻辑

Relation.tbRelationName = {
	[Relation.emKPLAYERRELATION_TYPE_TMPFRIEND] = "好友",	-- 临时好友
	[Relation.emKPLAYERRELATION_TYPE_BLACKLIST] = "黑名单",
	[Relation.emKPLAYERRELATION_TYPE_BIDFRIEND] = "好友",
	[Relation.emKPLAYERRELATION_TYPE_SIBLING] = "结拜",
	[Relation.emKPLAYERRELATION_TYPE_ENEMEY] = "仇人",
	[Relation.emKPLAYERRELATION_TYPE_TRAINING] = "师徒",
	[Relation.emKPLAYERRELATION_TYPE_TRAINED] = "师徒",
	[Relation.emKPLAYERRELATION_TYPE_COUPLE] = "知己",
	[Relation.emKPLAYERRELATION_TYPE_INTRODUCTION] = "介绍人",
	[Relation.emKPLAYERRELATION_TYPE_BUDDY] = "指定密友",
	};
	