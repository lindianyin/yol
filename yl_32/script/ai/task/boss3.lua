-- 编写者：hejingxian
-- 版本号：12

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
    ai.RegisterUserAction(2129, 'CustomAction129')

    -- 初始
    state = ai.NewState(134)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 51)

    -- 记录原点
    action = ai.NewAction(51, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(132)
    g_AIDebugInfo[51] = '[51] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(132, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(175)
    g_AIDebugInfo[132] = '[132] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(175, AI_ACTION.SET_STATE)
    action.SetParam(127) -- 待机状态
    g_AIDebugInfo[175] = '[175] SetParam(State127)'

    -- 待机状态
    state = ai.NewState(127)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 133)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 选择事件发起者
    action = ai.NewAction(133, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(118, 172)
    g_AIDebugInfo[133] = '[133] SELECT_EVENT_SRC()'

    -- 设置系统无敌
    action = ai.NewAction(172, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(138)
    g_AIDebugInfo[172] = '[172] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(138, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(143, 143)
    g_AIDebugInfo[138] = '[138] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(143, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(177)
    g_AIDebugInfo[143] = '[143] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(177, AI_ACTION.SET_STATE)
    action.SetParam(61) -- 无敌返回状态
    g_AIDebugInfo[177] = '[177] SetParam(State61)'

    -- 无敌返回状态
    state = ai.NewState(61)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 162)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 162)

    -- 返回
    action = ai.NewAction(162, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(100, 100)
    g_AIDebugInfo[162] = '[162] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(100, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(179)
    g_AIDebugInfo[100] = '[100] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(179, AI_ACTION.SET_STATE)
    action.SetParam(24) -- 返回成功
    g_AIDebugInfo[179] = '[179] SetParam(State24)'

    -- 返回成功
    state = ai.NewState(24)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 30)

    -- 清除DEBUFF
    action = ai.NewAction(30, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(83)
    g_AIDebugInfo[30] = '[30] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(83, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(132)
    g_AIDebugInfo[83] = '[83] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(118, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(121)
    g_AIDebugInfo[118] = '[118] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(121, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(19)
    g_AIDebugInfo[121] = '[121] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(19, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(74)
    g_AIDebugInfo[19] = '[19] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(74, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(181)
    g_AIDebugInfo[74] = '[74] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(181, AI_ACTION.SET_STATE)
    action.SetParam(49) -- 战斗状态
    g_AIDebugInfo[181] = '[181] SetParam(State49)'

    -- 战斗状态
    state = ai.NewState(49)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 35)

    -- 选择第一威胁
    action = ai.NewAction(35, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(156, 172)
    g_AIDebugInfo[35] = '[35] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(156, AI_ACTION.CHECK_LIFE)
    action.SetParam(60)
    action.SetBranch(9, 9, 154)
    g_AIDebugInfo[156] = '[156] CHECK_LIFE(60)'

    -- 设置主定时器
    action = ai.NewAction(9, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(183)
    g_AIDebugInfo[9] = '[9] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(183, AI_ACTION.SET_STATE)
    action.SetParam(53) -- 发飙状态1
    g_AIDebugInfo[183] = '[183] SetParam(State53)'

    -- 发飙状态1
    state = ai.NewState(53)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 62)

    -- Npc标准技能选择
    action = ai.NewAction(62, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(70, 9)
    g_AIDebugInfo[62] = '[62] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(70, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(129, 129)
    g_AIDebugInfo[70] = '[70] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(129, 2129)
    action.SetBranch(168)
    g_AIDebugInfo[129] = '[129] CustomAction129()'

    -- 设置主定时器
    action = ai.NewAction(168, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(185)
    g_AIDebugInfo[168] = '[168] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(185, AI_ACTION.SET_STATE)
    action.SetParam(27) -- 喊话结束
    g_AIDebugInfo[185] = '[185] SetParam(State27)'

    -- 喊话结束
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 33)

    -- 释放技能
    action = ai.NewAction(33, AI_ACTION.CAST_SKILL)
    action.SetParam(364, 1)
    action.SetBranch(52, 52)
    g_AIDebugInfo[33] = '[33] CAST_SKILL(364, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(52, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(73, 73)
    g_AIDebugInfo[52] = '[52] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(73, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(187)
    g_AIDebugInfo[73] = '[73] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(187, AI_ACTION.SET_STATE)
    action.SetParam(99) -- 进入下一状态
    g_AIDebugInfo[187] = '[187] SetParam(State99)'

    -- 进入下一状态
    state = ai.NewState(99)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 79)

    -- 选择第一威胁
    action = ai.NewAction(79, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(147, 172)
    g_AIDebugInfo[79] = '[79] SELECT_FIRST_THREAT()'

    -- 设置主定时器
    action = ai.NewAction(147, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(189)
    g_AIDebugInfo[147] = '[147] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(189, AI_ACTION.SET_STATE)
    action.SetParam(84) -- 攻击状态2
    g_AIDebugInfo[189] = '[189] SetParam(State84)'

    -- 攻击状态2
    state = ai.NewState(84)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 172)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 87)

    -- Npc标准技能选择
    action = ai.NewAction(87, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(106, 147)
    g_AIDebugInfo[87] = '[87] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(106, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(6, 6)
    g_AIDebugInfo[106] = '[106] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(6, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(73, 39)
    g_AIDebugInfo[6] = '[6] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(39, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(147, 172)
    g_AIDebugInfo[39] = '[39] CHECK_SKILL_FAILED_COUNTER(100)'

    -- 设置主定时器
    action = ai.NewAction(154, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(191)
    g_AIDebugInfo[154] = '[154] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(191, AI_ACTION.SET_STATE)
    action.SetParam(21) -- 攻击状态1
    g_AIDebugInfo[191] = '[191] SetParam(State21)'

    -- 攻击状态1
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 46)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 172)

    -- Npc标准技能选择
    action = ai.NewAction(46, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(96, 74)
    g_AIDebugInfo[46] = '[46] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(96, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(82, 82)
    g_AIDebugInfo[96] = '[96] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(82, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(156, 94)
    g_AIDebugInfo[82] = '[82] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(94, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(156, 172)
    g_AIDebugInfo[94] = '[94] CHECK_SKILL_FAILED_COUNTER(100)'

    -- 搜索敌人
    action = ai.NewAction(7, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(118, 132)
    g_AIDebugInfo[7] = '[7] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(134)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction129(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

