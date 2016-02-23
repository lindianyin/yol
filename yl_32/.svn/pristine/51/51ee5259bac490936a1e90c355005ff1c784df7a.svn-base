-- 编写者：hejingxian
-- 版本号：11

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
    ai.RegisterUserAction(2100, 'CustomAction100')
    ai.RegisterUserAction(2181, 'CustomAction181')

    -- 初始
    state = ai.NewState(95)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 105)

    -- 记录原点
    action = ai.NewAction(105, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(145)
    g_AIDebugInfo[105] = '[105] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(145, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(192)
    g_AIDebugInfo[145] = '[145] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(192, AI_ACTION.SET_STATE)
    action.SetParam(164) -- 待机状态
    g_AIDebugInfo[192] = '[192] SetParam(State164)'

    -- 待机状态
    state = ai.NewState(164)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 112)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 127)

    -- 搜索敌人
    action = ai.NewAction(112, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(146, 145)
    g_AIDebugInfo[112] = '[112] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(146, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(94)
    g_AIDebugInfo[146] = '[146] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(94, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(121)
    g_AIDebugInfo[94] = '[94] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(121, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(163)
    g_AIDebugInfo[121] = '[121] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(163, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(194)
    g_AIDebugInfo[163] = '[163] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(194, AI_ACTION.SET_STATE)
    action.SetParam(117) -- 战斗状态
    g_AIDebugInfo[194] = '[194] SetParam(State117)'

    -- 战斗状态
    state = ai.NewState(117)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 93)

    -- 选择第一威胁
    action = ai.NewAction(93, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(156, 110)
    g_AIDebugInfo[93] = '[93] SELECT_FIRST_THREAT()'

    -- 设置系统无敌
    action = ai.NewAction(110, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(118)
    g_AIDebugInfo[110] = '[110] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(118, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(126, 126)
    g_AIDebugInfo[118] = '[118] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(126, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(196)
    g_AIDebugInfo[126] = '[126] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(196, AI_ACTION.SET_STATE)
    action.SetParam(138) -- 无敌返回状态
    g_AIDebugInfo[196] = '[196] SetParam(State138)'

    -- 无敌返回状态
    state = ai.NewState(138)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 125)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 125)

    -- 返回
    action = ai.NewAction(125, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(130, 130)
    g_AIDebugInfo[125] = '[125] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(130, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(198)
    g_AIDebugInfo[130] = '[130] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(198, AI_ACTION.SET_STATE)
    action.SetParam(159) -- 返回成功
    g_AIDebugInfo[198] = '[198] SetParam(State159)'

    -- 返回成功
    state = ai.NewState(159)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 89)

    -- 清除DEBUFF
    action = ai.NewAction(89, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(144)
    g_AIDebugInfo[89] = '[89] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(144, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(145)
    g_AIDebugInfo[144] = '[144] SET_SYSTEM_SHEILD(0)'

    -- Npc标准技能选择
    action = ai.NewAction(156, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(106, 163)
    g_AIDebugInfo[156] = '[156] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(106, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(183, 183)
    g_AIDebugInfo[106] = '[106] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 三选项随机
    action = ai.NewAction(183, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(5, 10, 85)
    action.SetBranch(100, 181, 170)
    g_AIDebugInfo[183] = '[183] RANDOM_TRI_BRANCH(5, 10, 85)'

    -- NPC喊话
    action = ai.NewAction(100, 2100)
    action.SetBranch(133)
    g_AIDebugInfo[100] = '[100] CustomAction100()'

    -- 设置主定时器
    action = ai.NewAction(133, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(200)
    g_AIDebugInfo[133] = '[133] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(200, AI_ACTION.SET_STATE)
    action.SetParam(131) -- 喊话结束
    g_AIDebugInfo[200] = '[200] SetParam(State131)'

    -- 喊话结束
    state = ai.NewState(131)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 170)

    -- 设置主定时器
    action = ai.NewAction(170, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(202)
    g_AIDebugInfo[170] = '[170] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(202, AI_ACTION.SET_STATE)
    action.SetParam(172) -- 战斗状态
    g_AIDebugInfo[202] = '[202] SetParam(State172)'

    -- 战斗状态
    state = ai.NewState(172)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 132)

    -- 选择第一威胁
    action = ai.NewAction(132, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(113, 110)
    g_AIDebugInfo[132] = '[132] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(113, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(149, 170)
    g_AIDebugInfo[113] = '[113] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(149, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(96, 169)
    g_AIDebugInfo[149] = '[149] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(96, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(170, 122)
    g_AIDebugInfo[96] = '[96] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(122, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(132, 110)
    g_AIDebugInfo[122] = '[122] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置主定时器
    action = ai.NewAction(169, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(204)
    g_AIDebugInfo[169] = '[169] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(204, AI_ACTION.SET_STATE)
    action.SetParam(99) -- 追击状态
    g_AIDebugInfo[204] = '[204] SetParam(State99)'

    -- 追击状态
    state = ai.NewState(99)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 119)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 145)

    -- 比较返回点距离
    action = ai.NewAction(119, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(110, 149, 149)
    g_AIDebugInfo[119] = '[119] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- NPC喊话
    action = ai.NewAction(181, 2181)
    action.SetBranch(182)
    g_AIDebugInfo[181] = '[181] CustomAction181()'

    -- 设置主定时器
    action = ai.NewAction(182, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(206)
    g_AIDebugInfo[182] = '[182] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(206, AI_ACTION.SET_STATE)
    action.SetParam(180) -- 喊话结束
    g_AIDebugInfo[206] = '[206] SetParam(State180)'

    -- 喊话结束
    state = ai.NewState(180)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 170)

    -- 选择事件发起者
    action = ai.NewAction(127, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(146, 110)
    g_AIDebugInfo[127] = '[127] SELECT_EVENT_SRC()'

    ai.SetInitState(95)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction100(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction181(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end

