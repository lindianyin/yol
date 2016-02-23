-- 编写者：hejingxian
-- 版本号：15

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
    ai.RegisterUserAction(2169, 'AI:CustomAction169')

    -- 初始
    state = ai.NewState(163)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 167)

    -- 设置主定时器
    action = ai.NewAction(167, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(90)
    action.SetBranch(171)
    g_AIDebugInfo[167] = '[167] SET_PRIMARY_TIMER(42)'

    -- 设置状态
    action = ai.NewAction(171, AI_ACTION.SET_STATE)
    action.SetParam(159) -- 待机
    g_AIDebugInfo[171] = '[171] SetParam(State159)'

    -- 待机
    state = ai.NewState(159)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 169)

    -- 选中自己
    action = ai.NewAction(169, 2169)
    action.SetBranch(157)
    g_AIDebugInfo[169] = '[169] AI:CustomAction169()'

    -- 释放技能
    action = ai.NewAction(157, AI_ACTION.CAST_SKILL)
    action.SetParam(304, 1)
    action.SetBranch(175, 173)
    g_AIDebugInfo[157] = '[157] CAST_SKILL(304, 1)'

    -- 设置状态
    action = ai.NewAction(173, AI_ACTION.SET_STATE)
    action.SetParam(168) -- 状态
    g_AIDebugInfo[173] = '[173] SetParam(State168)'

    -- 状态
    state = ai.NewState(168)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 160)

    -- 设置状态
    action = ai.NewAction(160, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[160] = '[160] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(175, AI_ACTION.SET_STATE)
    action.SetParam(168) -- 状态
    g_AIDebugInfo[175] = '[175] SetParam(State168)'

    ai.SetInitState(163)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction169(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

