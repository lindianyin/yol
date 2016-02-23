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
    ai.RegisterUserAction(2238, 'CustomAction238')
    ai.RegisterUserAction(2240, 'CustomAction240')

    -- 初始
    state = ai.NewState(175)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 187)

    -- 记录原点
    action = ai.NewAction(187, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(144)
    g_AIDebugInfo[187] = '[187] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(144, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(245)
    g_AIDebugInfo[144] = '[144] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(245, AI_ACTION.SET_STATE)
    action.SetParam(161) -- 待机状态
    g_AIDebugInfo[245] = '[245] SetParam(State161)'

    -- 待机状态
    state = ai.NewState(161)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 234)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 144)

    -- 选择事件发起者
    action = ai.NewAction(234, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(207, 144)
    g_AIDebugInfo[234] = '[234] SELECT_EVENT_SRC()'

    -- 三选项随机
    action = ai.NewAction(207, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(50, 45, 5)
    action.SetBranch(208, 212, 240)
    g_AIDebugInfo[207] = '[207] RANDOM_TRI_BRANCH(50, 45, 5)'

    -- 释放技能
    action = ai.NewAction(208, AI_ACTION.CAST_SKILL)
    action.SetParam(445, 1)
    action.SetBranch(211, 211)
    g_AIDebugInfo[208] = '[208] CAST_SKILL(445, 1)'

    -- 设置主定时器
    action = ai.NewAction(211, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(247)
    g_AIDebugInfo[211] = '[211] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(247, AI_ACTION.SET_STATE)
    action.SetParam(213) -- 待机状态
    g_AIDebugInfo[247] = '[247] SetParam(State213)'

    -- 待机状态
    state = ai.NewState(213)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 144)

    -- 释放技能
    action = ai.NewAction(212, AI_ACTION.CAST_SKILL)
    action.SetParam(446, 1)
    action.SetBranch(211, 211)
    g_AIDebugInfo[212] = '[212] CAST_SKILL(446, 1)'

    -- NPC喊话
    action = ai.NewAction(240, 2240)
    action.SetBranch(239)
    g_AIDebugInfo[240] = '[240] CustomAction240()'

    -- 设置主定时器
    action = ai.NewAction(239, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(249)
    g_AIDebugInfo[239] = '[239] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(249, AI_ACTION.SET_STATE)
    action.SetParam(241) -- 喊话结束
    g_AIDebugInfo[249] = '[249] SetParam(State241)'

    -- 喊话结束
    state = ai.NewState(241)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 238)

    -- CALL NPC
    action = ai.NewAction(238, 2238)
    action.SetBranch(211)
    g_AIDebugInfo[238] = '[238] CustomAction238()'

    ai.SetInitState(175)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction240(pNpc, ai)
    -- 输入自定义动作实现脚本
    Dialog:NpcBubbleToNearBy(pNpc.dwId, "吼~~")
    return 1
end

function AI:CustomAction238(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1008,nSceneId, 1068,5505,1048576);
	KGameBase.CallNpc(1008,nSceneId, 1022,6289,1048576);
	KGameBase.CallNpc(1008,nSceneId, 1619,6712,1048576);
	KGameBase.CallNpc(1008,nSceneId, 2279,6309,1048576);
    return 1
end

