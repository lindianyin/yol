-- 编写者：chenkangyin
-- 版本号：6

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
    ai.RegisterUserAction(2001, 'AI:GetYunWuren')

    -- 初始
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 3)

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(13)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(13, AI_ACTION.SET_STATE)
    action.SetParam(5) -- 待机
    g_AIDebugInfo[13] = '[13] SetParam(State5)'

    -- 待机
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    -- 云雾人选中自己
    action = ai.NewAction(1, 2001)
    action.SetBranch(7)
    g_AIDebugInfo[1] = '[1] AI:GetYunWuren()'

    -- 释放技能
    action = ai.NewAction(7, AI_ACTION.CAST_SKILL)
    action.SetParam(742, 1)
    action.SetBranch(11, 11)
    g_AIDebugInfo[7] = '[7] CAST_SKILL(742, 1)'

    -- 设置状态
    action = ai.NewAction(11, AI_ACTION.SET_STATE)
    action.SetParam(1)
    g_AIDebugInfo[11] = '[11] SET_STATE(1)'

    ai.SetInitState(12)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:GetYunWuren(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

