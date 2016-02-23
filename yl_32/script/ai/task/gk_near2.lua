-- 编写者：hejingxian
-- 版本号：18

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
    ai.RegisterUserAction(2048, 'CustomAction48')
    ai.RegisterUserAction(2055, 'CustomAction55')

    -- 初始
    state = ai.NewState(40)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 42)

    -- 设置主定时器
    action = ai.NewAction(42, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(69)
    g_AIDebugInfo[42] = '[42] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(69, AI_ACTION.SET_STATE)
    action.SetParam(37) -- 待机
    g_AIDebugInfo[69] = '[69] SetParam(State37)'

    -- 待机
    state = ai.NewState(37)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 48)

    -- 选中自己
    action = ai.NewAction(48, 2048)
    action.SetBranch(47)
    g_AIDebugInfo[48] = '[48] CustomAction48()'

    -- Npc标准技能选择
    action = ai.NewAction(47, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(50, 50)
    g_AIDebugInfo[47] = '[47] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(50, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(65, 65)
    g_AIDebugInfo[50] = '[50] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(65, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(71)
    g_AIDebugInfo[65] = '[65] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(71, AI_ACTION.SET_STATE)
    action.SetParam(53) -- 待机
    g_AIDebugInfo[71] = '[71] SetParam(State53)'

    -- 待机
    state = ai.NewState(53)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 55)

    -- 选中自己
    action = ai.NewAction(55, 2055)
    action.SetBranch(57)
    g_AIDebugInfo[55] = '[55] CustomAction55()'

    -- 释放技能
    action = ai.NewAction(57, AI_ACTION.CAST_SKILL)
    action.SetParam(370, 1)
    action.SetBranch(67, 67)
    g_AIDebugInfo[57] = '[57] CAST_SKILL(370, 1)'

    -- 设置主定时器
    action = ai.NewAction(67, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(19900)
    action.SetBranch(73)
    g_AIDebugInfo[67] = '[67] SET_PRIMARY_TIMER(19900)'

    -- 设置状态
    action = ai.NewAction(73, AI_ACTION.SET_STATE)
    action.SetParam(60) -- 待机
    g_AIDebugInfo[73] = '[73] SetParam(State60)'

    -- 待机
    state = ai.NewState(60)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 52)

    -- 设置状态
    action = ai.NewAction(52, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[52] = '[52] SET_STATE(4)'

    ai.SetInitState(40)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction48(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction55(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

