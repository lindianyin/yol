-- 事件枚举,对应ninesky_code\sourcecode\include\nsbase\kobjeventsetting.h 里面的定义

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER or MODULE_SKILLSIMULATOR) then
	return
end

local nId = 0;
local function IdAccumulator(nSegment)
	nId = nSegment or nId + 1;
	return nId;
end

KOBJEVENT = {
	---------------------------------------------------------------------

	-- GameWorld 事件
	emKOBJEVENTTYPE_GAMEWORLD_BEGIN								= IdAccumulator(0x00000000),
	emKOBJEVENTTYPE_GAMEWORLD_ADD_PLAYER						= IdAccumulator(),
	emKOBJEVENTTYPE_GAMEWORLD_ADD_NPC							= IdAccumulator(),
	emKOBJEVENTTYPE_GAMEWORLD_ADD_DOODAD						= IdAccumulator(),
	emKOBJEVENTTYPE_GAMEWORLD_REMOVE_PLAYER						= IdAccumulator(),
	emKOBJEVENTTYPE_GAMEWORLD_REMOVE_NPC						= IdAccumulator(),
	emKOBJEVENTTYPE_GAMEWORLD_REMOVE_DOODAD					    = IdAccumulator(),
	emKOBJEVENTTYPE_GAMEWORLD_LOST_CONNECT						= IdAccumulator(),			-- 与服务器连接丢失
	emKOBJEVENTTYPE_GAMEWORLD_SHUTDOWN							= IdAccumulator(),			-- 断开连接
	emKOBJEVENTTYPE_GC_CLIENT_MODULE_READY						= IdAccumulator(),			-- liuchang 无效，新GC不能保证这个事件到达时数据已经同步
	emKOBJEVENTTYPE_GAMEWORLD_SOCIETY_RANK_CHG					= IdAccumulator(),			-- 社会发展水平改变
	emKOBJEVENTTYPE_GAMECENTER_LOST								= IdAccumulator(),			-- 与gc模块的连接已断开
	emKOBJEVENTTYPE_GAMECENTER_CONNECT							= IdAccumulator(),			-- 已连接到gc模块

	-- Character 事件
	emKOBJEVENTTYPE_CHARACTER_BEGIN								= IdAccumulator(0x00010000),
	emKOBJEVENTTYPE_CHARACTER_BEGIN_CAST_SKILL					= IdAccumulator(),
	emKOBJEVENTTYPE_CHARACTER_END_CAST_SKILL					= IdAccumulator(),
	emKOBJEVENTTYPE_CHARACTER_DODGE								= IdAccumulator(),
	emKOBJEVENTTYPE_CHARACTER_HIT								= IdAccumulator(),			-- 命中时间
	emKOBJEVENTTYPE_CHARACTER_REACH_TARGET						= IdAccumulator(),			-- 移动 到达目标
	emKOBJEVENTTYPE_CHARACTER_DETOUR_BEGIN						= IdAccumulator(),			-- 移动中开始绕路
	emKOBJEVENTTYPE_CHARACTER_MOVE_DETOUR						= IdAccumulator(),			-- 移动中绕路
	emKOBJEVENTTYPE_CHARACTER_DETOUR_END						= IdAccumulator(),			-- 移动中结束绕路
	emKOBJEVENTTYPE_CHARACTER_MOVE_STOP							= IdAccumulator(),			-- 停止移动
	emKOBJEVENTTYPE_CHARACTER_ROUND_OVER						= IdAccumulator(),
	emKOBJEVENTTYPE_CHARACTER_ATTRIBUTE_CHANGED					= IdAccumulator(),			-- character属性改变了
	emKOBJEVENTTYPE_CHARACTER_UPDATE_ALL_ATTRIBUTE				= IdAccumulator(),			-- 更新玩家所有的属性
	emKOBJEVENTTYPE_CHARACTER_SET_DOING							= IdAccumulator(),			-- character set doing
	emKOBJEVENTTYPE_CHARACTER_KILL								= IdAccumulator(),			-- 谋杀
	emKOBJEVENTTYPE_CHARACTER_DEATH								= IdAccumulator(),			-- 死亡
	emKOBJEVENTTYPE_CHARACTER_ADDBUFFER							= IdAccumulator(),			-- 添加一个Buffer
	emKOBJEVENTTYPE_CHARACTER_REMOVEBUFFER						= IdAccumulator(),			-- 删除一个Buffer
	emKOBJEVENTTYPE_CHARACTER_REVIVE							= IdAccumulator(),			-- 某人复活了
	emKOBJEVENTTYPE_CHARACTER_EXIT_COMBAT						= IdAccumulator(),          
	emKOBJEVENTTYPE_CHARACTER_ENTER_EXPSCENE					= IdAccumulator(),
	emKOBJEVENTTYPE_CHARACTER_BEHIT                             = IdAccumulator();          -- 被命事件
	emKOBJEVENTTYPE_CHARACTER_BEDODGE							= IdAccumulator();          -- 被闪避技能事件
	emKOBJEVENTTYPE_CHARACTER_BEHITEDBYWIND                     = IdAccumulator();          -- 受到风属性攻击
	emKOBJEVENTTYPE_CHARACTER_BEHITEDBYTHUNDER			        = IdAccumulator();          -- 受到雷属性攻击
	emKOBJEVENTTYPE_CHARACTER_BEHITEDBYWATER                    = IdAccumulator();          -- 受到水属性攻击
	emKOBJEVENTTYPE_CHARACTER_BEHITEDBYFIRE                     = IdAccumulator();          -- 受到火属性攻击
	emKOBJEVENTTYPE_CHARACTER_SELFNODYING                       = IdAccumulator();          -- 自己生命高于20%
	emKOBJEVENTTYPE_CHARACTER_SELFDYING                         = IdAccumulator();          -- 自己生命不高于20%
	emKOBJEVENTTYPE_CHARACTER_SELFHEALTH                        = IdAccumulator();          -- 自身满血
	emKOBJEVENTTYPE_CHARACTER_SELFHURT                          = IdAccumulator();          -- 自身不满血
	emKOBJEVENTTYPE_CHARACTER_ENEMYDYING                        = IdAccumulator();          -- 敌方生命值不高于20%
	emKOBJEVENTTYPE_CHARACTER_SELFOUTOFMANA                     = IdAccumulator();          -- 自身魔法值不高于20
	emKOBJEVENTTYPE_CHARACTER_SELFINMANA                        = IdAccumulator();          -- 自身魔法值高于20
	emKOBJEVENTTYPE_CHARACTER_SELFBUFFERBECLEANED               = IdAccumulator();          -- 自己buffer被驱散
	emKOBJEVENTTYPE_CHARACTER_ENTER_COMBAT						= IdAccumulator();          -- character进入战场
	emKOBJEVENTTYPE_CHARACTER_PLAYSFX                           = IdAccumulator();          -- 播放特效
	emKOBJEVENTTYPE_CHARACTER_EFFECTRECEIVERLIST                = IdAccumulator();          -- 技能作用npc列表
	emKOBJEVENTTYPE_CHARACTER_NPCRELATION_CHANGED               = IdAccumulator();          -- NPC关系改变
	emKOBJEVENTTYPE_CHARACTER_REBOUND                           = IdAccumulator();          -- 反击事件
	emKOBJEVENTTYPE_CHARACTER_FIERCE                            = IdAccumulator();          -- npc狂暴事件
	emKOBJEVENTTYPE_CHARACTER_MOVE							    = IdAccumulator();          -- 角色移动
	emKOBJEVENTTYPE_CHARACTER_ENTER_TRAP					    = IdAccumulator();          -- 进入trap
	emKOBJEVENTTYPE_CHARACTER_LEAVE_TRAP					    = IdAccumulator();          -- 离开trap
	emKOBJEVENTTYPE_CHARACTER_START_AUTOPATH					= IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_END_AUTOPATH						= IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_FINISH_AUTOPATH					= IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_DAMAGE                            = IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_BOSS_BLOOD_CHANGED                = IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_BORN								= IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_BUFFER_NOT_HIT                    = IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_BUFFER_IMMUNITY                   = IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_BUFFER_HIT                        = IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_LIFE_LESS_THAN                    = IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_MANA_LESS_THAN                    = IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_KNOCK								= IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_PULL								= IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_LEAVE_SCENE					    = IdAccumulator();
	emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE				        = IdAccumulator();
	emKOBJEVENTTYPE_OFFLINE_FIGHT_EXIT						    = IdAccumulator();

	-- Player 事件
	emKOBJEVENTTYPE_PLAYER_BEGIN								= IdAccumulator(0x00020000),
	emKOBJEVENTTYPE_PLAYER_PK_EXERCISE_INVITE					= IdAccumulator(),			--接受切磋邀请
	emKOBJEVENTTYPE_PLAYER_TEAM_RECEIVE_INVITE					= IdAccumulator(),		--接受入队邀请
	emKOBJEVENTTYPE_PLAYER_TEAM_APPLY_ADD						= IdAccumulator(),		--接受申请入队
	emKOBJEVENTTYPE_PLAYER_LOGIN								= IdAccumulator(),		-- 某人上线了
	emKOBJEVENTTYPE_PLAYER_LOGOUT								= IdAccumulator(),		-- 某人下线了
	emKOBJEVENTTYPE_PLAYER_MSG_ARRIVE							= IdAccumulator(),		-- 聊天消息到达
	emKOBJEVENTTYPE_PLAYER_CAST_NIRVANA                         = IdAccumulator(),      -- 释放必杀技能
	emKOBJEVENTTYPE_PLAYER_TELEPORT								= IdAccumulator(),		-- 某人执行了传送
	emKOBJEVENTTYPE_PLAYER_OPEN_DOODAD							= IdAccumulator(),		-- 某人打开了doodad
	emKOBJEVENTTYPE_PLAYER_ITEM_OPS								= IdAccumulator(),		-- 道具操作
	emKOBJEVENTTYPE_PLAYER_EQUIP_OPS							= IdAccumulator(),		-- 装备操作

	---------------------------------------------------------------------
	-- 场景事件
	emKOBJEVENTTYPE_SCENE_BEGIN									= IdAccumulator(0x00030000),
	emKOBJEVENTTYPE_SCENE_CREATE								= IdAccumulator(),			-- 创建场景
	emKOBJEVENTTYPE_SCENE_DELETE								= IdAccumulator(),			-- 删除场景
	emKOBJEVENTTYPE_SCENE_RECYCLE								= IdAccumulator(),			-- 副本回收
	emKOBJEVENTTYPE_SCENE_LOAD_FINISH                           = IdAccumulator(),          -- 场景加载完成                             
	
	---------------------------------------------------------------------
	-- Doodad事件
	emKOBJEVENTTYPE_DOODAD_BEGIN							    = IdAccumulator(0x00035000),
	emKOBJEVENTTYPE_DOODAD_ENTER_SCENE							= IdAccumulator(),
	emKOBJEVENTTYPE_DOODAD_LEAVE_SCENE							= IdAccumulator(),
	emKOBJEVENTTYPE_DOODAD_STATE_CHANGED						= IdAccumulator(),
	
	---------------------------------------------------------------------
	-- Combat 事件
	emKOBJEVENTTYPE_COMBAT_BEGIN								= IdAccumulator(0x00040000),
	emKOBJEVENTTYPE_COMBAT_SLIPCELL								= IdAccumulator(),			-- 格子坐标变换，给UI用，获取要显示的格子列表
	emKOBJEVENTTYPE_COMBAT_CONFIRMCELL							= IdAccumulator(),			-- 格子坐标确定，给UI用
	emKOBJEVENTTYPE_COMBAT_SLIPSEAT								= IdAccumulator(),			-- 滑过初始站位
	emKOBJEVENTTYPE_COMBAT_CONFIRMSEAT							= IdAccumulator(),			-- 选中初始站位	
	emKOBJEVENTTYPE_COMBAT_SETSEAT								= IdAccumulator(),			-- 开始选择初始站位
	emKOBJEVENTTYPE_COMBAT_ROUNDBEGIN							= IdAccumulator(),			-- 回合开始	
	emKOBJEVENTTYPE_COMBAT_ROUNDEND								= IdAccumulator(),			-- 回合结束
	emKOBJEVENTTYPE_COMBAT_SET_OP								= IdAccumulator(),			-- 选择动作
	emKOBJEVENTTYPE_COMBAT_SET_OP_DONE							= IdAccumulator(),			-- 选择动作结束
	emKOBJEVENTTYPE_COMBAT_PLAY_ACTION							= IdAccumulator(),			-- 开始播放动作
	emKOBJEVENTTYPE_COMBAT_FINISH								= IdAccumulator(),			-- 战斗结束
	emKOBJEVENTTYPE_COMBAT_ACCEPTEDSEAT							= IdAccumulator(),			-- 收到服务端站位确认消息
	emKOBJEVENTTYPE_COMBAT_EXEC_ACTION_DONE						= IdAccumulator(),			-- 服务端完成了动作执行
	emKOBJEVENTTYPE_COMBAT_PLAY_ACTION_DONE						= IdAccumulator(),			-- 播放动作结束，发给服务端
	emKOBJEVENTTYPE_COMBAT_START								= IdAccumulator(),			-- 战斗开始
	emKOBJEVENTTYPE_COMBAT_CANCELSELCELL						= IdAccumulator(),			-- 取消选择格子
	emKOBJEVENTTYPE_COMBAT_INIT									= IdAccumulator(),			-- 战斗初始化，用于UI显示
	emKOBJEVENTTYPE_COMBAT_CHARACTER_EXIT						= IdAccumulator(),			-- 某人退出了战斗
	emKOBJEVENTTYPE_COMBAT_OPENMOVING							= IdAccumulator(),			-- 开场动画
	emKOBJEVENTTYPE_COMBAT_OPENMOVING_DONE						= IdAccumulator(),			-- 开场动画结束
	emKOBJEVENTTYPE_COMBAT_SELECTACTION_DONE					= IdAccumulator(),			-- 选择动作完成
	emKOBJEVENTTYPE_COMBAT_SYNC_OPDONE							= IdAccumulator(),			-- 队友选择操作完成
	emKOBJEVENTTYPE_COMBAT_PLAY_STATE_DONE						= IdAccumulator(),			-- 战场状态表现完成
	emKOBJEVENTTYPE_COMBAT_PREROUNDEND							= IdAccumulator(),			-- 回合快结束
	emKOBJEVENTTYPE_COMBAT_ACTIONS                              = IdAccumulator(),          -- 选择的动作列表
	emKOBJEVENTTYPE_COMBAT_DAMAGESHOW							= IdAccumulator(),          -- 表现伤害显示
	emKOBJEVENTTYPE_COMBAT_LEAVE_BATTLE                         = IdAccumulator(),          -- 离开战场
	emKOBJEVENTTYPE_COMBAT_ONE_SET_OP_DONE                      = IdAccumulator();          -- 某人选择动作结束
	emKOBJEVENTTYPE_COMBAT_AUTO_FIGHT                           = IdAccumulator();          -- 自动战斗状态改变
	emKOBJEVENTTYPE_COMBAT_BEGIN_ROUND_ACTION                   = IdAccumulator();          -- 开始一个回合动作
	emKOBJEVENTTYPE_COMBAT_END_ROUND_ACTION                     = IdAccumulator();          -- 结束一个回合动作
	emKOBJEVENTTYPE_COMBAT_CHARACTER_ATTRIBUTE_CHANGE           = IdAccumulator();          -- 战场人物属性改变
	---------------------------------------------------------------------
	-- 魔法事件
	emKOBJEVENTTYPE_MAGIC_BEGIN									= IdAccumulator(0x00050000),
	emKOBJEVENTTYPE_MAGIC_REVIVE								= IdAccumulator(),								
	emKOBJEVENTTYPE_MAGIC_SLEEP									= IdAccumulator(),					
	emKOBJEVENTTYPE_MAGIC_CHANGE_PHYSICS_ATTACK_POWER_BASE		= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_PHYSICS_ATTACK_POWER_PAC		= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_MAGIC_ATTACK_POWER_BASE		= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_MAGIC_ATTACK_POWER_PAC			= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_PHYSICS_DEFENCE_BASE			= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_PHYSICS_DEFENCE_PAC			= IdAccumulator(),					
	emKOBJEVENTTYPE_MAGIC_CHANGE_MAGIC_DEFENCE_BASE				= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_MAGIC_DEFENCE_PAC				= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_HIT_BASE						= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_HIT_PAC						= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_DODGE_BASE						= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_DODGE_PAC						= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_CRITICAL_BASE					= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_CRITICAL_PAC					= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_DECRITICAL_BASE				= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_DECRITICAL_PAC					= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_MAXLIFE_BASE					= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_MAXLIFE_PAC					= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_MAXMANA						= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_DAMAGE							= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_HEAL							= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_SHACKLE							= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_PETRIFY							= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_DEATH							= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_CONFUSION						= IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_REMOVEBUFFER					= IdAccumulator(),
	
	-- add by panhui bengin
	emKOBJEVENTTYPE_MAGIC_CAUSE_CAST_SKILL                      = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_IMMUNE_DEATH                    = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_SILENCE                         = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_CHANGETARGETTOSELF              = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_CHANGETARGET                    = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_DAMAGETRANSER                   = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_ACTIONLOCATION                 = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_TRANSFERDAMAGETOMANA            = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_EXCHANGECURRENTLIFE             = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_TRANSFERDAMAGETOHEAL            = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_TRANSFERHEALTODAMAGE            = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_TRANSFERLIFETOMANA              = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_ACTIONSPEED                    = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_RATINGDAMAGE                    = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_RATINGHEAL                      = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_HEALEFFECTPAC                  = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_BEHEALEFFECTPAC                = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_DAMAGEEFFECTPAC                = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_BEDAMAGEEFFECTPAC              = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_CHARM                           = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_DAMAGECLASSPAC                 = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_BEDAMAGECLASSPAC               = IdAccumulator(),	
	emKOBJEVENTTYPE_MAGIC_CAUSE_CURRENTLIFEADD                  = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_CURRENTLIFEPACADD               = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_MANAADD                         = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_STATICINF                       = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_POTIONEFFECTPAC                = IdAccumulator(),
	-- end
	
	-- add by panhui begin
    emKOBJEVENTTYPE_MAGIC_CAUSE_REBOUNDWITHWHOLEDAMAGE          = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_REBOUNDWITHPARTDAMAGE           = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_IMMUNITY                        = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_BUFFER_EFFECTTIME              = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_CRITMULTIPLEPAC                = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_COUNTER                         = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_NODEATH                         = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CHANGE_SKILLCOSTMPPAC                 = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_SKILLEXTRAEFFECT                = IdAccumulator(),
	emKOBJEVENTTYPE_MAGIC_CAUSE_STEALBUFFER                     = IdAccumulator();
	emKOBJEVENTTYPE_MAGIC_CAUSE_DIRECTDAMAGE                    = IdAccumulator();
	emKOBJEVENTTYPE_MAGIC_CAUSE_LIFESTEAL                       = IdAccumulator();
	emKOBJEVENTTYPE_MAGIC_CHANGE_BUFFERHIT                      = IdAccumulator();
	emKOBJEVENTTYPE_MAGIC_CHANGE_BUFFERBEHIT                    = IdAccumulator();
	emKOBJEVENTTYPE_MAGIC_CAUSE_PLAYSFX                         = IdAccumulator();
	emKOBJEVENTTYPE_MAGIC_CAUSE_NIRVANA                         = IdAccumulator();
	emKOBJEVENTTYPE_MAGIC_CAUSE_FIERCE                          = IdAccumulator();
	-- end
	
	emKOBJEVENTTYPE_MAGIC_END									= IdAccumulator(),
	
	------------------------------------------------------------------------
	emKOBJEVENTTYPE_ITEM_ADD									= IdAccumulator(0x00060000),
	emKOBJEVENTTYPE_ITEM_REMOVE									= IdAccumulator(),
	emKOBJEVENTTYPE_RECIPE_ADD									= IdAccumulator(),
	emKOBJEVENTTYPE_RECIPE_REMOVE								= IdAccumulator(),
	emKOBJEVENTTYPE_RECIPE_SYNCLISTREADY						= IdAccumulator(),
	emKOBJEVENTTYPE_COMPOSE_RESULT								= IdAccumulator(),
	emKOBJEVENTTYPE_ITEM_PROP_MODIFY							= IdAccumulator(),
	emKOBJEVENTTYPE_ITEM_USE									= IdAccumulator(),
	emKOBJEVENTTYPE_ITEM_TRANSFORM								= IdAccumulator(),
	emKOBJEVENTTYPE_ITEM_TRANSFORMSELECT						= IdAccumulator(),
	emKOBJEVENTTYPE_ITEM_ADDSTAR								= IdAccumulator(),

	-- 表现逻辑事件
	emKOBJEVENTTYPE_REPRESENT_BEGIN								= IdAccumulator(0x00070000),
	emKOBJEVENTTYPE_REPRESENT_INITGAME							= IdAccumulator(),		-- 初始化游戏场景
	emKOBJEVENTTYPE_REPRESENT_GOTO								= IdAccumulator(),		-- 走到哪
	emKOBJEVENTTYPE_REPRESENT_SELECT_MODEL						= IdAccumulator(),		-- 选中了模型
	emKOBJEVENTTYPE_REPRESENT_ENTER_MODEL						= IdAccumulator(),
	emKOBJEVENTTYPE_REPRESENT_LEAVE_MODEL						= IdAccumulator(),
	emKOBJEVENTTYPE_REPRESENT_NOTIFY_CANNTSEAT					= IdAccumulator(),		-- 通知用于，不能站位
	emKOBJEVENTTYPE_REPRESENT_SKILLANI_FINISH					= IdAccumulator(),		-- 技能动画播放结束
	emKOBJEVENTTYPE_REPRESENT_SKILLANI_READY					= IdAccumulator(),		-- 技能动画播准备完成
	emKOBJEVENTTYPE_REPRESENT_SET_CAMERATYPE					= IdAccumulator(),		-- 设置摄像机类型（用于技能动画中的操作节点）
	emKOBJEVENTTYPE_REPRESENT_SET_POSTYPE						= IdAccumulator(),		-- 设置位置类型（用于技能动画中的操作节点）
	emKOBJEVENTTYPE_REPRESENT_CAMERA_MOVE					    = IdAccumulator(),		-- 镜头拖动
	emKOBJEVENTTYPE_REPRESENT_LBUTTONDOWN						= IdAccumulator(),		-- 场景里鼠标左键按下
	emKOBJEVENTTYPE_REPRESENT_ADDMODEL							= IdAccumulator(),		-- 模型添加事件
	emKOBJEVENTTYPE_REPRESENT_DELMODEL							= IdAccumulator(),		-- 模型删除事件
	emKOBJEVENTTYPE_REPRESENT_RBTN_SELECT_MODEL 				= IdAccumulator(),		-- 右键选中了一个模型
	
	emKOBJEVENTTYPE_CHAT_CHANNEL								= IdAccumulator(0x00080000),
	emKOBJEVENTTYPE_CHAT_PLAYER_INFO_ARRIVAL					= IdAccumulator(),
	
	-- 队伍事件
	emKOBJEVENTTYPE_TEAM										= IdAccumulator(0x00090000),
	emKOBJEVENTTYPE_LOGIN_TEAM                                       = IdAccumulator(),
	emKOBJEVENTTYPE_LOGOUT_TEAM                                      = IdAccumulator(),
	emKOBJEVENTTYPE_CREATED_TEAM								= IdAccumulator(),		--自己创建队伍
	emKOBJEVENTTYPE_ADD_TEAM									= IdAccumulator(),		--加入别人队伍
	emKOBJEVENTTYPE_LEAVE_TEAM								    = IdAccumulator(),		--离开队伍
	emKOBJEVENTTYPE_DISBAND_TEAM								= IdAccumulator(),		--解散队伍(目前仅GS端有)
	emKOBJEVENTTYPE_CAPTAIN_CHANGE								= IdAccumulator(),		--队长移交(仅GS有 nTeamID, NewCaptainID)
	emKOBJEVENTTYPE_TEAM_MEMBER_CHANGED					        = IdAccumulator(),
	emKOBJEVENTTYPE_TEAM_SEARCH_INFO							= IdAccumulator(),
	emKOBJEVENTTYPE_SELF_SEARCH_INFO							= IdAccumulator(),
   	emKOBJEVENTTYPE_TEAM_DISABLE_STATE						    = IdAccumulator(),
   	emKOBJEVENTTYPE_TEAM_GROUP_CHANGED						    = IdAccumulator(),
   	emKOBJEVENTTYPE_TEAM_NEAR_BY_TEAM_UPDATED				    = IdAccumulator(),	--周围队伍数据更新
   	emKOBJEVENTTYPE_SUMMON_MEMBER							    = IdAccumulator(),  --队长召唤队员
   	emKOBJEVENTTYPE_REPLY_SUMMON							    = IdAccumulator(),  --队员对召唤的回复
   	emKOBJEVENTTYPE_TEAM_NAME_UPDATE						    = IdAccumulator(),  --队伍名称更新
   	emKOBJEVENTTYPE_FOLLOW_STATE_CHG						    = IdAccumulator(),  --跟随状态改变
   	emKOBJEVENTTYPE_TEAM_STATE_CHG							    = IdAccumulator(),	--队伍状态改变
	
	-- 任务事件
	emKOBJEVENTTYPE_TASK_BEGIN									= IdAccumulator(0x000A0000),
	emKOBJEVENTTYPE_TASK_ACCEPT									= IdAccumulator(),	--接受任务（包括主动接受和被共享接受）
	emKOBJEVENTTYPE_TASK_SHARE									= IdAccumulator(),	--共享任务给别人(todo 暂未支持)
	emKOBJEVENTTYPE_TASK_GIVEUP									= IdAccumulator(),	--放弃任务
	emKOBJEVENTTYPE_TASK_FINISH									= IdAccumulator(),	--完成任务
	emKOBJEVENTTYPE_TASK_RECOMMEND_REFRESH						= IdAccumulator(),	--推荐任务刷新
	emKOBJEVENTTYPE_TASK_SYNCDATAFINISHED						= IdAccumulator(),
	emKOBJEVENTTYPE_TASK_COMPLETE                               = IdAccumulator(),  --任务进度完成(未交) 
	emKOBJEVENTTYPE_TASK_END									= IdAccumulator(),
	
	--经济系统事件
	emKOBJEVENTTYPE_SHOP_BEGIN									= IdAccumulator(0x000B0000),
	emKOBJEVENTTYPE_SHOP_OPENSHOP								= IdAccumulator(),
	emKOBJEVENTTYPE_SHOP_CLOSESHOP								= IdAccumulator(),
	emKOBJEVENTTYPE_SHOP_SYNCSHOPDATAFINISH						= IdAccumulator(),
	
	emKOBJEVENTTYPE_IBSHOP_OPENWINDOW							= IdAccumulator(),
	emKOBJEVENTTYPE_IBSHOP_CLOSEWINDOW							= IdAccumulator(),
	
	--钱袋事件
	emKOBJEVENTTYPE_PURSE_BEGIN									= IdAccumulator(0x000C0000),
	emKOBJEVENTTYPE_PURSE_CHANGECURRENCY						= IdAccumulator(),
	emKOBJEVENTTYPE_PURSE_CHANGECOIN							= IdAccumulator(),
	emKOBJEVENTTYPE_PURSE_CHANGEMONEY							= IdAccumulator(),
	
	-- 职业事件
	emKOBJEVENTTYPE_FACTION_BEGIN								= IdAccumulator(0x000D0000),
	emKOBJEVENTTYPE_FACTION_BINDSKILL_CHANGED					= IdAccumulator(),		--服务器端发来绑定技能变化
	emKOBJEVENTTYPE_FACTION_EXP_CHANGED							= IdAccumulator(),		--服务器端发来职业经验变化
	emKOBJEVENTTYPE_FACTION_LEVEL_CHANGED						= IdAccumulator(),		--服务器端发来职业等级变化
	emKOBJEVENTTYPE_FACTION_SKILL_POWER_CHANGED					= IdAccumulator(),		--服务器端发来职业秘笈点数变化
	
	emKOBJEVENTTYPE_GROUP_BEGIN									= IdAccumulator(0x000E0000),
	emKOBJEVENTTYPE_GROUP_CREATE_GROUP							= IdAccumulator(),
	emKOBJEVENTTYPE_GROUP_DELETE_GROUP							= IdAccumulator(),
	emKOBJEVENTTYPE_GROUP_ADD_MEMBER							= IdAccumulator(),
	emKOBJEVENTTYPE_GROUP_REMOVE_MEMBER							= IdAccumulator(),
	emKOBJEVENTTYPE_GROUP_JOIN_REQUISITION						= IdAccumulator(),
	emKOBJEVENTTYPE_GROUP_ERROR_INFO							= IdAccumulator(),
	emKOBJEVENTTYPE_GROUP_GROUP_INFO							= IdAccumulator(),
	
	-- 剧情事件
	emKOBJEVENTTYPE_PLOT_PLAY_START                             = IdAccumulator(0x000F0000),
	emKOBJEVENTTYPE_PLOT_PLAY_FINISH                            = IdAccumulator(),
	
	
	--	全局变量
	emKOBJEVENTTYPE_GOLBALDATA_BEGIN							= IdAccumulator(0x00100000),
	emKOBJEVENTTYPE_GLOBALDATA_INTVALUECHANGE					= IdAccumulator(),
	emKOBJEVENTTYPE_GLOBALDATA_BINVALUECHANGE					= IdAccumulator(),
	emKOBJEVENTTYPE_GLOBALDATA_DATA_SYNC_FINISH					= IdAccumulator(),
	
	-- Story故事
	emKOBJEVENTTYPE_STORY_BEGIN									= IdAccumulator(0x00110000),
	emKOBJEVENTTYPE_STORY_FINISH								= IdAccumulator(),
	emKOBJEVENTTYPE_STORY_PROGRESS_FINISH						= IdAccumulator(),
	
	-- ExpScene副本/探索地图
	emKOBJEVENTTYPE_EXPSCENE_BEGIN								= IdAccumulator(0x00120000),
	emKOBJEVENTTYPE_EXPSCENE_FINISH								= IdAccumulator(),
	emKOBJEVENTTYPE_EXPSCENE_CHANGE								= IdAccumulator(),
	emKOBJEVENTTYPE_EXPSCENE_UPDATE_STAR						= IdAccumulator(),
	
	--联赛事件
	emKOBJEVENTTYPE_LEAGUE_INFO_REACH							= IdAccumulator(0x00130000),	--联赛信息到达客户端
	emKOBJEVENTTYPE_LEAGUE_PAGE_REACH							= IdAccumulator(),				--联赛排名数据到达客户端
	emKOBJEVENTTYPE_LEAGUE_QUEUE_FLAG_CHG						= IdAccumulator(),				--通知UI联赛报名标志改变
	
	emKOBJEVENTTYPE_RELATION_LIST_REACH							= IdAccumulator(0x00140000),	--人际关系列表到达客户端
	emKOBJEVENTTYPE_RELATION_INFO_REACH							= IdAccumulator(),				--人际关系玩家详细信息
	emKOBJEVENTTYPE_RELATION_SIGNATURE_CHG						= IdAccumulator(),				--自己签名已修改
	emKOBJEVENTTYPE_RELATION_MOOD_CHG							= IdAccumulator(),				--自己心情已修改
	emKOBJEVENTTYPE_RELATION_REMOVE								= IdAccumulator(),				--关系已解除
	emKOBJEVENTTYPE_RELATION_ADD								= IdAccumulator(),				--关系已建立
	emKOBJEVENTTYPE_RELATION_ONLINE_CHG							= IdAccumulator(),				--好友上下线
	
	--宠物事件
	emKOBJEVENTTYPE_SUB_ADDSUB									= IdAccumulator(0x00150000),
	emKOBJEVENTTYPE_SUB_DELSUB									= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_RESPONSE_SUBDATA						= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_STARTWORK								= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_STOPWORK								= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_AWARD_INFORM							= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_POSSESS									= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_UNPOSSESS								= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_ACCEPT_AWARD_BYSUB						= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_ACCEPT_AWARD							= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_MUDS_ONESUB								= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_MUDS_ALLSUB								= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_PROLONGWORK							    = IdAccumulator(),
	emKOBJEVENTTYPE_SUB_MAINSUB_CHANAGED						= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_FOLLOWSUB_CHANGED						= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_UPDATE_ADDPOINT							= IdAccumulator(),
	emKOBJEVENTTYPE_SUB_PROP_CHANGED							= IdAccumulator(),
	
	-- 家族事件
	emKOBJEVENTTYPE_KIN_CREATE									= IdAccumulator(0x00160000),
	emKOBJEVENTTYPE_KIN_DESTROY									= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_DETECTDESTROY							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_INVITE									= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_REFUSEINVITE							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPLYADDKIN								= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPROVEADDKIN							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_KICKOUT									= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_QUIT									= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_TRYCHANGEGROUP							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_CHANGEGROUP								= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_CHANGEFIGURE							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPLYKININFO							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPLYKININFOEX							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPLYKINMEMBERINFO						= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPLYKINAPPLYLIST						= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_CHANGEKININFO							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_GETKINID								= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_LOGIN									= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_LOGOUT									= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_KININFOCHANGE							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_TRIALRESULT								= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_GROUPRESULT								= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPLYKINWORKLIST						= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPLYKINWORKVOTEINFO					= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_VOTE									= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_ADDVOTEOPTION							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_DOWORK									= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_DOWORKRESULT							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_ADDDEVOTE								= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_APPLYDEVOTEINFO							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_FUNCCHANGE								= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_CANCELCAPTAINQUIT						= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_CHANGERECRUIT							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_SEARCHRECRUIT							= IdAccumulator(),
	emKOBJEVENTTYPE_KIN_TRIALNOTIFYRESULT						= IdAccumulator(),
	
	-- 邮件事件
	emKOBJEVENTTYPE_MAIL_LOADOK									= IdAccumulator(0x00170000),
	emKOBJEVENTTYPE_MAIL_SYNCLIST								= IdAccumulator(),
	emKOBJEVENTTYPE_MAIL_NEW									= IdAccumulator(),
	emKOBJEVENTTYPE_MAIL_SENDRESULT								= IdAccumulator(),
	emKOBJEVENTTYPE_MAIL_FETCHITEMOK							= IdAccumulator(),
	emKOBJEVENTTYPE_MAIL_SYNCCONTENT							= IdAccumulator(),
	emKOBJEVENTTYPE_MAIL_NEWDELIVERY							= IdAccumulator(),

	-- 采集事件
	emKOBJEVENTTYPE_GATHER_BEGIN								= IdAccumulator(0x00180000),
	emKOBJEVENTTYPE_GATHER_READY								= IdAccumulator(),
	emKOBJEVENTTYPE_GATHER_FINISH								= IdAccumulator(),
	------------------------------------------------------------------------
	
	---Buff 事件
	emKOBJEVENTTYPE_BUFF_BUFF_IN					            = IdAccumulator(0x00190000),	--血量到达某个点
	emKOBJEVENTTYPE_BUFF_BUFF_OUT						        = IdAccumulator(),
	emKOBJEVENTTYPE_BUFF_DMG_IN									= IdAccumulator(),
	emKOBJEVENTTYPE_BUFF_DMG_OUT								= IdAccumulator(),
	emKOBJEVENTTYPE_BUFF_DEATH									= IdAccumulator(),
	emKOBJEVENTTYPE_BUFF_KILL								    = IdAccumulator(),
	emKOBJEVENTTYPE_BUFF_BUFF_PILE							    = IdAccumulator(),
	emKOBJEVENTTYPE_BUFF_CRITICAL								= IdAccumulator(),
	emKOBJEVENTTYPE_BUFF_IS_BACK								= IdAccumulator(),
	emKOBJEVENTTYPE_BUFF_MISS									= IdAccumulator(),
}
