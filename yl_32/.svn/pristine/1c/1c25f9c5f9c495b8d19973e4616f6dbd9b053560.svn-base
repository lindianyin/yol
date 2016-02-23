-- 编写者：hejingxian
-- 版本号：5

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
    ai.RegisterUserAction(2075, 'CustomAction75')

    -- 初始
    state = ai.NewState(61)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 48)

    -- 记录原点
    action = ai.NewAction(48, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(50)
    g_AIDebugInfo[48] = '[48] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(50, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(88)
    g_AIDebugInfo[50] = '[50] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(88, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 待机状态
    g_AIDebugInfo[88] = '[88] SetParam(State10)'

    -- 待机状态
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 41)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 39)

    -- 选择事件发起者
    action = ai.NewAction(41, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(54, 50)
    g_AIDebugInfo[41] = '[41] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(54, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(71)
    g_AIDebugInfo[54] = '[54] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(71, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(53)
    g_AIDebugInfo[71] = '[71] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(53, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(46)
    g_AIDebugInfo[53] = '[53] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(46, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(90)
    g_AIDebugInfo[46] = '[46] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(90, AI_ACTION.SET_STATE)
    action.SetParam(67) -- 战斗状态
    g_AIDebugInfo[90] = '[90] SetParam(State67)'

    -- 战斗状态
    state = ai.NewState(67)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 43)

    -- 选择第一威胁
    action = ai.NewAction(43, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(3, 50)
    g_AIDebugInfo[43] = '[43] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(3, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(59, 46)
    g_AIDebugInfo[3] = '[3] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(59, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(31, 30)
    g_AIDebugInfo[59] = '[59] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(30, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(92)
    g_AIDebugInfo[30] = '[30] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(92, AI_ACTION.SET_STATE)
    action.SetParam(72) -- 追击状态
    g_AIDebugInfo[92] = '[92] SetParam(State72)'

    -- 追击状态
    state = ai.NewState(72)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 24)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 50)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 70)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 50)

    -- 选择事件发起者
    action = ai.NewAction(24, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(70, 46)
    g_AIDebugInfo[24] = '[24] SELECT_EVENT_SRC()'

    -- 比较返回点距离
    action = ai.NewAction(70, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(50, 59, 59)
    g_AIDebugInfo[70] = '[70] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(31, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(83, 83)
    g_AIDebugInfo[31] = '[31] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(83, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(94)
    g_AIDebugInfo[83] = '[83] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(94, AI_ACTION.SET_STATE)
    action.SetParam(84) -- 自杀
    g_AIDebugInfo[94] = '[94] SetParam(State84)'

    -- 自杀
    state = ai.NewState(84)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 75)

    -- 选中自己
    action = ai.NewAction(75, 2075)
    action.SetBranch(79)
    g_AIDebugInfo[75] = '[75] CustomAction75()'

    -- 释放技能
    action = ai.NewAction(79, AI_ACTION.CAST_SKILL)
    action.SetParam(370, 1)
    action.SetBranch(96, 98)
    g_AIDebugInfo[79] = '[79] CAST_SKILL(370, 1)'

    -- 设置状态
    action = ai.NewAction(96, AI_ACTION.SET_STATE)
    action.SetParam(78) -- 自杀
    g_AIDebugInfo[96] = '[96] SetParam(State78)'

    -- 自杀
    state = ai.NewState(78)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 80)

    -- 设置状态
    action = ai.NewAction(80, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[80] = '[80] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(98, AI_ACTION.SET_STATE)
    action.SetParam(78) -- 自杀
    g_AIDebugInfo[98] = '[98] SetParam(State78)'

    -- 搜索敌人
    action = ai.NewAction(39, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(54, 50)
    g_AIDebugInfo[39] = '[39] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(61)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction75(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

