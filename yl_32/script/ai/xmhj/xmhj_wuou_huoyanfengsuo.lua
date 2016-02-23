-- 编写者：chenkangyin
-- 版本号：1

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
    ai.RegisterUserAction(2005, 'AI:GetFengSuoHuoYan')

    -- 开始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(15)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(15, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 待机
    g_AIDebugInfo[15] = '[15] SetParam(State4)'

    -- 待机
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 5)

    -- 选中自己
    action = ai.NewAction(5, 2005)
    action.SetBranch(8)
    g_AIDebugInfo[5] = '[5] AI:GetFengSuoHuoYan()'

    -- 释放技能
    action = ai.NewAction(8, AI_ACTION.CAST_SKILL)
    action.SetParam(4062, 1)
    action.SetBranch(17, 19)
    g_AIDebugInfo[8] = '[8] CAST_SKILL(4062, 1)'

    -- 设置状态
    action = ai.NewAction(17, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 状态
    g_AIDebugInfo[17] = '[17] SetParam(State10)'

    -- 状态
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 13)

    -- 设置状态
    action = ai.NewAction(13, AI_ACTION.SET_STATE)
    action.SetParam(10)
    g_AIDebugInfo[13] = '[13] SET_STATE(10)'

    -- 设置状态
    action = ai.NewAction(19, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 状态
    g_AIDebugInfo[19] = '[19] SetParam(State10)'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:GetFengSuoHuoYan(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

