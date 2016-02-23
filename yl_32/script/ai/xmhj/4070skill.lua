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
    ai.RegisterUserAction(2008, 'AI:GetSkillAction4070')

    -- 初始
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 11)

    -- 设置主定时器
    action = ai.NewAction(11, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(15)
    g_AIDebugInfo[11] = '[11] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(15, AI_ACTION.SET_STATE)
    action.SetParam(5) -- 待机
    g_AIDebugInfo[15] = '[15] SetParam(State5)'

    -- 待机
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 选中自己
    action = ai.NewAction(8, 2008)
    action.SetBranch(9)
    g_AIDebugInfo[8] = '[8] AI:GetSkillAction4070()'

    -- 释放技能
    action = ai.NewAction(9, AI_ACTION.CAST_SKILL)
    action.SetParam(4070, 1)
    action.SetBranch(19, 17)
    g_AIDebugInfo[9] = '[9] CAST_SKILL(4070, 1)'

    -- 设置状态
    action = ai.NewAction(17, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 状态
    g_AIDebugInfo[17] = '[17] SetParam(State4)'

    -- 状态
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 设置状态
    action = ai.NewAction(7, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[7] = '[7] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(19, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 状态
    g_AIDebugInfo[19] = '[19] SetParam(State4)'

    ai.SetInitState(2)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:GetSkillAction4070(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

