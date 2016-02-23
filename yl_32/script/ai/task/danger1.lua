-- 编写者：hejingxian
-- 版本号：14

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
    ai.RegisterUserAction(2003, 'AI:CustomAction3')

    -- 初始
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 设置主定时器
    action = ai.NewAction(6, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(42)
    g_AIDebugInfo[6] = '[6] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(42, AI_ACTION.SET_STATE)
    action.SetParam(2) -- 待机
    g_AIDebugInfo[42] = '[42] SetParam(State2)'

    -- 待机
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 3)

    -- 选中自己
    action = ai.NewAction(3, 2003)
    action.SetBranch(26)
    g_AIDebugInfo[3] = '[3] AI:CustomAction3()'

    -- 释放技能
    action = ai.NewAction(26, AI_ACTION.CAST_SKILL)
    action.SetParam(351, 1)
    action.SetBranch(40, 40)
    g_AIDebugInfo[26] = '[26] CAST_SKILL(351, 1)'

    -- 设置主定时器
    action = ai.NewAction(40, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(44)
    g_AIDebugInfo[40] = '[40] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(44, AI_ACTION.SET_STATE)
    action.SetParam(30) -- 状态
    g_AIDebugInfo[44] = '[44] SetParam(State30)'

    -- 状态
    state = ai.NewState(30)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    ai.SetInitState(12)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction3(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

