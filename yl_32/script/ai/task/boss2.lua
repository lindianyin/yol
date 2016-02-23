-- 编写者：hejingxian
-- 版本号：33

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
    ai.RegisterUserAction(2103, 'CustomAction103')
    ai.RegisterUserAction(2173, 'CustomAction173')

    -- 初始
    state = ai.NewState(76)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 20)

    -- 记录原点
    action = ai.NewAction(20, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(10)
    g_AIDebugInfo[20] = '[20] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(10, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(223)
    g_AIDebugInfo[10] = '[10] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(223, AI_ACTION.SET_STATE)
    action.SetParam(112) -- 待机状态
    g_AIDebugInfo[223] = '[223] SetParam(State112)'

    -- 待机状态
    state = ai.NewState(112)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 30)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 41)

    -- 搜索敌人
    action = ai.NewAction(30, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(12, 10)
    g_AIDebugInfo[30] = '[30] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(12, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(61)
    g_AIDebugInfo[12] = '[12] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(61, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(21)
    g_AIDebugInfo[61] = '[61] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(21, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(106)
    g_AIDebugInfo[21] = '[21] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(106, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(225)
    g_AIDebugInfo[106] = '[106] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(225, AI_ACTION.SET_STATE)
    action.SetParam(88) -- 战斗状态
    g_AIDebugInfo[225] = '[225] SetParam(State88)'

    -- 战斗状态
    state = ai.NewState(88)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 125)

    -- 选择第一威胁
    action = ai.NewAction(125, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(80, 14)
    g_AIDebugInfo[125] = '[125] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(80, AI_ACTION.CHECK_LIFE)
    action.SetParam(80)
    action.SetBranch(43, 43, 217)
    g_AIDebugInfo[80] = '[80] CHECK_LIFE(80)'

    -- 设置主定时器
    action = ai.NewAction(43, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(227)
    g_AIDebugInfo[43] = '[43] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(227, AI_ACTION.SET_STATE)
    action.SetParam(119) -- 发飙状态1
    g_AIDebugInfo[227] = '[227] SetParam(State119)'

    -- 发飙状态1
    state = ai.NewState(119)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 3)

    -- Npc标准技能选择
    action = ai.NewAction(3, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(9, 43)
    g_AIDebugInfo[3] = '[3] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(9, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(103, 103)
    g_AIDebugInfo[9] = '[9] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(103, 2103)
    action.SetBranch(116)
    g_AIDebugInfo[103] = '[103] CustomAction103()'

    -- 设置主定时器
    action = ai.NewAction(116, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(229)
    g_AIDebugInfo[116] = '[116] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(229, AI_ACTION.SET_STATE)
    action.SetParam(50) -- 喊话结束
    g_AIDebugInfo[229] = '[229] SetParam(State50)'

    -- 喊话结束
    state = ai.NewState(50)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 124)

    -- 释放技能
    action = ai.NewAction(124, AI_ACTION.CAST_SKILL)
    action.SetParam(356, 1)
    action.SetBranch(33, 33)
    g_AIDebugInfo[124] = '[124] CAST_SKILL(356, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(33, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(146, 146)
    g_AIDebugInfo[33] = '[33] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(146, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(231)
    g_AIDebugInfo[146] = '[146] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(231, AI_ACTION.SET_STATE)
    action.SetParam(148) -- 待机一会
    g_AIDebugInfo[231] = '[231] SetParam(State148)'

    -- 待机一会
    state = ai.NewState(148)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 152)

    -- 设置主定时器
    action = ai.NewAction(152, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(233)
    g_AIDebugInfo[152] = '[152] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(233, AI_ACTION.SET_STATE)
    action.SetParam(157) -- 进入下一状态
    g_AIDebugInfo[233] = '[233] SetParam(State157)'

    -- 进入下一状态
    state = ai.NewState(157)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 159)

    -- 选择第一威胁
    action = ai.NewAction(159, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(151, 14)
    g_AIDebugInfo[159] = '[159] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(151, AI_ACTION.CHECK_LIFE)
    action.SetParam(35)
    action.SetBranch(175, 175, 38)
    g_AIDebugInfo[151] = '[151] CHECK_LIFE(35)'

    -- 设置主定时器
    action = ai.NewAction(38, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(235)
    g_AIDebugInfo[38] = '[38] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(235, AI_ACTION.SET_STATE)
    action.SetParam(34) -- 攻击状态2
    g_AIDebugInfo[235] = '[235] SetParam(State34)'

    -- 攻击状态2
    state = ai.NewState(34)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 127)

    -- 设置系统无敌
    action = ai.NewAction(14, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(113)
    g_AIDebugInfo[14] = '[14] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(113, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(78, 78)
    g_AIDebugInfo[113] = '[113] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(78, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(237)
    g_AIDebugInfo[78] = '[78] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(237, AI_ACTION.SET_STATE)
    action.SetParam(68) -- 无敌返回状态
    g_AIDebugInfo[237] = '[237] SetParam(State68)'

    -- 无敌返回状态
    state = ai.NewState(68)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 121)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 121)

    -- 返回
    action = ai.NewAction(121, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(60, 60)
    g_AIDebugInfo[121] = '[121] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(60, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(239)
    g_AIDebugInfo[60] = '[60] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(239, AI_ACTION.SET_STATE)
    action.SetParam(97) -- 返回成功
    g_AIDebugInfo[239] = '[239] SetParam(State97)'

    -- 返回成功
    state = ai.NewState(97)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 128)

    -- 清除DEBUFF
    action = ai.NewAction(128, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(130)
    g_AIDebugInfo[128] = '[128] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(130, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(10)
    g_AIDebugInfo[130] = '[130] SET_SYSTEM_SHEILD(0)'

    -- Npc标准技能选择
    action = ai.NewAction(127, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(59, 152)
    g_AIDebugInfo[127] = '[127] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(59, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(65, 65)
    g_AIDebugInfo[59] = '[59] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(65, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(151, 100)
    g_AIDebugInfo[65] = '[65] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(100, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(151, 14)
    g_AIDebugInfo[100] = '[100] CHECK_SKILL_FAILED_COUNTER(100)'

    -- Npc标准技能选择
    action = ai.NewAction(175, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(179, 179)
    g_AIDebugInfo[175] = '[175] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(179, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(173, 173)
    g_AIDebugInfo[179] = '[179] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(173, 2173)
    action.SetBranch(174)
    g_AIDebugInfo[173] = '[173] CustomAction173()'

    -- 设置主定时器
    action = ai.NewAction(174, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(241)
    g_AIDebugInfo[174] = '[174] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(241, AI_ACTION.SET_STATE)
    action.SetParam(176) -- 喊话结束
    g_AIDebugInfo[241] = '[241] SetParam(State176)'

    -- 喊话结束
    state = ai.NewState(176)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 167)

    -- 释放技能
    action = ai.NewAction(167, AI_ACTION.CAST_SKILL)
    action.SetParam(356, 1)
    action.SetBranch(178, 178)
    g_AIDebugInfo[167] = '[167] CAST_SKILL(356, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(178, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(177, 177)
    g_AIDebugInfo[178] = '[178] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(177, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(243)
    g_AIDebugInfo[177] = '[177] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(243, AI_ACTION.SET_STATE)
    action.SetParam(172) -- 待机一会
    g_AIDebugInfo[243] = '[243] SetParam(State172)'

    -- 待机一会
    state = ai.NewState(172)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 210)

    -- 设置主定时器
    action = ai.NewAction(210, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(245)
    g_AIDebugInfo[210] = '[210] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(245, AI_ACTION.SET_STATE)
    action.SetParam(208) -- 攻击状态2
    g_AIDebugInfo[245] = '[245] SetParam(State208)'

    -- 攻击状态2
    state = ai.NewState(208)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 199)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 14)

    -- Npc标准技能选择
    action = ai.NewAction(199, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(212, 210)
    g_AIDebugInfo[199] = '[199] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(212, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(211, 211)
    g_AIDebugInfo[212] = '[212] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(211, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(210, 201)
    g_AIDebugInfo[211] = '[211] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(201, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(210, 14)
    g_AIDebugInfo[201] = '[201] CHECK_SKILL_FAILED_COUNTER(100)'

    -- 设置主定时器
    action = ai.NewAction(217, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(247)
    g_AIDebugInfo[217] = '[217] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(247, AI_ACTION.SET_STATE)
    action.SetParam(218) -- 攻击状态1
    g_AIDebugInfo[247] = '[247] SetParam(State218)'

    -- 攻击状态1
    state = ai.NewState(218)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 141)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 14)

    -- Npc标准技能选择
    action = ai.NewAction(141, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(28, 106)
    g_AIDebugInfo[141] = '[141] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(28, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(138, 138)
    g_AIDebugInfo[28] = '[28] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(138, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(80, 42)
    g_AIDebugInfo[138] = '[138] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(42, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(80, 14)
    g_AIDebugInfo[42] = '[42] CHECK_SKILL_FAILED_COUNTER(100)'

    -- 选择事件发起者
    action = ai.NewAction(41, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(12, 14)
    g_AIDebugInfo[41] = '[41] SELECT_EVENT_SRC()'

    ai.SetInitState(76)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction103(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction173(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end
