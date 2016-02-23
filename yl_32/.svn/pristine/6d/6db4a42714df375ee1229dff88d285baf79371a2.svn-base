-- 编写者：chenkangyin
-- 版本号：4

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    -- 注册自定义动作
    ai.RegisterUserAction(2012, 'AI:GeShuiMoYanZhuzi')

    -- 初始
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    -- 设置主定时器
    action = ai.NewAction(1, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(14)
    g_AIDebugInfo[1] = '[1] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(14, AI_ACTION.SET_STATE)
    action.SetParam(5) -- 待机
    g_AIDebugInfo[14] = '[14] SetParam(State5)'

    -- 待机
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 12)

    -- 自定义动作8
    action = ai.NewAction(12, 2012)
    action.SetBranch(9)
    g_AIDebugInfo[12] = '[12] AI:GeShuiMoYanZhuzi()'

    -- 释放技能
    action = ai.NewAction(9, AI_ACTION.CAST_SKILL)
    action.SetParam(554, 1)
    action.SetBranch(10, 10)
    g_AIDebugInfo[9] = '[9] CAST_SKILL(554, 1)'

    -- 设置状态
    action = ai.NewAction(10, AI_ACTION.SET_STATE)
    action.SetParam(1)
    g_AIDebugInfo[10] = '[10] SET_STATE(1)'

    ai.SetInitState(4)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:GeShuiMoYanZhuzi(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

