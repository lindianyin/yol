-- 编写者：hejingxian
-- 版本号：123

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]
    AI[ai.nAIType] = g_AIDebugInfo

    -- 注册自定义动作
    ai.RegisterUserAction(2095, 'CustomAction95')
    ai.RegisterUserAction(2127, 'CustomAction127')
    ai.RegisterUserAction(2195, 'CustomAction195')
    ai.RegisterUserAction(2491, 'AI:CustomAction491')
    ai.RegisterUserAction(2495, 'CustomAction495')
    ai.RegisterUserAction(2497, 'CustomAction497')

    -- 关卡-妖王
    state = ai.NewState(164)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 183)

    -- 记录原点
    action = ai.NewAction(183, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(44)
    g_AIDebugInfo[183] = '[183] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(44, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(523)
    g_AIDebugInfo[44] = '[44] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(523, AI_ACTION.SET_STATE)
    action.SetParam(59) -- 待机状态
    g_AIDebugInfo[523] = '[523] SetParam(State59)'

    -- 待机状态
    state = ai.NewState(59)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 162)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 64)

    -- 选择事件发起者
    action = ai.NewAction(162, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(30, 44)
    g_AIDebugInfo[162] = '[162] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(30, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(37)
    g_AIDebugInfo[30] = '[30] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(37, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(101)
    g_AIDebugInfo[37] = '[37] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(101, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(108)
    g_AIDebugInfo[101] = '[101] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(108, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(525)
    g_AIDebugInfo[108] = '[108] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(525, AI_ACTION.SET_STATE)
    action.SetParam(237) -- 战斗状态
    g_AIDebugInfo[525] = '[525] SetParam(State237)'

    -- 战斗状态
    state = ai.NewState(237)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 139)

    -- 选择第一威胁
    action = ai.NewAction(139, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(218, 44)
    g_AIDebugInfo[139] = '[139] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(218, AI_ACTION.CHECK_LIFE)
    action.SetParam(80)
    action.SetBranch(39, 39, 172)
    g_AIDebugInfo[218] = '[218] CHECK_LIFE(80)'

    -- 设置主定时器
    action = ai.NewAction(172, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(527)
    g_AIDebugInfo[172] = '[172] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(527, AI_ACTION.SET_STATE)
    action.SetParam(145) -- 普攻+范围攻
    g_AIDebugInfo[527] = '[527] SetParam(State145)'

    -- 普攻+范围攻
    state = ai.NewState(145)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 249)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 415)

    -- Npc标准技能选择
    action = ai.NewAction(249, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(56, 108)
    g_AIDebugInfo[249] = '[249] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(56, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(49, 412)
    g_AIDebugInfo[56] = '[56] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(49, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(218, 129)
    g_AIDebugInfo[49] = '[49] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(129, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(6)
    action.SetBranch(218, 415)
    g_AIDebugInfo[129] = '[129] CHECK_SKILL_FAILED_COUNTER(6)'

    -- 清除目标威胁
    action = ai.NewAction(415, AI_ACTION.CLEAR_THREAT_FOR_TARGET)
    action.SetBranch(44, 44)
    g_AIDebugInfo[415] = '[415] CLEAR_THREAT_FOR_TARGET()'

    -- 释放技能
    action = ai.NewAction(412, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(210, 129)
    g_AIDebugInfo[412] = '[412] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(210, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(529)
    g_AIDebugInfo[210] = '[210] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(529, AI_ACTION.SET_STATE)
    action.SetParam(257) -- 追击状态
    g_AIDebugInfo[529] = '[529] SetParam(State257)'

    -- 追击状态
    state = ai.NewState(257)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 504)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 415)

    -- 目标知否存在
    action = ai.NewAction(504, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(84, 139)
    g_AIDebugInfo[504] = '[504] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(84, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(44, 56, 56)
    g_AIDebugInfo[84] = '[84] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置主定时器
    action = ai.NewAction(39, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(531)
    g_AIDebugInfo[39] = '[39] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(531, AI_ACTION.SET_STATE)
    action.SetParam(176) -- 召唤鬼魂
    g_AIDebugInfo[531] = '[531] SetParam(State176)'

    -- 召唤鬼魂
    state = ai.NewState(176)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 127)

    -- NPC喊话
    action = ai.NewAction(127, 2127)
    action.SetBranch(178)
    g_AIDebugInfo[127] = '[127] CustomAction127()'

    -- 设置主定时器
    action = ai.NewAction(178, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(533)
    g_AIDebugInfo[178] = '[178] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(533, AI_ACTION.SET_STATE)
    action.SetParam(165) -- 喊话结束
    g_AIDebugInfo[533] = '[533] SetParam(State165)'

    -- 喊话结束
    state = ai.NewState(165)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 200)

    -- 释放技能
    action = ai.NewAction(200, AI_ACTION.CAST_SKILL)
    action.SetParam(1112, 1)
    action.SetBranch(491, 491)
    g_AIDebugInfo[200] = '[200] CAST_SKILL(1112, 1)'

    -- CALL NPC
    action = ai.NewAction(491, 2491)
    action.SetBranch(185)
    g_AIDebugInfo[491] = '[491] AI:CustomAction491()'

    -- 设置主定时器
    action = ai.NewAction(185, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(535)
    g_AIDebugInfo[185] = '[185] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(535, AI_ACTION.SET_STATE)
    action.SetParam(75) -- 待机一会
    g_AIDebugInfo[535] = '[535] SetParam(State75)'

    -- 待机一会
    state = ai.NewState(75)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 122)

    -- 设置主定时器
    action = ai.NewAction(122, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(537)
    g_AIDebugInfo[122] = '[122] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(537, AI_ACTION.SET_STATE)
    action.SetParam(89) -- 进入下一状态
    g_AIDebugInfo[537] = '[537] SetParam(State89)'

    -- 进入下一状态
    state = ai.NewState(89)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 242)

    -- 选择第一威胁
    action = ai.NewAction(242, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(135, 361)
    g_AIDebugInfo[242] = '[242] SELECT_FIRST_THREAT()'

    -- 站立
    action = ai.NewAction(361, AI_ACTION.STAND)
    action.SetBranch(432)
    g_AIDebugInfo[361] = '[361] STAND()'

    -- 设置主定时器
    action = ai.NewAction(432, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(539)
    g_AIDebugInfo[432] = '[432] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(539, AI_ACTION.SET_STATE)
    action.SetParam(431) -- 待机状态
    g_AIDebugInfo[539] = '[539] SetParam(State431)'

    -- 待机状态
    state = ai.NewState(431)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 358)

    -- 搜索敌人
    action = ai.NewAction(358, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(356, 443)
    g_AIDebugInfo[358] = '[358] SEARCH_ENEMY(param[AlertRange])'

    -- 释放技能
    action = ai.NewAction(443, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(361, 476)
    g_AIDebugInfo[443] = '[443] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(476, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(361, 452)
    g_AIDebugInfo[476] = '[476] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(452, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(456)
    g_AIDebugInfo[452] = '[452] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(456, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(457, 457)
    g_AIDebugInfo[456] = '[456] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(457, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(541)
    g_AIDebugInfo[457] = '[457] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(541, AI_ACTION.SET_STATE)
    action.SetParam(464) -- 无敌返回状态
    g_AIDebugInfo[541] = '[541] SetParam(State464)'

    -- 无敌返回状态
    state = ai.NewState(464)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 446)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 446)

    -- 返回
    action = ai.NewAction(446, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(445, 445)
    g_AIDebugInfo[446] = '[446] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(445, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(543)
    g_AIDebugInfo[445] = '[445] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(543, AI_ACTION.SET_STATE)
    action.SetParam(451) -- 返回成功
    g_AIDebugInfo[543] = '[543] SetParam(State451)'

    -- 返回成功
    state = ai.NewState(451)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 466)

    -- 清除DEBUFF
    action = ai.NewAction(466, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(461)
    g_AIDebugInfo[466] = '[466] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(461, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(44)
    g_AIDebugInfo[461] = '[461] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(356, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(242)
    g_AIDebugInfo[356] = '[356] ADD_TARGET_TO_THREAT_LIST()'

    -- 检查生命
    action = ai.NewAction(135, AI_ACTION.CHECK_LIFE)
    action.SetParam(50)
    action.SetBranch(95, 95, 2)
    g_AIDebugInfo[135] = '[135] CHECK_LIFE(50)'

    -- NPC喊话
    action = ai.NewAction(95, 2095)
    action.SetBranch(214)
    g_AIDebugInfo[95] = '[95] CustomAction95()'

    -- 设置主定时器
    action = ai.NewAction(214, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(545)
    g_AIDebugInfo[214] = '[214] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(545, AI_ACTION.SET_STATE)
    action.SetParam(227) -- 喊话结束
    g_AIDebugInfo[545] = '[545] SetParam(State227)'

    -- 喊话结束
    state = ai.NewState(227)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 251)

    -- 释放技能
    action = ai.NewAction(251, AI_ACTION.CAST_SKILL)
    action.SetParam(1112, 1)
    action.SetBranch(495, 495)
    g_AIDebugInfo[251] = '[251] CAST_SKILL(1112, 1)'

    -- CALL NPC
    action = ai.NewAction(495, 2495)
    action.SetBranch(499)
    g_AIDebugInfo[495] = '[495] CustomAction495()'

    -- 设置主定时器
    action = ai.NewAction(499, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(547)
    g_AIDebugInfo[499] = '[499] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(547, AI_ACTION.SET_STATE)
    action.SetParam(311) -- 待机一会
    g_AIDebugInfo[547] = '[547] SetParam(State311)'

    -- 待机一会
    state = ai.NewState(311)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 180)

    -- 设置主定时器
    action = ai.NewAction(180, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(549)
    g_AIDebugInfo[180] = '[180] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(549, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 下一状态
    g_AIDebugInfo[549] = '[549] SetParam(State10)'

    -- 下一状态
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 24)

    -- 选择第一威胁
    action = ai.NewAction(24, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(80, 366)
    g_AIDebugInfo[24] = '[24] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(80, AI_ACTION.CHECK_LIFE)
    action.SetParam(30)
    action.SetBranch(26, 26, 119)
    g_AIDebugInfo[80] = '[80] CHECK_LIFE(30)'

    -- 设置主定时器
    action = ai.NewAction(119, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(551)
    g_AIDebugInfo[119] = '[119] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(551, AI_ACTION.SET_STATE)
    action.SetParam(79) -- 普+范围+闪现范围
    g_AIDebugInfo[551] = '[551] SetParam(State79)'

    -- 普+范围+闪现范围
    state = ai.NewState(79)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 270)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 366)

    -- 双选项随机
    action = ai.NewAction(270, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(80, 20)
    action.SetBranch(283, 277)
    g_AIDebugInfo[270] = '[270] RANDOM_BI_BRANCH(80, 20)'

    -- 释放技能
    action = ai.NewAction(277, AI_ACTION.CAST_SKILL)
    action.SetParam(1113, 1)
    action.SetBranch(275, 80)
    g_AIDebugInfo[277] = '[277] CAST_SKILL(1113, 1)'

    -- 设置主定时器
    action = ai.NewAction(275, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(553)
    g_AIDebugInfo[275] = '[275] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(553, AI_ACTION.SET_STATE)
    action.SetParam(298) -- 闪现结束
    g_AIDebugInfo[553] = '[553] SetParam(State298)'

    -- 闪现结束
    state = ai.NewState(298)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 280)

    -- 释放技能
    action = ai.NewAction(280, AI_ACTION.CAST_SKILL)
    action.SetParam(1104, 1)
    action.SetBranch(296, 287)
    g_AIDebugInfo[280] = '[280] CAST_SKILL(1104, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(287, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(80, 366)
    g_AIDebugInfo[287] = '[287] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(366, AI_ACTION.STAND)
    action.SetBranch(435)
    g_AIDebugInfo[366] = '[366] STAND()'

    -- 设置主定时器
    action = ai.NewAction(435, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(555)
    g_AIDebugInfo[435] = '[435] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(555, AI_ACTION.SET_STATE)
    action.SetParam(436) -- 待机状态
    g_AIDebugInfo[555] = '[555] SetParam(State436)'

    -- 待机状态
    state = ai.NewState(436)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 368)

    -- 搜索敌人
    action = ai.NewAction(368, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(369, 469)
    g_AIDebugInfo[368] = '[368] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(369, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(24)
    g_AIDebugInfo[369] = '[369] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(469, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(366, 459)
    g_AIDebugInfo[469] = '[469] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(459, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(366, 452)
    g_AIDebugInfo[459] = '[459] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(296, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(557)
    g_AIDebugInfo[296] = '[296] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(557, AI_ACTION.SET_STATE)
    action.SetParam(281) -- 范围技能结束
    g_AIDebugInfo[557] = '[557] SetParam(State281)'

    -- 范围技能结束
    state = ai.NewState(281)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 520)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 366)

    -- 目标知否存在
    action = ai.NewAction(520, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(80, 366)
    g_AIDebugInfo[520] = '[520] IS_TARGET_EXIST()'

    -- Npc标准技能选择
    action = ai.NewAction(283, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(273, 180)
    g_AIDebugInfo[283] = '[283] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(273, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(302, 421)
    g_AIDebugInfo[273] = '[273] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(302, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(80, 287)
    g_AIDebugInfo[302] = '[302] NPC_CAST_SELECT_SKILL()'

    -- 释放技能
    action = ai.NewAction(421, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(294, 287)
    g_AIDebugInfo[421] = '[421] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(294, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(559)
    g_AIDebugInfo[294] = '[294] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(559, AI_ACTION.SET_STATE)
    action.SetParam(282) -- 追击状态
    g_AIDebugInfo[559] = '[559] SetParam(State282)'

    -- 追击状态
    state = ai.NewState(282)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 510)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 366)

    -- 目标知否存在
    action = ai.NewAction(510, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(300, 24)
    g_AIDebugInfo[510] = '[510] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(300, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(180, 273, 273)
    g_AIDebugInfo[300] = '[300] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置主定时器
    action = ai.NewAction(26, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(561)
    g_AIDebugInfo[26] = '[26] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(561, AI_ACTION.SET_STATE)
    action.SetParam(20) -- 寻路到正中间
    g_AIDebugInfo[561] = '[561] SetParam(State20)'

    -- 寻路到正中间
    state = ai.NewState(20)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 48)

    -- 进入巡逻路径
    action = ai.NewAction(48, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(182)
    g_AIDebugInfo[48] = '[48] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(182, AI_ACTION.DO_PATROL)
    action.SetBranch(255, 195)
    g_AIDebugInfo[182] = '[182] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(255, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(563)
    g_AIDebugInfo[255] = '[255] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(563, AI_ACTION.SET_STATE)
    action.SetParam(203) -- 主状态
    g_AIDebugInfo[563] = '[563] SetParam(State203)'

    -- 主状态
    state = ai.NewState(203)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 255)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 182)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 193)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 182)

    -- 设置副定时器
    action = ai.NewAction(193, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(4)
    action.SetBranch(195)
    g_AIDebugInfo[193] = '[193] SET_SECONDARY_TIMER(4)'

    -- NPC喊话
    action = ai.NewAction(195, 2195)
    action.SetBranch(147)
    g_AIDebugInfo[195] = '[195] CustomAction195()'

    -- 设置主定时器
    action = ai.NewAction(147, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(565)
    g_AIDebugInfo[147] = '[147] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(565, AI_ACTION.SET_STATE)
    action.SetParam(238) -- 喊话结束
    g_AIDebugInfo[565] = '[565] SetParam(State238)'

    -- 喊话结束
    state = ai.NewState(238)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 174)

    -- 释放技能
    action = ai.NewAction(174, AI_ACTION.CAST_SKILL)
    action.SetParam(1112, 1)
    action.SetBranch(497, 497)
    g_AIDebugInfo[174] = '[174] CAST_SKILL(1112, 1)'

    -- CALL NPC
    action = ai.NewAction(497, 2497)
    action.SetBranch(47)
    g_AIDebugInfo[497] = '[497] CustomAction497()'

    -- 设置主定时器
    action = ai.NewAction(47, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(567)
    g_AIDebugInfo[47] = '[47] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(567, AI_ACTION.SET_STATE)
    action.SetParam(50) -- 进入下一状态
    g_AIDebugInfo[567] = '[567] SetParam(State50)'

    -- 进入下一状态
    state = ai.NewState(50)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 382)

    -- 选择第一威胁
    action = ai.NewAction(382, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(73, 375)
    g_AIDebugInfo[382] = '[382] SELECT_FIRST_THREAT()'

    -- 设置主定时器
    action = ai.NewAction(73, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(569)
    g_AIDebugInfo[73] = '[73] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(569, AI_ACTION.SET_STATE)
    action.SetParam(69) -- 呼唤状态
    g_AIDebugInfo[569] = '[569] SetParam(State69)'

    -- 呼唤状态
    state = ai.NewState(69)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 315)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 375)

    -- 站立
    action = ai.NewAction(315, AI_ACTION.STAND)
    action.SetBranch(429)
    g_AIDebugInfo[315] = '[315] STAND()'

    -- 释放技能
    action = ai.NewAction(429, AI_ACTION.CAST_SKILL)
    action.SetParam(1117, 1)
    action.SetBranch(319, 47)
    g_AIDebugInfo[429] = '[429] CAST_SKILL(1117, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(319, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(47, 317)
    g_AIDebugInfo[319] = '[319] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(317, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(47, 375)
    g_AIDebugInfo[317] = '[317] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(375, AI_ACTION.STAND)
    action.SetBranch(440)
    g_AIDebugInfo[375] = '[375] STAND()'

    -- 设置主定时器
    action = ai.NewAction(440, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(571)
    g_AIDebugInfo[440] = '[440] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(571, AI_ACTION.SET_STATE)
    action.SetParam(439) -- 待机状态
    g_AIDebugInfo[571] = '[571] SetParam(State439)'

    -- 待机状态
    state = ai.NewState(439)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 379)

    -- 搜索敌人
    action = ai.NewAction(379, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(381, 472)
    g_AIDebugInfo[379] = '[379] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(381, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(382)
    g_AIDebugInfo[381] = '[381] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(472, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(375, 479)
    g_AIDebugInfo[472] = '[472] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(479, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(375, 452)
    g_AIDebugInfo[479] = '[479] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(573)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(573, AI_ACTION.SET_STATE)
    action.SetParam(21) -- 普+范围+闪晕范围
    g_AIDebugInfo[573] = '[573] SetParam(State21)'

    -- 普+范围+闪晕范围
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 361)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 514)

    -- 目标知否存在
    action = ai.NewAction(514, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(230, 361)
    g_AIDebugInfo[514] = '[514] IS_TARGET_EXIST()'

    -- 双选项随机
    action = ai.NewAction(230, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(80, 20)
    action.SetBranch(196, 43)
    g_AIDebugInfo[230] = '[230] RANDOM_BI_BRANCH(80, 20)'

    -- 释放技能
    action = ai.NewAction(43, AI_ACTION.CAST_SKILL)
    action.SetParam(1113, 1)
    action.SetBranch(266, 135)
    g_AIDebugInfo[43] = '[43] CAST_SKILL(1113, 1)'

    -- 设置主定时器
    action = ai.NewAction(266, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(575)
    g_AIDebugInfo[266] = '[266] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(575, AI_ACTION.SET_STATE)
    action.SetParam(267) -- 闪现结束
    g_AIDebugInfo[575] = '[575] SetParam(State267)'

    -- 闪现结束
    state = ai.NewState(267)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 263)

    -- 释放技能
    action = ai.NewAction(263, AI_ACTION.CAST_SKILL)
    action.SetParam(1104, 1)
    action.SetBranch(199, 170)
    g_AIDebugInfo[263] = '[263] CAST_SKILL(1104, 1)'

    -- 设置主定时器
    action = ai.NewAction(199, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(577)
    g_AIDebugInfo[199] = '[199] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(577, AI_ACTION.SET_STATE)
    action.SetParam(13) -- 范围技能结束
    g_AIDebugInfo[577] = '[577] SetParam(State13)'

    -- 范围技能结束
    state = ai.NewState(13)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 517)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 361)

    -- 目标知否存在
    action = ai.NewAction(517, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(135, 361)
    g_AIDebugInfo[517] = '[517] IS_TARGET_EXIST()'

    -- 检查技能失败计数
    action = ai.NewAction(170, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(135, 361)
    g_AIDebugInfo[170] = '[170] CHECK_SKILL_FAILED_COUNTER(10)'

    -- Npc标准技能选择
    action = ai.NewAction(196, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(233, 135)
    g_AIDebugInfo[196] = '[196] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(233, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(177, 418)
    g_AIDebugInfo[233] = '[233] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 释放技能
    action = ai.NewAction(418, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(191, 170)
    g_AIDebugInfo[418] = '[418] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(191, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(579)
    g_AIDebugInfo[191] = '[191] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(579, AI_ACTION.SET_STATE)
    action.SetParam(219) -- 追击状态
    g_AIDebugInfo[579] = '[579] SetParam(State219)'

    -- 追击状态
    state = ai.NewState(219)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 507)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 361)

    -- 目标知否存在
    action = ai.NewAction(507, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(141, 361)
    g_AIDebugInfo[507] = '[507] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(141, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(122, 233, 233)
    g_AIDebugInfo[141] = '[141] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(177, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(135, 170)
    g_AIDebugInfo[177] = '[177] NPC_CAST_SELECT_SKILL()'

    -- 搜索敌人
    action = ai.NewAction(64, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(30, 44)
    g_AIDebugInfo[64] = '[64] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(164)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction127(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end



function AI:CustomAction491(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1083,nSceneId, 3997,1971,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3998,1681,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3988,1361,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3968,1013,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5500,2135,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5561,1767,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5562,1391,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5547,947,1048576);
    return 1
end

function AI:CustomAction95(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end

function AI:CustomAction495(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1083,nSceneId, 3997,1971,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3998,1681,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3988,1361,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3968,1013,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5500,2135,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5561,1767,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5562,1391,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5547,947,1048576);
    return 1
end

function AI:CustomAction195(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, "你们的死期到了！")
    return 1
end

function AI:CustomAction497(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1083,nSceneId, 3997,1971,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3998,1681,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3988,1361,1048576);
	KGameBase.CallNpc(1083,nSceneId, 3968,1013,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5500,2135,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5561,1767,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5562,1391,1048576);
	KGameBase.CallNpc(1083,nSceneId, 5547,947,1048576);
    return 1
end

