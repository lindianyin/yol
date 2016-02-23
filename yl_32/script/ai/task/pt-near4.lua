-- 编写者：hejingxian
-- 版本号：7

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
    ai.RegisterUserAction(2111, 'AI:CustomAction111')

    -- 初始
    state = ai.NewState(54)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 30)

    -- 记录原点
    action = ai.NewAction(30, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(113)
    g_AIDebugInfo[30] = '[30] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(113, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(119)
    g_AIDebugInfo[113] = '[113] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(119, AI_ACTION.SET_STATE)
    action.SetParam(112) -- 给自己加持续伤害
    g_AIDebugInfo[119] = '[119] SetParam(State112)'

    -- 给自己加持续伤害
    state = ai.NewState(112)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 111)

    -- 选中自己
    action = ai.NewAction(111, 2111)
    action.SetBranch(116)
    g_AIDebugInfo[111] = '[111] AI:CustomAction111()'

    -- 释放技能
    action = ai.NewAction(116, AI_ACTION.CAST_SKILL)
    action.SetParam(1108, 1)
    action.SetBranch(109, 109)
    g_AIDebugInfo[116] = '[116] CAST_SKILL(1108, 1)'

    -- 设置主定时器
    action = ai.NewAction(109, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(121)
    g_AIDebugInfo[109] = '[109] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(121, AI_ACTION.SET_STATE)
    action.SetParam(106) -- 进入闲逛
    g_AIDebugInfo[121] = '[121] SetParam(State106)'

    -- 进入闲逛
    state = ai.NewState(106)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 81)

    -- 选择状态
    action = ai.NewAction(81, AI_ACTION.SELECT_STATE)
    action.SetParam(1)
    action.SetBranch(83, 6, 83)
    g_AIDebugInfo[81] = '[81] SELECT_STATE(1)'

    -- 闲逛一次
    action = ai.NewAction(6, AI_ACTION.WANDER)
    action.SetParam(500, 600)
    action.SetBranch(5)
    g_AIDebugInfo[6] = '[6] WANDER(500, 600)'

    -- 设置副定时器
    action = ai.NewAction(5, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(32)
    action.SetBranch(83)
    g_AIDebugInfo[5] = '[5] SET_SECONDARY_TIMER(32)'

    -- 设置主定时器
    action = ai.NewAction(83, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(123)
    g_AIDebugInfo[83] = '[83] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(123, AI_ACTION.SET_STATE)
    action.SetParam(118) -- 待机状态
    g_AIDebugInfo[123] = '[123] SetParam(State118)'

    -- 待机状态
    state = ai.NewState(118)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 81)

    ai.SetInitState(54)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction111(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

