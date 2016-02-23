-- 编写者：hejingxian
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
    ai.RegisterUserAction(2060, 'CustomAction60')

    -- 初始
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 33)

    -- 记录原点
    action = ai.NewAction(33, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(51)
    g_AIDebugInfo[33] = '[33] RECORD_ORIGIN_POSITION()'

    -- 选择状态
    action = ai.NewAction(51, AI_ACTION.SELECT_STATE)
    action.SetParam(1)
    action.SetBranch(65, 23, 65)
    g_AIDebugInfo[51] = '[51] SELECT_STATE(1)'

    -- 设置主定时器
    action = ai.NewAction(65, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(106)
    g_AIDebugInfo[65] = '[65] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(106, AI_ACTION.SET_STATE)
    action.SetParam(32) -- 待机状态
    g_AIDebugInfo[106] = '[106] SetParam(State32)'

    -- 待机状态
    state = ai.NewState(32)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 51)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 65)

    -- 双选项随机
    action = ai.NewAction(23, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(5, 95)
    action.SetBranch(60, 34)
    g_AIDebugInfo[23] = '[23] RANDOM_BI_BRANCH(5, 95)'

    -- 闲逛一次
    action = ai.NewAction(34, AI_ACTION.WANDER)
    action.SetParam(400, 600)
    action.SetBranch(43)
    g_AIDebugInfo[34] = '[34] WANDER(400, 600)'

    -- 设置副定时器
    action = ai.NewAction(43, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(48)
    action.SetBranch(108)
    g_AIDebugInfo[43] = '[43] SET_SECONDARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(108, AI_ACTION.SET_STATE)
    action.SetParam(32) -- 待机状态
    g_AIDebugInfo[108] = '[108] SetParam(State32)'

    -- NPC喊话
    action = ai.NewAction(60, 2060)
    action.SetBranch(54)
    g_AIDebugInfo[60] = '[60] CustomAction60()'

    -- 设置主定时器
    action = ai.NewAction(54, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(110)
    g_AIDebugInfo[54] = '[54] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(110, AI_ACTION.SET_STATE)
    action.SetParam(40) -- 喊话结束
    g_AIDebugInfo[110] = '[110] SetParam(State40)'

    -- 喊话结束
    state = ai.NewState(40)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 34)

    ai.SetInitState(21)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction60(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

