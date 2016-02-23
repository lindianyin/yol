-- 编写者：chenkangyin
-- 版本号：3

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
    ai.RegisterUserAction(2007, 'AI:GetYaoTongBenShen')

    -- 初始
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 4)

    -- 设置主定时器
    action = ai.NewAction(4, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(15)
    g_AIDebugInfo[4] = '[4] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(15, AI_ACTION.SET_STATE)
    action.SetParam(6) -- 待机
    g_AIDebugInfo[15] = '[15] SetParam(State6)'

    -- 待机
    state = ai.NewState(6)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 药桶buff添加
    action = ai.NewAction(7, 2007)
    action.SetBranch(10)
    g_AIDebugInfo[7] = '[7] AI:GetYaoTongBenShen()'

    -- 释放技能
    action = ai.NewAction(10, AI_ACTION.CAST_SKILL)
    action.SetParam(725, 1)
    action.SetBranch(17, 19)
    g_AIDebugInfo[10] = '[10] CAST_SKILL(725, 1)'

    -- 设置状态
    action = ai.NewAction(17, AI_ACTION.SET_STATE)
    action.SetParam(13) -- 状态
    g_AIDebugInfo[17] = '[17] SetParam(State13)'

    -- 状态
    state = ai.NewState(13)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 设置状态
    action = ai.NewAction(8, AI_ACTION.SET_STATE)
    action.SetParam(13)
    g_AIDebugInfo[8] = '[8] SET_STATE(13)'

    -- 设置状态
    action = ai.NewAction(19, AI_ACTION.SET_STATE)
    action.SetParam(13) -- 状态
    g_AIDebugInfo[19] = '[19] SetParam(State13)'

    ai.SetInitState(5)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function   AI:GetYaoTongBenShen(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


