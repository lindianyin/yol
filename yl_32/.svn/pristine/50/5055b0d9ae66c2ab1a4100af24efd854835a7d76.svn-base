-- 编写者：hejingxian
-- 版本号：4

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
    ai.RegisterUserAction(2049, 'CustomAction49')
    ai.RegisterUserAction(2052, 'CustomAction52')
    ai.RegisterUserAction(2256, 'CustomAction256')
    ai.RegisterUserAction(2394, 'CustomAction394')
    ai.RegisterUserAction(2459, 'AI:CustomAction459')
    ai.RegisterUserAction(2462, 'AI:CustomAction462')
    ai.RegisterUserAction(2466, 'AI:CustomAction466')
    ai.RegisterUserAction(2468, 'AI:CustomAction468')

    -- 多人本-鞭妖
    state = ai.NewState(118)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 219)

    -- 记录原点
    action = ai.NewAction(219, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(211)
    g_AIDebugInfo[219] = '[219] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(211, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(472)
    g_AIDebugInfo[211] = '[211] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(472, AI_ACTION.SET_STATE)
    action.SetParam(128) -- 待机状态
    g_AIDebugInfo[472] = '[472] SetParam(State128)'

    -- 待机状态
    state = ai.NewState(128)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 248)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 280)

    -- 选择事件发起者
    action = ai.NewAction(248, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(317, 211)
    g_AIDebugInfo[248] = '[248] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(317, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(443)
    g_AIDebugInfo[317] = '[317] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(443, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(340)
    g_AIDebugInfo[443] = '[443] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(340, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(337)
    g_AIDebugInfo[340] = '[340] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(337, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(474)
    g_AIDebugInfo[337] = '[337] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(474, AI_ACTION.SET_STATE)
    action.SetParam(148) -- 战斗状态
    g_AIDebugInfo[474] = '[474] SetParam(State148)'

    -- 战斗状态
    state = ai.NewState(148)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 290)

    -- 选择第一威胁
    action = ai.NewAction(290, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(436, 211)
    g_AIDebugInfo[290] = '[290] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(436, AI_ACTION.CHECK_LIFE)
    action.SetParam(90)
    action.SetBranch(287, 287, 444)
    g_AIDebugInfo[436] = '[436] CHECK_LIFE(90)'

    -- 设置主定时器
    action = ai.NewAction(287, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(476)
    g_AIDebugInfo[287] = '[287] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(476, AI_ACTION.SET_STATE)
    action.SetParam(103) -- 召唤小兵
    g_AIDebugInfo[476] = '[476] SetParam(State103)'

    -- 召唤小兵
    state = ai.NewState(103)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 394)

    -- NPC喊话
    action = ai.NewAction(394, 2394)
    action.SetBranch(447)
    g_AIDebugInfo[394] = '[394] CustomAction394()'

    -- 设置主定时器
    action = ai.NewAction(447, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(478)
    g_AIDebugInfo[447] = '[447] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(478, AI_ACTION.SET_STATE)
    action.SetParam(275) -- 喊话结束
    g_AIDebugInfo[478] = '[478] SetParam(State275)'

    -- 喊话结束
    state = ai.NewState(275)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 261)

    -- 释放技能
    action = ai.NewAction(261, AI_ACTION.CAST_SKILL)
    action.SetParam(307, 1)
    action.SetBranch(459, 459)
    g_AIDebugInfo[261] = '[261] CAST_SKILL(307, 1)'

    -- CALL NPC
    action = ai.NewAction(459, 2459)
    action.SetBranch(20)
    g_AIDebugInfo[459] = '[459] AI:CustomAction459()'

    -- 设置主定时器
    action = ai.NewAction(20, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(480)
    g_AIDebugInfo[20] = '[20] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(480, AI_ACTION.SET_STATE)
    action.SetParam(192) -- 待机一会
    g_AIDebugInfo[480] = '[480] SetParam(State192)'

    -- 待机一会
    state = ai.NewState(192)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 457)

    -- 设置主定时器
    action = ai.NewAction(457, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(482)
    g_AIDebugInfo[457] = '[457] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(482, AI_ACTION.SET_STATE)
    action.SetParam(263) -- 进入下一状态
    g_AIDebugInfo[482] = '[482] SetParam(State263)'

    -- 进入下一状态
    state = ai.NewState(263)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 454)

    -- 选择第一威胁
    action = ai.NewAction(454, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(243, 110)
    g_AIDebugInfo[454] = '[454] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(243, AI_ACTION.CHECK_LIFE)
    action.SetParam(70)
    action.SetBranch(89, 89, 87)
    g_AIDebugInfo[243] = '[243] CHECK_LIFE(70)'

    -- 设置主定时器
    action = ai.NewAction(89, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(484)
    g_AIDebugInfo[89] = '[89] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(484, AI_ACTION.SET_STATE)
    action.SetParam(19) -- 撞人
    g_AIDebugInfo[484] = '[484] SetParam(State19)'

    -- 撞人
    state = ai.NewState(19)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 49)

    -- NPC喊话
    action = ai.NewAction(49, 2049)
    action.SetBranch(433)
    g_AIDebugInfo[49] = '[49] CustomAction49()'

    -- 设置主定时器
    action = ai.NewAction(433, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(486)
    g_AIDebugInfo[433] = '[433] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(486, AI_ACTION.SET_STATE)
    action.SetParam(319) -- 喊话结束
    g_AIDebugInfo[486] = '[486] SetParam(State319)'

    -- 喊话结束
    state = ai.NewState(319)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 300)

    -- 释放技能
    action = ai.NewAction(300, AI_ACTION.CAST_SKILL)
    action.SetParam(307, 1)
    action.SetBranch(462, 462)
    g_AIDebugInfo[300] = '[300] CAST_SKILL(307, 1)'

    -- CALL NPC
    action = ai.NewAction(462, 2462)
    action.SetBranch(155)
    g_AIDebugInfo[462] = '[462] AI:CustomAction462()'

    -- 设置主定时器
    action = ai.NewAction(155, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(488)
    g_AIDebugInfo[155] = '[155] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(488, AI_ACTION.SET_STATE)
    action.SetParam(120) -- 待机一会
    g_AIDebugInfo[488] = '[488] SetParam(State120)'

    -- 待机一会
    state = ai.NewState(120)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 258)

    -- 设置主定时器
    action = ai.NewAction(258, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(490)
    g_AIDebugInfo[258] = '[258] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(490, AI_ACTION.SET_STATE)
    action.SetParam(356) -- 进入下一状态
    g_AIDebugInfo[490] = '[490] SetParam(State356)'

    -- 进入下一状态
    state = ai.NewState(356)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 259)

    -- 选择第一威胁
    action = ai.NewAction(259, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(362, 117)
    g_AIDebugInfo[259] = '[259] SELECT_FIRST_THREAT()'

    -- 站立
    action = ai.NewAction(117, AI_ACTION.STAND)
    action.SetBranch(379)
    g_AIDebugInfo[117] = '[117] STAND()'

    -- 设置主定时器
    action = ai.NewAction(379, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(492)
    g_AIDebugInfo[379] = '[379] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(492, AI_ACTION.SET_STATE)
    action.SetParam(204) -- 待机状态
    g_AIDebugInfo[492] = '[492] SetParam(State204)'

    -- 待机状态
    state = ai.NewState(204)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 170)

    -- 搜索敌人
    action = ai.NewAction(170, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(309, 422)
    g_AIDebugInfo[170] = '[170] SEARCH_ENEMY(param[AlertRange])'

    -- 释放技能
    action = ai.NewAction(422, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(117, 407)
    g_AIDebugInfo[422] = '[422] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(407, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(117, 171)
    g_AIDebugInfo[407] = '[407] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(171, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(108)
    g_AIDebugInfo[171] = '[171] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(108, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(228, 228)
    g_AIDebugInfo[108] = '[108] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(228, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(494)
    g_AIDebugInfo[228] = '[228] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(494, AI_ACTION.SET_STATE)
    action.SetParam(125) -- 无敌返回状态
    g_AIDebugInfo[494] = '[494] SetParam(State125)'

    -- 无敌返回状态
    state = ai.NewState(125)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 82)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 82)

    -- 返回
    action = ai.NewAction(82, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(397, 397)
    g_AIDebugInfo[82] = '[82] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(397, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(496)
    g_AIDebugInfo[397] = '[397] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(496, AI_ACTION.SET_STATE)
    action.SetParam(213) -- 返回成功
    g_AIDebugInfo[496] = '[496] SetParam(State213)'

    -- 返回成功
    state = ai.NewState(213)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 144)

    -- 清除DEBUFF
    action = ai.NewAction(144, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(185)
    g_AIDebugInfo[144] = '[144] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(185, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(211)
    g_AIDebugInfo[185] = '[185] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(309, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(259)
    g_AIDebugInfo[309] = '[309] ADD_TARGET_TO_THREAT_LIST()'

    -- 检查生命
    action = ai.NewAction(362, AI_ACTION.CHECK_LIFE)
    action.SetParam(50)
    action.SetBranch(52, 52, 329)
    g_AIDebugInfo[362] = '[362] CHECK_LIFE(50)'

    -- NPC喊话
    action = ai.NewAction(52, 2052)
    action.SetBranch(181)
    g_AIDebugInfo[52] = '[52] CustomAction52()'

    -- 设置主定时器
    action = ai.NewAction(181, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(498)
    g_AIDebugInfo[181] = '[181] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(498, AI_ACTION.SET_STATE)
    action.SetParam(402) -- 防御形态
    g_AIDebugInfo[498] = '[498] SetParam(State402)'

    -- 防御形态
    state = ai.NewState(402)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 38)

    -- 释放技能
    action = ai.NewAction(38, AI_ACTION.CAST_SKILL)
    action.SetParam(307, 1)
    action.SetBranch(466, 466)
    g_AIDebugInfo[38] = '[38] CAST_SKILL(307, 1)'

    -- CALL NPC
    action = ai.NewAction(466, 2466)
    action.SetBranch(10)
    g_AIDebugInfo[466] = '[466] AI:CustomAction466()'

    -- 设置主定时器
    action = ai.NewAction(10, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(500)
    g_AIDebugInfo[10] = '[10] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(500, AI_ACTION.SET_STATE)
    action.SetParam(97) -- 下一状态
    g_AIDebugInfo[500] = '[500] SetParam(State97)'

    -- 下一状态
    state = ai.NewState(97)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 322)

    -- 选择第一威胁
    action = ai.NewAction(322, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(408, 425)
    g_AIDebugInfo[322] = '[322] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(408, AI_ACTION.CHECK_LIFE)
    action.SetParam(35)
    action.SetBranch(256, 256, 252)
    g_AIDebugInfo[408] = '[408] CHECK_LIFE(35)'

    -- 设置主定时器
    action = ai.NewAction(252, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(502)
    g_AIDebugInfo[252] = '[252] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(502, AI_ACTION.SET_STATE)
    action.SetParam(399) -- 高速状态
    g_AIDebugInfo[502] = '[502] SetParam(State399)'

    -- 高速状态
    state = ai.NewState(399)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 425)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 428)

    -- 站立
    action = ai.NewAction(425, AI_ACTION.STAND)
    action.SetBranch(335)
    g_AIDebugInfo[425] = '[425] STAND()'

    -- 设置主定时器
    action = ai.NewAction(335, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(504)
    g_AIDebugInfo[335] = '[335] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(504, AI_ACTION.SET_STATE)
    action.SetParam(169) -- 待机状态
    g_AIDebugInfo[504] = '[504] SetParam(State169)'

    -- 待机状态
    state = ai.NewState(169)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 182)

    -- 搜索敌人
    action = ai.NewAction(182, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(190, 17)
    g_AIDebugInfo[182] = '[182] SEARCH_ENEMY(param[AlertRange])'

    -- 释放技能
    action = ai.NewAction(17, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(425, 452)
    g_AIDebugInfo[17] = '[17] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(452, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(425, 171)
    g_AIDebugInfo[452] = '[452] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(190, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(322)
    g_AIDebugInfo[190] = '[190] ADD_TARGET_TO_THREAT_LIST()'

    -- Npc标准技能选择
    action = ai.NewAction(428, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(424, 408)
    g_AIDebugInfo[428] = '[428] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(424, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(94, 418)
    g_AIDebugInfo[424] = '[424] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(94, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(408, 380)
    g_AIDebugInfo[94] = '[94] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(380, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(408, 425)
    g_AIDebugInfo[380] = '[380] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 释放技能
    action = ai.NewAction(418, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(450, 380)
    g_AIDebugInfo[418] = '[418] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(450, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(506)
    g_AIDebugInfo[450] = '[450] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(506, AI_ACTION.SET_STATE)
    action.SetParam(305) -- 追击状态
    g_AIDebugInfo[506] = '[506] SetParam(State305)'

    -- 追击状态
    state = ai.NewState(305)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 229)

    -- 比较返回点距离
    action = ai.NewAction(229, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(322, 424, 424)
    g_AIDebugInfo[229] = '[229] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- NPC喊话
    action = ai.NewAction(256, 2256)
    action.SetBranch(361)
    g_AIDebugInfo[256] = '[256] CustomAction256()'

    -- 设置主定时器
    action = ai.NewAction(361, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(508)
    g_AIDebugInfo[361] = '[361] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(508, AI_ACTION.SET_STATE)
    action.SetParam(96) -- 喊话结束
    g_AIDebugInfo[508] = '[508] SetParam(State96)'

    -- 喊话结束
    state = ai.NewState(96)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 345)

    -- 释放技能
    action = ai.NewAction(345, AI_ACTION.CAST_SKILL)
    action.SetParam(307, 1)
    action.SetBranch(468, 468)
    g_AIDebugInfo[345] = '[345] CAST_SKILL(307, 1)'

    -- CALL NPC
    action = ai.NewAction(468, 2468)
    action.SetBranch(93)
    g_AIDebugInfo[468] = '[468] AI:CustomAction468()'

    -- 设置主定时器
    action = ai.NewAction(93, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(510)
    g_AIDebugInfo[93] = '[93] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(510, AI_ACTION.SET_STATE)
    action.SetParam(390) -- 变大结束
    g_AIDebugInfo[510] = '[510] SetParam(State390)'

    -- 变大结束
    state = ai.NewState(390)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 304)

    -- 选择第一威胁
    action = ai.NewAction(304, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(64, 26)
    g_AIDebugInfo[304] = '[304] SELECT_FIRST_THREAT()'

    -- 设置主定时器
    action = ai.NewAction(64, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(512)
    g_AIDebugInfo[64] = '[64] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(512, AI_ACTION.SET_STATE)
    action.SetParam(29) -- 暴走状态
    g_AIDebugInfo[512] = '[512] SetParam(State29)'

    -- 暴走状态
    state = ai.NewState(29)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 288)

    -- 站立
    action = ai.NewAction(26, AI_ACTION.STAND)
    action.SetBranch(160)
    g_AIDebugInfo[26] = '[26] STAND()'

    -- 设置主定时器
    action = ai.NewAction(160, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(514)
    g_AIDebugInfo[160] = '[160] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(514, AI_ACTION.SET_STATE)
    action.SetParam(114) -- 待机状态
    g_AIDebugInfo[514] = '[514] SetParam(State114)'

    -- 待机状态
    state = ai.NewState(114)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 搜索敌人
    action = ai.NewAction(7, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(439, 286)
    g_AIDebugInfo[7] = '[7] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(439, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(304)
    g_AIDebugInfo[439] = '[439] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(286, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(26, 116)
    g_AIDebugInfo[286] = '[286] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(116, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(26, 171)
    g_AIDebugInfo[116] = '[116] CHECK_SKILL_FAILED_COUNTER(3)'

    -- Npc标准技能选择
    action = ai.NewAction(288, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(332, 64)
    g_AIDebugInfo[288] = '[288] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(332, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(176, 79)
    g_AIDebugInfo[332] = '[332] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 释放技能
    action = ai.NewAction(79, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(446, 406)
    g_AIDebugInfo[79] = '[79] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(406, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(64, 26)
    g_AIDebugInfo[406] = '[406] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 设置主定时器
    action = ai.NewAction(446, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(516)
    g_AIDebugInfo[446] = '[446] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(516, AI_ACTION.SET_STATE)
    action.SetParam(145) -- 追击状态
    g_AIDebugInfo[516] = '[516] SetParam(State145)'

    -- 追击状态
    state = ai.NewState(145)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 240)

    -- 比较返回点距离
    action = ai.NewAction(240, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(93, 332, 332)
    g_AIDebugInfo[240] = '[240] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(176, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(64, 406)
    g_AIDebugInfo[176] = '[176] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(329, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(518)
    g_AIDebugInfo[329] = '[329] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(518, AI_ACTION.SET_STATE)
    action.SetParam(277) -- 高速状态
    g_AIDebugInfo[518] = '[518] SetParam(State277)'

    -- 高速状态
    state = ai.NewState(277)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 346)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 117)

    -- Npc标准技能选择
    action = ai.NewAction(346, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(265, 362)
    g_AIDebugInfo[346] = '[346] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(265, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(32, 158)
    g_AIDebugInfo[265] = '[265] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 释放技能
    action = ai.NewAction(158, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(78, 146)
    g_AIDebugInfo[158] = '[158] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(146, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(362, 117)
    g_AIDebugInfo[146] = '[146] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 设置主定时器
    action = ai.NewAction(78, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(520)
    g_AIDebugInfo[78] = '[78] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(520, AI_ACTION.SET_STATE)
    action.SetParam(212) -- 追击状态
    g_AIDebugInfo[520] = '[520] SetParam(State212)'

    -- 追击状态
    state = ai.NewState(212)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 324)

    -- 比较返回点距离
    action = ai.NewAction(324, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(258, 265, 265)
    g_AIDebugInfo[324] = '[324] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(32, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(362, 146)
    g_AIDebugInfo[32] = '[32] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(87, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(522)
    g_AIDebugInfo[87] = '[87] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(522, AI_ACTION.SET_STATE)
    action.SetParam(57) -- 高速状态
    g_AIDebugInfo[522] = '[522] SetParam(State57)'

    -- 高速状态
    state = ai.NewState(57)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 129)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 110)

    -- Npc标准技能选择
    action = ai.NewAction(129, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(413, 243)
    g_AIDebugInfo[129] = '[129] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(413, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(389, 266)
    g_AIDebugInfo[413] = '[413] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(389, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(243, 378)
    g_AIDebugInfo[389] = '[389] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(378, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(243, 110)
    g_AIDebugInfo[378] = '[378] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(110, AI_ACTION.STAND)
    action.SetBranch(206)
    g_AIDebugInfo[110] = '[110] STAND()'

    -- 设置主定时器
    action = ai.NewAction(206, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(524)
    g_AIDebugInfo[206] = '[206] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(524, AI_ACTION.SET_STATE)
    action.SetParam(285) -- 待机状态
    g_AIDebugInfo[524] = '[524] SetParam(State285)'

    -- 待机状态
    state = ai.NewState(285)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 37)

    -- 搜索敌人
    action = ai.NewAction(37, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(172, 3)
    g_AIDebugInfo[37] = '[37] SEARCH_ENEMY(param[AlertRange])'

    -- 释放技能
    action = ai.NewAction(3, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(110, 308)
    g_AIDebugInfo[3] = '[3] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(308, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(110, 171)
    g_AIDebugInfo[308] = '[308] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(172, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(454)
    g_AIDebugInfo[172] = '[172] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(266, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(276, 378)
    g_AIDebugInfo[266] = '[266] CAST_SKILL(311, 1)'

    -- 设置主定时器
    action = ai.NewAction(276, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(526)
    g_AIDebugInfo[276] = '[276] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(526, AI_ACTION.SET_STATE)
    action.SetParam(354) -- 追击状态
    g_AIDebugInfo[526] = '[526] SetParam(State354)'

    -- 追击状态
    state = ai.NewState(354)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 71)

    -- 比较返回点距离
    action = ai.NewAction(71, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(457, 413, 413)
    g_AIDebugInfo[71] = '[71] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置主定时器
    action = ai.NewAction(444, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(528)
    g_AIDebugInfo[444] = '[444] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(528, AI_ACTION.SET_STATE)
    action.SetParam(66) -- 普通攻击
    g_AIDebugInfo[528] = '[528] SetParam(State66)'

    -- 普通攻击
    state = ai.NewState(66)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 291)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 269)

    -- 清除目标威胁
    action = ai.NewAction(291, AI_ACTION.CLEAR_THREAT_FOR_TARGET)
    action.SetBranch(211, 211)
    g_AIDebugInfo[291] = '[291] CLEAR_THREAT_FOR_TARGET()'

    -- Npc标准技能选择
    action = ai.NewAction(269, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(193, 337)
    g_AIDebugInfo[269] = '[269] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(193, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(14, 68)
    g_AIDebugInfo[193] = '[193] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 释放技能
    action = ai.NewAction(68, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(150, 384)
    g_AIDebugInfo[68] = '[68] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(384, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(6)
    action.SetBranch(436, 291)
    g_AIDebugInfo[384] = '[384] CHECK_SKILL_FAILED_COUNTER(6)'

    -- 设置主定时器
    action = ai.NewAction(150, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(530)
    g_AIDebugInfo[150] = '[150] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(530, AI_ACTION.SET_STATE)
    action.SetParam(372) -- 追击状态
    g_AIDebugInfo[530] = '[530] SetParam(State372)'

    -- 追击状态
    state = ai.NewState(372)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 83)

    -- 比较返回点距离
    action = ai.NewAction(83, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(211, 193, 193)
    g_AIDebugInfo[83] = '[83] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(14, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(436, 384)
    g_AIDebugInfo[14] = '[14] NPC_CAST_SELECT_SKILL()'

    -- 搜索敌人
    action = ai.NewAction(280, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(317, 211)
    g_AIDebugInfo[280] = '[280] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(118)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction394(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction459(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1458,nSceneId, 1288,1557,1048576);
	KGameBase.CallNpc(1458,nSceneId, 1095,1728,1048576);
	KGameBase.CallNpc(1458,nSceneId, 912,1975,1048576);
	KGameBase.CallNpc(1458,nSceneId, 797,2254,1048576);
	KGameBase.CallNpc(1458,nSceneId, 787,2648,1048576);
	KGameBase.CallNpc(1458,nSceneId, 1473,2542,1048576);
	KGameBase.CallNpc(1458,nSceneId, 1434,2464,1048576);
	KGameBase.CallNpc(1458,nSceneId, 1387,2781,1048576);
	KGameBase.CallNpc(1458,nSceneId, 1406,2919,1048576);
	KGameBase.CallNpc(1458,nSceneId, 1472,3123,1048576);
	KGameBase.CallNpc(1458,nSceneId, 826,3126,1048576);
	KGameBase.CallNpc(1458,nSceneId, 953,3373,1048576);
    return 1
end

function AI:CustomAction49(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
	return 1
end

function AI:CustomAction462(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1459,nSceneId, 1048,4130,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1149,4236,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1270,4293,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1435,4357,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1507,4177,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1355,4083,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1229,4019,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1079,3907,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1234,3793,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1397,3952,1048576);
	KGameBase.CallNpc(1459,nSceneId, 1593,3927,1048576);
    return 1
end

function AI:CustomAction52(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction466(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1460,nSceneId, 1048,4130,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1149,4236,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1270,4293,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1435,4357,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1507,4177,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1355,4083,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1229,4019,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1079,3907,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1234,3793,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1397,3952,1048576);
	KGameBase.CallNpc(1460,nSceneId, 1593,3927,1048576);
    return 1
end

function AI:CustomAction256(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction468(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1461,nSceneId, 1214,4160,1048576);
    return 1
end



