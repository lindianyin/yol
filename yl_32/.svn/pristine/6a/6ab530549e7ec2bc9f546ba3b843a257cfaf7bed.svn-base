-- 编写者：hejingxian
-- 版本号：174

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
    ai.RegisterUserAction(2265, 'CustomAction265')
    ai.RegisterUserAction(2266, 'CustomAction266')
    ai.RegisterUserAction(2330, 'CustomAction330')
    ai.RegisterUserAction(2467, 'CustomAction467')
    ai.RegisterUserAction(2629, 'CustomAction629')

    -- 关卡-巨石怪
    state = ai.NewState(166)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 71)

    -- 记录原点
    action = ai.NewAction(71, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(7)
    g_AIDebugInfo[71] = '[71] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(7, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(691)
    g_AIDebugInfo[7] = '[7] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(691, AI_ACTION.SET_STATE)
    action.SetParam(13) -- 待机状态
    g_AIDebugInfo[691] = '[691] SetParam(State13)'

    -- 待机状态
    state = ai.NewState(13)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 87)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 103)

    -- 选择事件发起者
    action = ai.NewAction(87, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(175, 7)
    g_AIDebugInfo[87] = '[87] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(175, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(195)
    g_AIDebugInfo[175] = '[175] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(195, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(67)
    g_AIDebugInfo[195] = '[195] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(67, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(230)
    g_AIDebugInfo[67] = '[67] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(230, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(693)
    g_AIDebugInfo[230] = '[230] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(693, AI_ACTION.SET_STATE)
    action.SetParam(39) -- 战斗状态
    g_AIDebugInfo[693] = '[693] SetParam(State39)'

    -- 战斗状态
    state = ai.NewState(39)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 179)

    -- 选择第一威胁
    action = ai.NewAction(179, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(135, 7)
    g_AIDebugInfo[179] = '[179] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(135, AI_ACTION.CHECK_LIFE)
    action.SetParam(90)
    action.SetBranch(108, 108, 3)
    g_AIDebugInfo[135] = '[135] CHECK_LIFE(90)'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(695)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(695, AI_ACTION.SET_STATE)
    action.SetParam(129) -- 普通攻击
    g_AIDebugInfo[695] = '[695] SetParam(State129)'

    -- 普通攻击
    state = ai.NewState(129)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 245)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 534)

    -- Npc标准技能选择
    action = ai.NewAction(245, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(242, 230)
    g_AIDebugInfo[245] = '[245] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(242, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(85, 544)
    g_AIDebugInfo[242] = '[242] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(85, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(135, 206)
    g_AIDebugInfo[85] = '[85] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(206, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(6)
    action.SetBranch(135, 534)
    g_AIDebugInfo[206] = '[206] CHECK_SKILL_FAILED_COUNTER(6)'

    -- 清除目标威胁
    action = ai.NewAction(534, AI_ACTION.CLEAR_THREAT_FOR_TARGET)
    action.SetBranch(7, 7)
    g_AIDebugInfo[534] = '[534] CLEAR_THREAT_FOR_TARGET()'

    -- 释放技能
    action = ai.NewAction(544, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(246, 206)
    g_AIDebugInfo[544] = '[544] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(246, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(697)
    g_AIDebugInfo[246] = '[246] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(697, AI_ACTION.SET_STATE)
    action.SetParam(248) -- 追击状态
    g_AIDebugInfo[697] = '[697] SetParam(State248)'

    -- 追击状态
    state = ai.NewState(248)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 679)

    -- 目标知否存在
    action = ai.NewAction(679, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(247, 179)
    g_AIDebugInfo[679] = '[679] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(247, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(7, 242, 242)
    g_AIDebugInfo[247] = '[247] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置主定时器
    action = ai.NewAction(108, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(699)
    g_AIDebugInfo[108] = '[108] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(699, AI_ACTION.SET_STATE)
    action.SetParam(104) -- 撞人
    g_AIDebugInfo[699] = '[699] SetParam(State104)'

    -- 撞人
    state = ai.NewState(104)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 629)

    -- NPC喊话
    action = ai.NewAction(629, 2629)
    action.SetBranch(124)
    g_AIDebugInfo[629] = '[629] CustomAction629()'

    -- 设置主定时器
    action = ai.NewAction(124, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(701)
    g_AIDebugInfo[124] = '[124] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(701, AI_ACTION.SET_STATE)
    action.SetParam(51) -- 喊话结束
    g_AIDebugInfo[701] = '[701] SetParam(State51)'

    -- 喊话结束
    state = ai.NewState(51)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 93)

    -- 释放技能
    action = ai.NewAction(93, AI_ACTION.CAST_SKILL)
    action.SetParam(444, 1)
    action.SetBranch(231, 231)
    g_AIDebugInfo[93] = '[93] CAST_SKILL(444, 1)'

    -- 设置主定时器
    action = ai.NewAction(231, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(703)
    g_AIDebugInfo[231] = '[231] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(703, AI_ACTION.SET_STATE)
    action.SetParam(146) -- 待机一会
    g_AIDebugInfo[703] = '[703] SetParam(State146)'

    -- 待机一会
    state = ai.NewState(146)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 631)

    -- 释放技能
    action = ai.NewAction(631, AI_ACTION.CAST_SKILL)
    action.SetParam(443, 1)
    action.SetBranch(86, 86)
    g_AIDebugInfo[631] = '[631] CAST_SKILL(443, 1)'

    -- 设置主定时器
    action = ai.NewAction(86, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(705)
    g_AIDebugInfo[86] = '[86] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(705, AI_ACTION.SET_STATE)
    action.SetParam(47) -- 进入下一状态
    g_AIDebugInfo[705] = '[705] SetParam(State47)'

    -- 进入下一状态
    state = ai.NewState(47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 148)

    -- 选择第一威胁
    action = ai.NewAction(148, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(160, 598)
    g_AIDebugInfo[148] = '[148] SELECT_FIRST_THREAT()'

    -- 站立
    action = ai.NewAction(598, AI_ACTION.STAND)
    action.SetBranch(597)
    g_AIDebugInfo[598] = '[598] STAND()'

    -- 设置主定时器
    action = ai.NewAction(597, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(707)
    g_AIDebugInfo[597] = '[597] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(707, AI_ACTION.SET_STATE)
    action.SetParam(596) -- 待机状态
    g_AIDebugInfo[707] = '[707] SetParam(State596)'

    -- 待机状态
    state = ai.NewState(596)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 481)

    -- 搜索敌人
    action = ai.NewAction(481, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(480, 560)
    g_AIDebugInfo[481] = '[481] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(480, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(148)
    g_AIDebugInfo[480] = '[480] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(560, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(598, 567)
    g_AIDebugInfo[560] = '[560] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(567, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(598, 577)
    g_AIDebugInfo[567] = '[567] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(577, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(561)
    g_AIDebugInfo[577] = '[577] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(561, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(586, 586)
    g_AIDebugInfo[561] = '[561] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(586, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(709)
    g_AIDebugInfo[586] = '[586] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(709, AI_ACTION.SET_STATE)
    action.SetParam(572) -- 无敌返回状态
    g_AIDebugInfo[709] = '[709] SetParam(State572)'

    -- 无敌返回状态
    state = ai.NewState(572)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 582)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 582)

    -- 返回
    action = ai.NewAction(582, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(568, 568)
    g_AIDebugInfo[582] = '[582] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(568, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(711)
    g_AIDebugInfo[568] = '[568] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(711, AI_ACTION.SET_STATE)
    action.SetParam(583) -- 返回成功
    g_AIDebugInfo[711] = '[711] SetParam(State583)'

    -- 返回成功
    state = ai.NewState(583)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 575)

    -- 清除DEBUFF
    action = ai.NewAction(575, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(578)
    g_AIDebugInfo[575] = '[575] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(578, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(7)
    g_AIDebugInfo[578] = '[578] SET_SYSTEM_SHEILD(0)'

    -- 检查生命
    action = ai.NewAction(160, AI_ACTION.CHECK_LIFE)
    action.SetParam(70)
    action.SetBranch(328, 328, 83)
    g_AIDebugInfo[160] = '[160] CHECK_LIFE(70)'

    -- 设置主定时器
    action = ai.NewAction(83, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(713)
    g_AIDebugInfo[83] = '[83] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(713, AI_ACTION.SET_STATE)
    action.SetParam(32) -- 高速状态
    g_AIDebugInfo[713] = '[713] SetParam(State32)'

    -- 高速状态
    state = ai.NewState(32)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 188)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 598)

    -- 双选项随机
    action = ai.NewAction(188, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(80, 20)
    action.SetBranch(241, 126)
    g_AIDebugInfo[188] = '[188] RANDOM_BI_BRANCH(80, 20)'

    -- 释放技能
    action = ai.NewAction(126, AI_ACTION.CAST_SKILL)
    action.SetParam(413, 1)
    action.SetBranch(201, 149)
    g_AIDebugInfo[126] = '[126] CAST_SKILL(413, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(149, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(160, 598)
    g_AIDebugInfo[149] = '[149] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 设置主定时器
    action = ai.NewAction(201, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(715)
    g_AIDebugInfo[201] = '[201] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(715, AI_ACTION.SET_STATE)
    action.SetParam(301) -- 待一会儿
    g_AIDebugInfo[715] = '[715] SetParam(State301)'

    -- 待一会儿
    state = ai.NewState(301)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 160)

    -- Npc标准技能选择
    action = ai.NewAction(241, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(42, 160)
    g_AIDebugInfo[241] = '[241] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(42, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(165, 546)
    g_AIDebugInfo[42] = '[42] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(165, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(160, 149)
    g_AIDebugInfo[165] = '[165] NPC_CAST_SELECT_SKILL()'

    -- 释放技能
    action = ai.NewAction(546, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(256, 149)
    g_AIDebugInfo[546] = '[546] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(256, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(717)
    g_AIDebugInfo[256] = '[256] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(717, AI_ACTION.SET_STATE)
    action.SetParam(255) -- 追击状态
    g_AIDebugInfo[717] = '[717] SetParam(State255)'

    -- 追击状态
    state = ai.NewState(255)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 675)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 598)

    -- 目标知否存在
    action = ai.NewAction(675, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(254, 148)
    g_AIDebugInfo[675] = '[675] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(254, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(86, 42, 42)
    g_AIDebugInfo[254] = '[254] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置主定时器
    action = ai.NewAction(328, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(719)
    g_AIDebugInfo[328] = '[328] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(719, AI_ACTION.SET_STATE)
    action.SetParam(334) -- 撞人
    g_AIDebugInfo[719] = '[719] SetParam(State334)'

    -- 撞人
    state = ai.NewState(334)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 330)

    -- NPC喊话
    action = ai.NewAction(330, 2330)
    action.SetBranch(336)
    g_AIDebugInfo[330] = '[330] CustomAction330()'

    -- 设置主定时器
    action = ai.NewAction(336, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(721)
    g_AIDebugInfo[336] = '[336] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(721, AI_ACTION.SET_STATE)
    action.SetParam(345) -- 喊话结束
    g_AIDebugInfo[721] = '[721] SetParam(State345)'

    -- 喊话结束
    state = ai.NewState(345)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 373)

    -- 释放技能
    action = ai.NewAction(373, AI_ACTION.CAST_SKILL)
    action.SetParam(444, 1)
    action.SetBranch(352, 352)
    g_AIDebugInfo[373] = '[373] CAST_SKILL(444, 1)'

    -- 设置主定时器
    action = ai.NewAction(352, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(723)
    g_AIDebugInfo[352] = '[352] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(723, AI_ACTION.SET_STATE)
    action.SetParam(342) -- 待机一会
    g_AIDebugInfo[723] = '[723] SetParam(State342)'

    -- 待机一会
    state = ai.NewState(342)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 637)

    -- 释放技能
    action = ai.NewAction(637, AI_ACTION.CAST_SKILL)
    action.SetParam(443, 1)
    action.SetBranch(357, 357)
    g_AIDebugInfo[637] = '[637] CAST_SKILL(443, 1)'

    -- 设置主定时器
    action = ai.NewAction(357, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(725)
    g_AIDebugInfo[357] = '[357] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(725, AI_ACTION.SET_STATE)
    action.SetParam(371) -- 进入下一状态
    g_AIDebugInfo[725] = '[725] SetParam(State371)'

    -- 进入下一状态
    state = ai.NewState(371)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 359)

    -- 选择第一威胁
    action = ai.NewAction(359, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(390, 606)
    g_AIDebugInfo[359] = '[359] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(390, AI_ACTION.CHECK_LIFE)
    action.SetParam(50)
    action.SetBranch(299, 299, 399)
    g_AIDebugInfo[390] = '[390] CHECK_LIFE(50)'

    -- 设置主定时器
    action = ai.NewAction(299, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(727)
    g_AIDebugInfo[299] = '[299] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(727, AI_ACTION.SET_STATE)
    action.SetParam(287) -- 寻路到正中间
    g_AIDebugInfo[727] = '[727] SetParam(State287)'

    -- 寻路到正中间
    state = ai.NewState(287)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 293)

    -- 进入巡逻路径
    action = ai.NewAction(293, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(292)
    g_AIDebugInfo[293] = '[293] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(292, AI_ACTION.DO_PATROL)
    action.SetBranch(290, 467)
    g_AIDebugInfo[292] = '[292] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(290, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(729)
    g_AIDebugInfo[290] = '[290] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(729, AI_ACTION.SET_STATE)
    action.SetParam(291) -- 主状态
    g_AIDebugInfo[729] = '[729] SetParam(State291)'

    -- 主状态
    state = ai.NewState(291)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 283)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 292)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 292)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 290)

    -- 设置副定时器
    action = ai.NewAction(283, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(4)
    action.SetBranch(467)
    g_AIDebugInfo[283] = '[283] SET_SECONDARY_TIMER(4)'

    -- NPC喊话
    action = ai.NewAction(467, 2467)
    action.SetBranch(471)
    g_AIDebugInfo[467] = '[467] CustomAction467()'

    -- 设置主定时器
    action = ai.NewAction(471, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(731)
    g_AIDebugInfo[471] = '[471] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(731, AI_ACTION.SET_STATE)
    action.SetParam(470) -- 防御形态
    g_AIDebugInfo[731] = '[731] SetParam(State470)'

    -- 防御形态
    state = ai.NewState(470)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 475)

    -- 释放技能
    action = ai.NewAction(475, AI_ACTION.CAST_SKILL)
    action.SetParam(412, 1)
    action.SetBranch(468, 468)
    g_AIDebugInfo[475] = '[475] CAST_SKILL(412, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(468, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(234, 234)
    g_AIDebugInfo[468] = '[468] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(234, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(733)
    g_AIDebugInfo[234] = '[234] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(733, AI_ACTION.SET_STATE)
    action.SetParam(9) -- 下一状态
    g_AIDebugInfo[733] = '[733] SetParam(State9)'

    -- 下一状态
    state = ai.NewState(9)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 193)

    -- 选择第一威胁
    action = ai.NewAction(193, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(121, 613)
    g_AIDebugInfo[193] = '[193] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(121, AI_ACTION.CHECK_LIFE)
    action.SetParam(35)
    action.SetBranch(265, 265, 415)
    g_AIDebugInfo[121] = '[121] CHECK_LIFE(35)'

    -- NPC喊话
    action = ai.NewAction(265, 2265)
    action.SetBranch(50)
    g_AIDebugInfo[265] = '[265] CustomAction265()'

    -- 设置主定时器
    action = ai.NewAction(50, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(735)
    g_AIDebugInfo[50] = '[50] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(735, AI_ACTION.SET_STATE)
    action.SetParam(80) -- 喊话结束
    g_AIDebugInfo[735] = '[735] SetParam(State80)'

    -- 喊话结束
    state = ai.NewState(80)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 40)

    -- 释放技能
    action = ai.NewAction(40, AI_ACTION.CAST_SKILL)
    action.SetParam(417, 1)
    action.SetBranch(162, 162)
    g_AIDebugInfo[40] = '[40] CAST_SKILL(417, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(162, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(266, 266)
    g_AIDebugInfo[162] = '[162] NPC_CAST_SELECT_SKILL()'

    -- NPC变大
    action = ai.NewAction(266, 2266)
    action.SetBranch(110)
    g_AIDebugInfo[266] = '[266] CustomAction266()'

    -- 设置主定时器
    action = ai.NewAction(110, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(737)
    g_AIDebugInfo[110] = '[110] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(737, AI_ACTION.SET_STATE)
    action.SetParam(197) -- 变大结束
    g_AIDebugInfo[737] = '[737] SetParam(State197)'

    -- 变大结束
    state = ai.NewState(197)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 508)

    -- 选择第一威胁
    action = ai.NewAction(508, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(217, 620)
    g_AIDebugInfo[508] = '[508] SELECT_FIRST_THREAT()'

    -- 设置主定时器
    action = ai.NewAction(217, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(739)
    g_AIDebugInfo[217] = '[217] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(739, AI_ACTION.SET_STATE)
    action.SetParam(226) -- 暴走状态
    g_AIDebugInfo[739] = '[739] SetParam(State226)'

    -- 暴走状态
    state = ai.NewState(226)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 133)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 620)

    -- 双选项随机
    action = ai.NewAction(133, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(70, 30)
    action.SetBranch(59, 232)
    g_AIDebugInfo[133] = '[133] RANDOM_BI_BRANCH(70, 30)'

    -- Npc标准技能选择
    action = ai.NewAction(59, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(139, 217)
    g_AIDebugInfo[59] = '[59] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(139, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(48, 555)
    g_AIDebugInfo[139] = '[139] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 释放技能
    action = ai.NewAction(555, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(270, 45)
    g_AIDebugInfo[555] = '[555] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(270, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(741)
    g_AIDebugInfo[270] = '[270] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(741, AI_ACTION.SET_STATE)
    action.SetParam(269) -- 追击状态
    g_AIDebugInfo[741] = '[741] SetParam(State269)'

    -- 追击状态
    state = ai.NewState(269)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 687)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 620)

    -- 目标知否存在
    action = ai.NewAction(687, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(268, 508)
    g_AIDebugInfo[687] = '[687] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(268, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(110, 139, 139)
    g_AIDebugInfo[268] = '[268] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 站立
    action = ai.NewAction(620, AI_ACTION.STAND)
    action.SetBranch(621)
    g_AIDebugInfo[620] = '[620] STAND()'

    -- 设置主定时器
    action = ai.NewAction(621, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(743)
    g_AIDebugInfo[621] = '[621] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(743, AI_ACTION.SET_STATE)
    action.SetParam(622) -- 待机状态
    g_AIDebugInfo[743] = '[743] SetParam(State622)'

    -- 待机状态
    state = ai.NewState(622)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 509)

    -- 搜索敌人
    action = ai.NewAction(509, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(512, 592)
    g_AIDebugInfo[509] = '[509] SEARCH_ENEMY(param[AlertRange])'

    -- 释放技能
    action = ai.NewAction(592, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(620, 593)
    g_AIDebugInfo[592] = '[592] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(593, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(620, 577)
    g_AIDebugInfo[593] = '[593] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(512, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(508)
    g_AIDebugInfo[512] = '[512] ADD_TARGET_TO_THREAT_LIST()'

    -- 检查技能失败计数
    action = ai.NewAction(45, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(217, 620)
    g_AIDebugInfo[45] = '[45] CHECK_SKILL_FAILED_COUNTER(10)'

    -- Npc释放选定技能
    action = ai.NewAction(48, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(217, 45)
    g_AIDebugInfo[48] = '[48] NPC_CAST_SELECT_SKILL()'

    -- 释放技能
    action = ai.NewAction(232, AI_ACTION.CAST_SKILL)
    action.SetParam(413, 1)
    action.SetBranch(159, 45)
    g_AIDebugInfo[232] = '[232] CAST_SKILL(413, 1)'

    -- 设置主定时器
    action = ai.NewAction(159, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(745)
    g_AIDebugInfo[159] = '[159] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(745, AI_ACTION.SET_STATE)
    action.SetParam(303) -- 待一会儿
    g_AIDebugInfo[745] = '[745] SetParam(State303)'

    -- 待一会儿
    state = ai.NewState(303)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 217)

    -- 设置主定时器
    action = ai.NewAction(415, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(747)
    g_AIDebugInfo[415] = '[415] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(747, AI_ACTION.SET_STATE)
    action.SetParam(436) -- 高速状态
    g_AIDebugInfo[747] = '[747] SetParam(State436)'

    -- 高速状态
    state = ai.NewState(436)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 669)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 613)

    -- 三选项随机
    action = ai.NewAction(669, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(50, 43, 7)
    action.SetBranch(438, 418, 665)
    g_AIDebugInfo[669] = '[669] RANDOM_TRI_BRANCH(50, 43, 7)'

    -- Npc标准技能选择
    action = ai.NewAction(438, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(440, 121)
    g_AIDebugInfo[438] = '[438] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(440, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(434, 551)
    g_AIDebugInfo[440] = '[440] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 释放技能
    action = ai.NewAction(551, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(416, 441)
    g_AIDebugInfo[551] = '[551] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(416, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(749)
    g_AIDebugInfo[416] = '[416] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(749, AI_ACTION.SET_STATE)
    action.SetParam(423) -- 追击状态
    g_AIDebugInfo[749] = '[749] SetParam(State423)'

    -- 追击状态
    state = ai.NewState(423)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 682)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 613)

    -- 目标知否存在
    action = ai.NewAction(682, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(426, 121)
    g_AIDebugInfo[682] = '[682] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(426, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(193, 440, 440)
    g_AIDebugInfo[426] = '[426] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 站立
    action = ai.NewAction(613, AI_ACTION.STAND)
    action.SetBranch(614)
    g_AIDebugInfo[613] = '[613] STAND()'

    -- 设置主定时器
    action = ai.NewAction(614, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(751)
    g_AIDebugInfo[614] = '[614] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(751, AI_ACTION.SET_STATE)
    action.SetParam(612) -- 待机状态
    g_AIDebugInfo[751] = '[751] SetParam(State612)'

    -- 待机状态
    state = ai.NewState(612)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 499)

    -- 搜索敌人
    action = ai.NewAction(499, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(503, 566)
    g_AIDebugInfo[499] = '[499] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(503, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(193)
    g_AIDebugInfo[503] = '[503] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(566, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(613, 576)
    g_AIDebugInfo[566] = '[566] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(576, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(613, 577)
    g_AIDebugInfo[576] = '[576] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 检查技能失败计数
    action = ai.NewAction(441, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(121, 613)
    g_AIDebugInfo[441] = '[441] CHECK_SKILL_FAILED_COUNTER(10)'

    -- Npc释放选定技能
    action = ai.NewAction(434, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(121, 441)
    g_AIDebugInfo[434] = '[434] NPC_CAST_SELECT_SKILL()'

    -- 释放技能
    action = ai.NewAction(418, AI_ACTION.CAST_SKILL)
    action.SetParam(413, 1)
    action.SetBranch(431, 441)
    g_AIDebugInfo[418] = '[418] CAST_SKILL(413, 1)'

    -- 设置主定时器
    action = ai.NewAction(431, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(753)
    g_AIDebugInfo[431] = '[431] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(753, AI_ACTION.SET_STATE)
    action.SetParam(439) -- 待一会儿
    g_AIDebugInfo[753] = '[753] SetParam(State439)'

    -- 待一会儿
    state = ai.NewState(439)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 121)

    -- 释放技能
    action = ai.NewAction(665, AI_ACTION.CAST_SKILL)
    action.SetParam(444, 1)
    action.SetBranch(664, 664)
    g_AIDebugInfo[665] = '[665] CAST_SKILL(444, 1)'

    -- 设置主定时器
    action = ai.NewAction(664, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(755)
    g_AIDebugInfo[664] = '[664] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(755, AI_ACTION.SET_STATE)
    action.SetParam(667) -- 待机一会
    g_AIDebugInfo[755] = '[755] SetParam(State667)'

    -- 待机一会
    state = ai.NewState(667)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 663)

    -- 释放技能
    action = ai.NewAction(663, AI_ACTION.CAST_SKILL)
    action.SetParam(443, 1)
    action.SetBranch(657, 657)
    g_AIDebugInfo[663] = '[663] CAST_SKILL(443, 1)'

    -- 设置主定时器
    action = ai.NewAction(657, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(757)
    g_AIDebugInfo[657] = '[657] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(757, AI_ACTION.SET_STATE)
    action.SetParam(660) -- 进入下一状态
    g_AIDebugInfo[757] = '[757] SetParam(State660)'

    -- 进入下一状态
    state = ai.NewState(660)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 121)

    -- 设置主定时器
    action = ai.NewAction(399, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(759)
    g_AIDebugInfo[399] = '[399] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(759, AI_ACTION.SET_STATE)
    action.SetParam(395) -- 高速状态
    g_AIDebugInfo[759] = '[759] SetParam(State395)'

    -- 高速状态
    state = ai.NewState(395)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 653)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 606)

    -- 三选项随机
    action = ai.NewAction(653, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(70, 25, 5)
    action.SetBranch(406, 400, 644)
    g_AIDebugInfo[653] = '[653] RANDOM_TRI_BRANCH(70, 25, 5)'

    -- 释放技能
    action = ai.NewAction(400, AI_ACTION.CAST_SKILL)
    action.SetParam(413, 1)
    action.SetBranch(380, 384)
    g_AIDebugInfo[400] = '[400] CAST_SKILL(413, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(384, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(390, 606)
    g_AIDebugInfo[384] = '[384] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(606, AI_ACTION.STAND)
    action.SetBranch(604)
    g_AIDebugInfo[606] = '[606] STAND()'

    -- 设置主定时器
    action = ai.NewAction(604, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(761)
    g_AIDebugInfo[604] = '[604] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(761, AI_ACTION.SET_STATE)
    action.SetParam(605) -- 待机状态
    g_AIDebugInfo[761] = '[761] SetParam(State605)'

    -- 待机状态
    state = ai.NewState(605)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 495)

    -- 搜索敌人
    action = ai.NewAction(495, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(489, 573)
    g_AIDebugInfo[495] = '[495] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(489, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(359)
    g_AIDebugInfo[489] = '[489] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(573, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(606, 563)
    g_AIDebugInfo[573] = '[573] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(563, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(606, 577)
    g_AIDebugInfo[563] = '[563] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(380, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(763)
    g_AIDebugInfo[380] = '[380] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(763, AI_ACTION.SET_STATE)
    action.SetParam(408) -- 待一会儿
    g_AIDebugInfo[763] = '[763] SetParam(State408)'

    -- 待一会儿
    state = ai.NewState(408)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 390)

    -- Npc标准技能选择
    action = ai.NewAction(406, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(405, 390)
    g_AIDebugInfo[406] = '[406] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(405, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(379, 548)
    g_AIDebugInfo[405] = '[405] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 释放技能
    action = ai.NewAction(548, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(394, 384)
    g_AIDebugInfo[548] = '[548] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(394, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(765)
    g_AIDebugInfo[394] = '[394] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(765, AI_ACTION.SET_STATE)
    action.SetParam(387) -- 追击状态
    g_AIDebugInfo[765] = '[765] SetParam(State387)'

    -- 追击状态
    state = ai.NewState(387)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 672)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 606)

    -- 目标知否存在
    action = ai.NewAction(672, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(381, 359)
    g_AIDebugInfo[672] = '[672] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(381, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(357, 405, 405)
    g_AIDebugInfo[381] = '[381] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(379, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(390, 384)
    g_AIDebugInfo[379] = '[379] NPC_CAST_SELECT_SKILL()'

    -- 释放技能
    action = ai.NewAction(644, AI_ACTION.CAST_SKILL)
    action.SetParam(444, 1)
    action.SetBranch(643, 643)
    g_AIDebugInfo[644] = '[644] CAST_SKILL(444, 1)'

    -- 设置主定时器
    action = ai.NewAction(643, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(767)
    g_AIDebugInfo[643] = '[643] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(767, AI_ACTION.SET_STATE)
    action.SetParam(640) -- 待机一会
    g_AIDebugInfo[767] = '[767] SetParam(State640)'

    -- 待机一会
    state = ai.NewState(640)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 645)

    -- 释放技能
    action = ai.NewAction(645, AI_ACTION.CAST_SKILL)
    action.SetParam(443, 1)
    action.SetBranch(641, 641)
    g_AIDebugInfo[645] = '[645] CAST_SKILL(443, 1)'

    -- 设置主定时器
    action = ai.NewAction(641, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(769)
    g_AIDebugInfo[641] = '[641] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(769, AI_ACTION.SET_STATE)
    action.SetParam(642) -- 进入下一状态
    g_AIDebugInfo[769] = '[769] SetParam(State642)'

    -- 进入下一状态
    state = ai.NewState(642)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 390)

    -- 搜索敌人
    action = ai.NewAction(103, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(175, 7)
    g_AIDebugInfo[103] = '[103] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(166)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction629(pNpc, ai)
    -- 输入自定义动作实现脚本
    	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction330(pNpc, ai)
    -- 输入自定义动作实现脚本
    	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction467(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end

function AI:CustomAction265(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction266(pNpc, ai)
    -- 输入自定义动作实现脚本
    Dialog:StartScaleAnimationToNearBy(pNpc.dwId,150)
    return 1
end
