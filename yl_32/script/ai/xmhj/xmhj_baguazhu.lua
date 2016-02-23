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
    ai.RegisterUserAction(2006, 'AI:GetBaGuaZhu')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 3)

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(16)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(16, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 待机
    g_AIDebugInfo[16] = '[16] SetParam(State4)'

    -- 待机
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 选中八卦柱
    action = ai.NewAction(6, 2006)
    action.SetBranch(8)
    g_AIDebugInfo[6] = '[6] AI:GetBaGuaZhu()'

    -- 释放技能
    action = ai.NewAction(8, AI_ACTION.CAST_SKILL)
    action.SetParam(615, 1)
    action.SetBranch(18, 20)
    g_AIDebugInfo[8] = '[8] CAST_SKILL(615, 1)'

    -- 设置状态
    action = ai.NewAction(18, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 状态
    g_AIDebugInfo[18] = '[18] SetParam(State14)'

    -- 状态
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 11)

    -- 设置状态
    action = ai.NewAction(11, AI_ACTION.SET_STATE)
    action.SetParam(14)
    g_AIDebugInfo[11] = '[11] SET_STATE(14)'

    -- 设置状态
    action = ai.NewAction(20, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 状态
    g_AIDebugInfo[20] = '[20] SetParam(State14)'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:GetBaGuaZhu(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

