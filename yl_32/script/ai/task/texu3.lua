-- 编写者：hejingxian
-- 版本号：10

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
    ai.RegisterUserAction(2003, 'CustomAction3')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 26)

    -- 记录原点
    action = ai.NewAction(26, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(24)
    g_AIDebugInfo[26] = '[26] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(24, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(56)
    g_AIDebugInfo[24] = '[24] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(56, AI_ACTION.SET_STATE)
    action.SetParam(25) -- 待机
    g_AIDebugInfo[56] = '[56] SetParam(State25)'

    -- 待机
    state = ai.NewState(25)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 15)

    -- 站立
    action = ai.NewAction(15, AI_ACTION.STAND)
    action.SetBranch(8)
    g_AIDebugInfo[15] = '[15] STAND()'

    -- 搜索敌人
    action = ai.NewAction(8, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(3, 24)
    g_AIDebugInfo[8] = '[8] SEARCH_ENEMY(param[AlertRange])'

    -- NPC喊话
    action = ai.NewAction(3, 2003)
    action.SetBranch(11)
    g_AIDebugInfo[3] = '[3] CustomAction3()'

    -- 设置主定时器
    action = ai.NewAction(11, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(58)
    g_AIDebugInfo[11] = '[11] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(58, AI_ACTION.SET_STATE)
    action.SetParam(12) -- 喊话结束
    g_AIDebugInfo[58] = '[58] SetParam(State12)'

    -- 喊话结束
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 24)

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction3(pNpc, ai)
    -- 输入自定义动作实现脚本
    local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end
