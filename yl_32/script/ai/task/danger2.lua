-- 编写者：chenkangyin
-- 版本号：2

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
    ai.RegisterUserAction(2009, 'AI:GetWuOuPenFa')

    -- 开始
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 4)

    -- 设置主定时器
    action = ai.NewAction(4, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(22)
    g_AIDebugInfo[4] = '[4] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(22, AI_ACTION.SET_STATE)
    action.SetParam(5) -- 待机
    g_AIDebugInfo[22] = '[22] SetParam(State5)'

    -- 待机
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 9)

    -- 选中自己
    action = ai.NewAction(9, 2009)
    action.SetBranch(11)
    g_AIDebugInfo[9] = '[9] AI:GetWuOuPenFa()'

    -- 释放技能
    action = ai.NewAction(11, AI_ACTION.CAST_SKILL)
    action.SetParam(355, 1)
    action.SetBranch(15, 15)
    g_AIDebugInfo[11] = '[11] CAST_SKILL(355, 1)'

    -- 设置主定时器
    action = ai.NewAction(15, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(40)
    action.SetBranch(24)
    g_AIDebugInfo[15] = '[15] SET_PRIMARY_TIMER(40)'

    -- 设置状态
    action = ai.NewAction(24, AI_ACTION.SET_STATE)
    action.SetParam(6) -- 待机
    g_AIDebugInfo[24] = '[24] SetParam(State6)'

    -- 待机
    state = ai.NewState(6)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 16)

    -- 释放技能
    action = ai.NewAction(16, AI_ACTION.CAST_SKILL)
    action.SetParam(396, 1)
    action.SetBranch(26, 28)
    g_AIDebugInfo[16] = '[16] CAST_SKILL(351, 1)'

    -- 设置状态
    action = ai.NewAction(26, AI_ACTION.SET_STATE)
    action.SetParam(18) -- 状态2
    g_AIDebugInfo[26] = '[26] SetParam(State18)'

    -- 状态2
    state = ai.NewState(18)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 设置状态
    action = ai.NewAction(2, AI_ACTION.SET_STATE)
    action.SetParam(18)
    g_AIDebugInfo[2] = '[2] SET_STATE(18)'

    -- 设置状态
    action = ai.NewAction(28, AI_ACTION.SET_STATE)
    action.SetParam(18) -- 状态2
    g_AIDebugInfo[28] = '[28] SetParam(State18)'

    ai.SetInitState(10)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:GetWuOuPenFa(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

