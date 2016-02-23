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
    ai.RegisterUserAction(2006, 'CustomAction6')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 记录原点
    action = ai.NewAction(2, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(15)
    g_AIDebugInfo[2] = '[2] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(15, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(20)
    g_AIDebugInfo[15] = '[15] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(20, AI_ACTION.SET_STATE)
    action.SetParam(12) -- 待机
    g_AIDebugInfo[20] = '[20] SetParam(State12)'

    -- 待机
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 10)

    -- 站立
    action = ai.NewAction(10, AI_ACTION.STAND)
    action.SetBranch(19)
    g_AIDebugInfo[10] = '[10] STAND()'

    -- 搜索敌人
    action = ai.NewAction(19, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(6, 15)
    g_AIDebugInfo[19] = '[19] SEARCH_ENEMY(param[AlertRange])'

    -- NPC喊话
    action = ai.NewAction(6, 2006)
    action.SetBranch(13)
    g_AIDebugInfo[6] = '[6] CustomAction6()'

    -- 设置主定时器
    action = ai.NewAction(13, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(22)
    g_AIDebugInfo[13] = '[13] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(22, AI_ACTION.SET_STATE)
    action.SetParam(7) -- 喊话结束
    g_AIDebugInfo[22] = '[22] SetParam(State7)'

    -- 喊话结束
    state = ai.NewState(7)

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction6(pNpc, ai)
    -- 输入自定义动作实现脚本
    local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

